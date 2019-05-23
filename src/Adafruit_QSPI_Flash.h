/* The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach and Dean Miller for Adafruit Industries LLC
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef ADAFRUIT_QSPI_FLASH_H_
#define ADAFRUIT_QSPI_FLASH_H_


#include "Adafruit_QSPI.h"
#include "Adafruit_SPIFlash.h"
#include "external_flash_device.h"

/**************************************************************************/
/*! 
    @brief  a class for interfacing with a generic QSPI flash device.
*/
/**************************************************************************/
class Adafruit_QSPI_Flash : public Adafruit_SPIFlash {

public:
  // Constant that is (mostly) true to all external flash devices
  enum {
    QSPI_FLASH_SECTOR_SIZE = 4096,
    QSPI_FLASH_PAGE_SIZE   = 256,
  };

	Adafruit_QSPI_Flash(void);
	~Adafruit_QSPI_Flash() {}

	bool begin(void);
	bool end(void);

	uint8_t readStatus(void);
	uint8_t readStatus2(void);
	bool writeEnable(void);
	bool chipErase(void);

	/******** SPI FLASH CLASS METHODS *************/

	void GetManufacturerInfo (uint8_t *manufID, uint8_t *deviceID);
	uint32_t GetJEDECID (void);
	
	uint32_t readBuffer (uint32_t address, uint8_t *buffer, uint32_t len);
	uint32_t writeBuffer (uint32_t address, uint8_t *buffer, uint32_t len);

	bool     EraseSector (uint32_t sectorNumber) { return eraseSector(sectorNumber); }
	bool     eraseSector (uint32_t sectorNumber);

	// Helper
	uint8_t  read8(uint32_t addr);
	uint16_t read16(uint32_t addr);
	uint32_t read32(uint32_t addr);

private:
	external_flash_device const * _flash_dev;

	void _wait_for_flash_ready(void)
	{
	  // both WIP and WREN bit should be clear
	  while ( readStatus() & 0x03 ) {}
	}
};

#endif /* ADAFRUIT_QSPI_FLASH_H_ */
