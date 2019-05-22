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

// Turn off cache and invalidate all data in it.
static void samd_peripherals_disable_and_clear_cache(void)
{
  CMCC->CTRL.bit.CEN = 0;
  while ( CMCC->SR.bit.CSTS ) { }
  CMCC->MAINT0.bit.INVALL = 1;
}

// Enable cache
static void samd_peripherals_enable_cache (void)
{
  CMCC->CTRL.bit.CEN = 1;
}

Adafruit_QSPI_SAMD::Adafruit_QSPI_SAMD(void)
{

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
    @brief Run a single QSPI instruction.
    @param command instruction code
    @param iframe iframe register value (configured by caller according to command code)
    @param addr the address to read or write from. If the instruction doesn't require an address, this parameter is meaningless.
    @param buffer pointer to the data to be written or stored depending on the type is Read or Write
    @param size the number of bytes to read or write.
*/
/**************************************************************************/
bool Adafruit_QSPI_SAMD::_run_instruction(uint8_t command, uint32_t iframe, uint32_t addr, uint8_t *buffer, uint32_t size)
{
  samd_peripherals_disable_and_clear_cache();

	uint8_t *qspi_mem = (uint8_t *)QSPI_AHB;
	if(addr)
		qspi_mem += addr;

	QSPI->INSTRCTRL.bit.INSTR = command;
	QSPI->INSTRADDR.reg = addr;

	QSPI->INSTRFRAME.reg = iframe;

	// Dummy read of INSTRFRAME needed to synchronize.
	// See Instruction Transmission Flow Diagram, figure 37.9, page 995
	// and Example 4, page 998, section 37.6.8.5.
	(volatile uint32_t) QSPI->INSTRFRAME.reg;

	if ( buffer && size )
	{
	  uint32_t const tfr_type = iframe & QSPI_INSTRFRAME_TFRTYPE_Msk;

	  if ( (tfr_type == QSPI_INSTRFRAME_TFRTYPE_READ) || (tfr_type == QSPI_INSTRFRAME_TFRTYPE_READMEMORY) )
	  {
	    memcpy(buffer, qspi_mem, size);
	  }else
	  {
	    memcpy(qspi_mem, buffer, size);
	  }
	}

	__asm__ volatile ("dsb");
	__asm__ volatile ("isb");

	QSPI->CTRLA.reg = QSPI_CTRLA_ENABLE | QSPI_CTRLA_LASTXFER;

	while( !QSPI->INTFLAG.bit.INSTREND ) {}
	QSPI->INTFLAG.bit.INSTREND = 1;

	samd_peripherals_enable_cache();

	return true;
}

bool Adafruit_QSPI_SAMD::runCommand(uint8_t command)
{
	uint32_t iframe = QSPI_INSTRFRAME_WIDTH_SINGLE_BIT_SPI | QSPI_INSTRFRAME_ADDRLEN_24BITS |
                    QSPI_INSTRFRAME_TFRTYPE_READ | QSPI_INSTRFRAME_INSTREN;

	return _run_instruction(command, iframe, 0, NULL, 0);
}

bool Adafruit_QSPI_SAMD::readCommand(uint8_t command, uint8_t* response, uint32_t len)
{
  uint32_t iframe = QSPI_INSTRFRAME_WIDTH_SINGLE_BIT_SPI | QSPI_INSTRFRAME_ADDRLEN_24BITS |
                    QSPI_INSTRFRAME_TFRTYPE_READ | QSPI_INSTRFRAME_INSTREN | QSPI_INSTRFRAME_DATAEN;

  return _run_instruction(command, iframe, 0, response, len);
}

bool Adafruit_QSPI_SAMD::writeCommand(uint8_t command, uint8_t const* data, uint32_t len)
{
	uint32_t iframe = QSPI_INSTRFRAME_WIDTH_SINGLE_BIT_SPI | QSPI_INSTRFRAME_ADDRLEN_24BITS |
	                  QSPI_INSTRFRAME_TFRTYPE_WRITE | QSPI_INSTRFRAME_INSTREN | (data != NULL ? QSPI_INSTRFRAME_DATAEN : 0);

	return _run_instruction(command, iframe, 0, (uint8_t*) data, len);
}

bool Adafruit_QSPI_SAMD::eraseSector(uint32_t sectorAddr)
{
	// Sector Erase
	uint32_t iframe = QSPI_INSTRFRAME_WIDTH_SINGLE_BIT_SPI | QSPI_INSTRFRAME_ADDRLEN_24BITS |
                    QSPI_INSTRFRAME_TFRTYPE_WRITE | QSPI_INSTRFRAME_INSTREN | QSPI_INSTRFRAME_ADDREN;

	return _run_instruction(QSPI_CMD_ERASE_SECTOR, iframe, sectorAddr, NULL, 0);
}

bool Adafruit_QSPI_SAMD::readMemory(uint32_t addr, uint8_t *data, uint32_t size)
{
  // Command 0x6B 1 line address, 4 line Data
  // with Continuous Read Mode and Quad output mode, read memory type
  uint32_t iframe = QSPI_INSTRFRAME_WIDTH_QUAD_OUTPUT | QSPI_INSTRFRAME_ADDRLEN_24BITS |
                    QSPI_INSTRFRAME_TFRTYPE_READMEMORY | QSPI_INSTRFRAME_INSTREN | QSPI_INSTRFRAME_ADDREN | QSPI_INSTRFRAME_DATAEN |
                    /*QSPI_INSTRFRAME_CRMODE |*/ QSPI_INSTRFRAME_DUMMYLEN(8);

  return _run_instruction(QSPI_CMD_QUAD_READ, iframe, addr, data, size);
}

bool Adafruit_QSPI_SAMD::writeMemory(uint32_t addr, uint8_t *data, uint32_t size)
{
  uint32_t iframe = QSPI_INSTRFRAME_WIDTH_QUAD_OUTPUT | QSPI_INSTRFRAME_ADDRLEN_24BITS |
                    QSPI_INSTRFRAME_TFRTYPE_WRITEMEMORY | QSPI_INSTRFRAME_INSTREN | QSPI_INSTRFRAME_ADDREN | QSPI_INSTRFRAME_DATAEN;

  return _run_instruction(QSPI_CMD_QUAD_PAGE_PROGRAM, iframe, addr, data, size);
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

void Adafruit_QSPI_SAMD::setClockSpeed(uint32_t clock_hz)
{
  QSPI->BAUD.bit.BAUD = VARIANT_MCK/clock_hz;
}

#endif
