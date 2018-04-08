/*
 * Flash_Generic.h
 *
 *  Created on: Jan 2, 2018
 *      Author: deanm
 */

#ifndef ADAFRUIT_SPI_FLASH_GENERIC_H_
#define ADAFRUIT_SPI_FLASH_GENERIC_H_


#include "Adafruit_QSPI.h"

class Adafruit_Flash_Generic {

public:
	Adafruit_QSPI_Generic() {}
	~Adafruit_QSPI_Generic() {}

	bool begin();

	byte readID();
	byte readStatus();
	void chipErase();

	byte read8(uint32_t addr);

	bool readMemory(uint32_t addr, uint8_t *data, uint32_t size);
	bool writeMemory(uint32_t addr, uint8_t *data, uint32_t size);
};

enum {
	FLASH_GENERIC_CMD_DEVID = 0,
	FLASH_GENERIC_CMD_MFGID,
	FLASH_GENERIC_READ_STATUS,
	FLASH_GENERIC_CMD_WRITE_STATUS,
	FLASH_GENERIC_CMD_WRITE_ENABLE,
	FLASH_GENERIC_CMD_WRITE_DISABLE,
	FLASH_GENERIC_CMD_CHIP_ERASE,
	FLASH_GENERIC_CMD_PAGE_PROGRAM,
	FLASH_GENERIC_CMD_QUAD_READ,
};

extern const QSPIInstr cmdSetGeneric[];


#endif /* ADAFRUIT_QSPI_FLASH_GENERIC_H_ */
