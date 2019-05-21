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
    @brief  Class for interfacing with QSPI hardware
*/
/**************************************************************************/
class Adafruit_QSPI_SAMD : Adafruit_QSPI
{
public:
	Adafruit_QSPI_SAMD(void);
	~Adafruit_QSPI_SAMD() {};

	virtual void begin(int sck, int cs, int io0, int io1, int io2, int io3);
	void begin(void)
	{
	  begin(PIN_QSPI_SCK, PIN_QSPI_CS, PIN_QSPI_IO0, PIN_QSPI_IO1, PIN_QSPI_IO2, PIN_QSPI_IO3);
	}

	void end(); ///< de-init the peripheral

	virtual void setClockDivider(uint8_t uc_div);
	virtual void setClockSpeed(uint32_t clock_hz);

	virtual bool runCommand(uint8_t command);
	virtual bool readCommand(uint8_t comamnd, uint8_t* response, uint32_t len);
	virtual bool writeCommand(uint8_t command, uint8_t const* data, uint32_t len);


	virtual void eraseSector(uint32_t sectorAddr);
	virtual bool readMemory(uint32_t addr, uint8_t *data, uint32_t size);
	virtual bool writeMemory(uint32_t addr, uint8_t *data, uint32_t size);

	byte readStatus();

private:
	bool runInstruction(uint8_t command, uint32_t ifr, uint32_t addr, uint8_t *buffer, uint32_t size);
};

extern Adafruit_QSPI_SAMD QSPI0; ///< default QSPI instance

#endif
