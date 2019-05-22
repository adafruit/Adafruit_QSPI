/*
 * Flash_Generic.h
 *
 *  Created on: Jan 2, 2018
 *      Author: deanm
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
  enum { FLASH_SECTOR_SIZE = 4096 };

	Adafruit_QSPI_Flash(void);
	~Adafruit_QSPI_Flash() {}

	bool begin(void);

	bool setFlashType(spiflash_type_t t);

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
	  while ( readStatus() & 0x01 ) {}
	}
};

#endif /* ADAFRUIT_QSPI_FLASH_H_ */
