/*!
 * @file Adafruit_QSPI_SAMD.h
 *
 * This is part of Adafruit's QSPI Peripheral driver for the Arduino platform.  It is
 * designed specifically to work with the Adafruit M4 Express development boards.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Written by Dean Miller for Adafruit Industries.
 *
 * BSD license, all text here must be included in any redistribution.
 *
 */

#ifndef ADAFRUIT_QSPI_SAMD_H
#define ADAFRUIT_QSPI_SAMD_H

#include "SPI.h"
#include <Arduino.h>

/**************************************************************************/
/*!
    @brief  The transfer mode
*/
/**************************************************************************/
typedef enum {
	QSPI_SPI_MODE = 0,
	QSPI_MEMORY_MODE,
} QSPIMode_t;


/**************************************************************************/
/*! 
    @brief  Class for interfacing with QSPI hardware
*/
/**************************************************************************/
class Adafruit_QSPI_SAMD : Adafruit_QSPI
{
public:
	Adafruit_QSPI_SAMD(void);
	~Adafruit_QSPI_SAMD() {};

	virtual void begin(void);
	void end(); ///< de-init the peripheral

	virtual void setClockDivider(uint8_t uc_div);

	virtual void runInstruction(const QSPIInstr *instr, uint32_t addr, uint8_t *txData, uint8_t *rxData, uint32_t size);
	using Adafruit_QSPI::runInstruction;

	virtual void eraseSector(uint32_t sectorAddr);
	virtual bool readMemory(uint32_t addr, uint8_t *data, uint32_t size);
	virtual bool writeMemory(uint32_t addr, uint8_t *data, uint32_t size);

	byte transfer(uint16_t data);
	void transfer(void *buf, size_t count);

	void setMemoryMode(QSPIMode_t mode);

	byte readStatus();

private:
	uint8_t _addr_len;
	uint8_t _cont_read;
	uint8_t _io_format;
	uint8_t _tfr_type;
};

extern Adafruit_QSPI_SAMD QSPI0; ///< default QSPI instance

#endif
