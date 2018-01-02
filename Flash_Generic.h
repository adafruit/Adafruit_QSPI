/*
 * Flash_Generic.h
 *
 *  Created on: Jan 2, 2018
 *      Author: deanm
 */

#ifndef ADAFRUIT_ZEROQSPI_FLASH_GENERIC_H_
#define ADAFRUIT_ZEROQSPI_FLASH_GENERIC_H_


#include "Adafruit_ZeroQSPI.h"

class Adafruit_Flash_Generic {

public:
	Adafruit_Flash_Generic() {}
	~Adafruit_Flash_Generic() {}

	bool begin();

	byte readID();
	byte readStatus();
	void chipErase();

	byte read8(uint32_t addr);
	bool write8(uint32_t addr, byte data);

	bool readMemory(uint32_t addr, uint32_t *data, uint32_t size);
	bool writeMemory(uint32_t addr, uint32_t *data, uint32_t size);
};


#endif /* ADAFRUIT_ZEROQSPI_FLASH_GENERIC_H_ */
