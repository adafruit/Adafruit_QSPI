/*!
 * @file Adafruit_QSPI.cpp
 *
 * @mainpage Adafruit QSPI library
 *
 * @section intro_sec Introduction
 *
 * This is documentation for Adafruit's library for interfacing with the QSPI
 * peripheral on specific microcontrollers. Currently only the SAMD51 is supported.
 * This is intened to be used with the Adafruit Metro M4 Express, Adafruit Feather M4 Express,
 * and other M4 Express development boards.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * @section author Author
 *
 * Written by Dean Miller for Adafruit Industries.
 *
 * @section license License
 *
 * BSD license, all text here must be included in any redistribution.
 *
 */
#ifdef __SAMD51__

#include "Adafruit_QSPI.h"
#include "wiring_private.h"

Adafruit_QSPI_SAMD QSPI0;

// dummy: 0, ddren: 0, crmode: 0, tfrtype: 0
// addrlen: 24 bit, optcodelen: 1 bit
// dataen: 0, optcodeen: 0, addren: 0, instren: 1
// wdith: single bit spi
#define IFRAME_DEFAULT  (QSPI_INSTRFRAME_ADDRLEN_24BITS | QSPI_INSTRFRAME_OPTCODELEN_1BIT | QSPI_INSTRFRAME_WIDTH_SINGLE_BIT_SPI)

Adafruit_QSPI_SAMD::Adafruit_QSPI_SAMD(void)
{
  _iframe = IFRAME_DEFAULT;
}

void Adafruit_QSPI_SAMD::begin(int sck, int cs, int io0, int io1, int io2, int io3)
{
	MCLK->AHBMASK.reg |= MCLK_AHBMASK_QSPI;
	MCLK->AHBMASK.reg |= MCLK_AHBMASK_QSPI_2X;
	MCLK->APBCMASK.reg |= MCLK_APBCMASK_QSPI;

	//set all pins to QSPI periph
	pinPeripheral(sck, PIO_COM);
	pinPeripheral(cs, PIO_COM);
	pinPeripheral(io0, PIO_COM);
	pinPeripheral(io1, PIO_COM);
	pinPeripheral(io2, PIO_COM);
	pinPeripheral(io3, PIO_COM);

	QSPI->CTRLA.bit.SWRST = 1;

	delay(1); //no syncbusy reg.. do we need this? Probably not

	QSPI->CTRLB.reg = QSPI_CTRLB_MODE_MEMORY | QSPI_CTRLB_CSMODE_NORELOAD | QSPI_CTRLB_DATALEN_8BITS | QSPI_CTRLB_CSMODE_LASTXFER;

	QSPI->BAUD.reg = QSPI_BAUD_BAUD(VARIANT_MCK/VARIANT_QSPI_BAUD_DEFAULT);

	QSPI->CTRLA.bit.ENABLE = 1;
}

/**************************************************************************/
/*! 
    @brief  Run a single QSPI instruction.
    @param instr pointer to the struct containing instruction settings
    @param addr the address to read or write from. If the instruction doesn't require an address, this parameter is meaningless.
    @param txData pointer to the data to be written.
    @param rxData pointer to where read data should be stored.
    @param size the number of bytes to read.
	@param invalidateCache manual cache management. Only use this parameter if you know what you're doing. Defaults to true.
*/
/**************************************************************************/
void Adafruit_QSPI_SAMD::runInstruction(const QSPIInstr *instr, uint32_t addr, uint8_t *txData, uint8_t *rxData, uint32_t size)
{
	bool needToEnableCache = false;
	bool invalidateCache = true;
	if(invalidateCache && CMCC->SR.bit.CSTS){
		// clear the cache
		CMCC->CTRL.bit.CEN = 0; // disable
		while(CMCC->SR.bit.CSTS);
		CMCC->MAINT0.bit.INVALL = 1; // invalidate all
		needToEnableCache = true;
	}

	uint8_t *qspi_mem = (uint8_t *)QSPI_AHB;
	if(addr)
		qspi_mem += addr;

	QSPI->INSTRCTRL.bit.INSTR = instr->instruction;
	QSPI->INSTRADDR.reg = addr;

	//read to synchronize
	uint32_t iframe = QSPI->INSTRFRAME.reg;

	iframe = _iframe;
	iframe |= QSPI_INSTRFRAME_DUMMYLEN(instr->dummylen) | QSPI_INSTRFRAME_INSTREN |
	          (instr->has_addr ? QSPI_INSTRFRAME_ADDREN : 0 ) | (instr->has_data ? QSPI_INSTRFRAME_DATAEN : 0);

	// Set to Write if not read/write memory but sending out data
	if ( !(_iframe & QSPI_INSTRFRAME_TFRTYPE_Msk) && txData && size )
	{
	  iframe |= QSPI_INSTRFRAME_TFRTYPE_WRITE;
	}

	QSPI->INSTRFRAME.reg = iframe;

	if(rxData != NULL){
		while(size){
			*rxData++ = *qspi_mem++;
			size--;
		}
	}
	else if(txData != NULL){
		while(size){
			*qspi_mem++ = *txData++;
			size--;
		}
	}

	__asm__ volatile ("dsb");
	__asm__ volatile ("isb");

	QSPI->CTRLA.reg = QSPI_CTRLA_ENABLE | QSPI_CTRLA_LASTXFER;

	while( !QSPI->INTFLAG.bit.INSTREND );

	QSPI->INTFLAG.bit.INSTREND = 1;

	if(needToEnableCache) 
		CMCC->CTRL.bit.CEN = 1; // re-enable
}

