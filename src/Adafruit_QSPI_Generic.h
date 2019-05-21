/*
 * Flash_Generic.h
 *
 *  Created on: Jan 2, 2018
 *      Author: deanm
 */

#ifndef ADAFRUIT_QSPI_GENERIC_H_
#define ADAFRUIT_QSPI_GENERIC_H_


#include "Adafruit_QSPI.h"
#include "Adafruit_SPIFlash.h"

/**************************************************************************/
/*! 
    @brief  a class for interfacing with a generic QSPI flash device.
*/
/**************************************************************************/
class Adafruit_QSPI_Generic : public Adafruit_SPIFlash {

public:
	Adafruit_QSPI_Generic() : Adafruit_SPIFlash(0) {}
	~Adafruit_QSPI_Generic() {}

	bool begin(void);

	bool setFlashType(spiflash_type_t t);

	uint8_t readStatus(void);
	void chipErase(void);
	void eraseBlock(uint32_t blocknum);

	/******** SPI FLASH CLASS METHODS *************/

	void GetManufacturerInfo (uint8_t *manufID, uint8_t *deviceID);
	uint32_t GetJEDECID (void);
	
	/* These are needed for compatibility with Adafruit_SPIFlash_FatFs */
	uint32_t readBuffer (uint32_t address, uint8_t *buffer, uint32_t len);
	uint32_t writeBuffer (uint32_t address, uint8_t *buffer, uint32_t len);

	uint8_t  read8(uint32_t addr);
	uint16_t read16(uint32_t addr);
	uint32_t read32(uint32_t addr);

	/**************************************************************************/
	/*! 
		@brief erase a sector of flash
		@param sectorNumber the sector number to erase. The address erased will be (sectorNumber * W25Q16BV_SECTORSIZE)
		@returns true
	*/
	/**************************************************************************/
	bool     EraseSector (uint32_t sectorNumber) { return eraseSector(sectorNumber); }
	bool     eraseSector (uint32_t sectorNumber);


	/******** END SPI FLASH CLASS METHODS *************/
};

#endif /* ADAFRUIT_QSPI_GENERIC_H_ */
