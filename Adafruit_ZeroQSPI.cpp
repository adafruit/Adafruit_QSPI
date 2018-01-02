/*
 * Adafruit_ZeroQSPI.cpp
 *
 *  Created on: Dec 28, 2017
 *      Author: deanm
 */

#include "Adafruit_ZeroQSPI.h"
#include "wiring_private.h"

void QSPIClass::begin() {
	MCLK->AHBMASK.reg |= MCLK_AHBMASK_QSPI;
	MCLK->AHBMASK.reg |= MCLK_AHBMASK_QSPI_2X;
	MCLK->APBCMASK.reg |= MCLK_APBCMASK_QSPI;

	//set all pins to QSPI periph
	pinPeripheral(PIN_QSPI_SCK, PIO_COM);
	pinPeripheral(PIN_QSPI_CS, PIO_COM);
	pinPeripheral(PIN_QSPI_IO0, PIO_COM);
	pinPeripheral(PIN_QSPI_IO1, PIO_COM);
	pinPeripheral(PIN_QSPI_IO2, PIO_COM);
	pinPeripheral(PIN_QSPI_IO3, PIO_COM);

	QSPI->CTRLA.bit.SWRST = 1;

	delay(1); //no syncbusy reg.. do we need this?

	QSPI->CTRLB.reg = QSPI_CTRLB_MODE_MEMORY | QSPI_CTRLB_CSMODE_NORELOAD | QSPI_CTRLB_DATALEN_8BITS | QSPI_CTRLB_CSMODE_LASTXFER;

	QSPI->BAUD.reg = QSPI_BAUD_BAUD(VARIANT_MCK/VARIANT_QSPI_BAUD_DEFAULT);

	QSPI->CTRLA.bit.ENABLE = 1;
}

void QSPIClass::runInstruction(const QSPIInstr *instr, uint32_t addr, uint8_t *txData, uint8_t *rxData, uint32_t size)
{
	uint8_t *qspi_mem = (uint8_t *)QSPI_AHB;
	if(addr)
		qspi_mem += addr;

	QSPI->INSTRCTRL.bit.INSTR = instr->instruction;
	QSPI->INSTRADDR.reg = addr;

	//read to synchronize
	uint32_t iframe = QSPI->INSTRFRAME.reg;

	iframe = QSPI_INSTRFRAME_WIDTH(instr->ioFormat) | instr->options |
			QSPI_INSTRFRAME_OPTCODELEN(instr->opcodeLen) | (instr->addrLen << QSPI_INSTRFRAME_ADDRLEN_Pos) |
			( instr->continuousRead << QSPI_INSTRFRAME_CRMODE_Pos) | QSPI_INSTRFRAME_TFRTYPE(instr->type) | QSPI_INSTRFRAME_DUMMYLEN(instr->dummylen);

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
}

byte QSPIClass::transfer(uint16_t data)
{
	QSPI->TXDATA.reg = data;
	while( !QSPI->INTFLAG.bit.TXC );

	return QSPI->RXDATA.reg;
}

void QSPIClass::transfer(void *buf, size_t count)
{
	uint8_t *buffer = reinterpret_cast<uint8_t *>(buf);
	for (size_t i=0; i<count; i++) {
		*buffer = transfer(*buffer);
		buffer++;
	}
}

void QSPIClass::setMemoryMode(QSPIMode_t mode)
{
	QSPI->CTRLB.bit.MODE = mode;
}

void QSPIClass::setClockDivider(uint8_t uc_div)
{
	QSPI->BAUD.bit.BAUD = uc_div;
}

void QSPIClass::setDataWidth(QSPIDataWidth_t width)
{
	QSPI->CTRLB.bit.DATALEN = QSPI_CTRLB_DATALEN(width);
}

QSPIClass QSPI0;