void Adafruit_QSPI_SAMD::eraseSector(uint32_t sectorAddr)
{
	//Write Enable
  const QSPIInstr cmd_wren = { 0x06, 0, false, false };
	runInstruction(&cmd_wren);

	// Sector Erase
  const QSPIInstr cmd_erase_sector = { 0x20, 0, true, false};
	runInstruction(&cmd_erase_sector, sectorAddr, NULL, NULL, 0);

	//wait for busy
	while(readStatus() & 0x01);
}

bool Adafruit_QSPI_SAMD::readMemory(uint32_t addr, uint8_t *data, uint32_t size)
{
  // Command 0x6B 1 line address, 4 line Data
  // with Continuous Read Mode and Quad output mode, read memory type
  const QSPIInstr cmd_read = { 0x6B, 8, true, true };

  _iframe = QSPI_INSTRFRAME_WIDTH_QUAD_OUTPUT | QSPI_INSTRFRAME_TFRTYPE_READMEMORY | QSPI_INSTRFRAME_CRMODE;
  runInstruction(&cmd_read, addr, NULL, data, size);
  _iframe = IFRAME_DEFAULT;

  return true;
}

bool Adafruit_QSPI_SAMD::writeMemory(uint32_t addr, uint8_t *data, uint32_t size)
{
  //Write Enable
  const QSPIInstr cmd_wren = { 0x06, 0, false, false };

  //Page Program
	const QSPIInstr cmd_pageprog = { 0x02, 0, true, true };

	uint16_t toWrite = 0;

	//write one page at a time
	while(size){
	  runInstruction(&cmd_wren);

		if(size > 256) toWrite = 256;
		else toWrite = size;
		size -= toWrite;

		_iframe = QSPI_INSTRFRAME_TFRTYPE_WRITEMEMORY;
		runInstruction(&cmd_pageprog, addr, data, NULL, toWrite);
		_iframe = IFRAME_DEFAULT;

		data += toWrite;
		addr += toWrite;

		while(readStatus() & 0x01);
	}

	return true;
}

byte Adafruit_QSPI_SAMD::readStatus()
{
  //read status register
	const QSPIInstr cmd_read_status =	{ 0x05, 0, false, true };

	byte r;
	runInstruction(&cmd_read_status, 0, NULL, &r, 1);
	return r;
}


/**************************************************************************/
/*! 
    @brief  set the clock divider
    @param uc_div the divider to set. Must be a value between 0 and 255. Note that QSPI uses GCLK0.
*/
/**************************************************************************/
void Adafruit_QSPI_SAMD::setClockDivider(uint8_t uc_div)
{
	QSPI->BAUD.bit.BAUD = uc_div;
}

/**************************************************************************/
/*!
    @brief transfer data via QSPI
    @param data the data to be sent
    @returns the data that was read
*/
/**************************************************************************/
byte Adafruit_QSPI_SAMD::transfer(uint16_t data)
{
	QSPI->TXDATA.reg = data;
	while( !QSPI->INTFLAG.bit.TXC );

	return QSPI->RXDATA.reg;
}

/**************************************************************************/
/*! 
    @brief  transfer multiple bytes via QSPI
    @param buf the bufer to read from and write to. This buffer will be modified by the function.
    @param count the number of bytes to transfer.
*/
/**************************************************************************/
void Adafruit_QSPI_SAMD::transfer(void *buf, size_t count)
{
	uint8_t *buffer = reinterpret_cast<uint8_t *>(buf);
	for (size_t i=0; i<count; i++) {
		*buffer = transfer(*buffer);
		buffer++;
	}
}

#endif
