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

	bool begin();

	bool setFlashType(spiflash_type_t t);

	byte readDeviceID();
	byte readManufacturerID();
	byte readStatus();
	void chipErase();
	void eraseBlock(uint32_t blocknum);

	byte read8(uint32_t addr);

	bool readMemory(uint32_t addr, uint8_t *data, uint32_t size, bool invalidateCache=true);
	bool writeMemory(uint32_t addr, uint8_t *data, uint32_t size);

	/******** SPI FLASH CLASS METHODS *************/

	void GetManufacturerInfo (uint8_t *manufID, uint8_t *deviceID);
	uint32_t GetJEDECID (void);
	
	/* These are needed for compatibility with Adafruit_SPIFlash_FatFs */
	uint32_t readBuffer (uint32_t address, uint8_t *buffer, uint32_t len);

	/**************************************************************************/
	/*! 
		@brief erase a sector of flash
		@param sectorNumber the sector number to erase. The address erased will be (sectorNumber * W25Q16BV_SECTORSIZE)
		@returns true
	*/
	/**************************************************************************/
	bool     EraseSector (uint32_t sectorNumber) { return eraseSector(sectorNumber); }
	bool     eraseSector (uint32_t sectorNumber);

	// Write an arbitrary-sized buffer
	uint32_t writeBuffer (uint32_t address, uint8_t *buffer, uint32_t len);

	/******** END SPI FLASH CLASS METHODS *************/
};

enum {
	ADAFRUIT_QSPI_GENERIC_CMD_DEVID = 0,
	ADAFRUIT_QSPI_GENERIC_CMD_MFGID,
	ADAFRUIT_QSPI_GENERIC_READ_STATUS,
	ADAFRUIT_QSPI_GENERIC_CMD_WRITE_STATUS,
	ADAFRUIT_QSPI_GENERIC_CMD_WRITE_ENABLE,
	ADAFRUIT_QSPI_GENERIC_CMD_WRITE_DISABLE,
	ADAFRUIT_QSPI_GENERIC_CMD_CHIP_ERASE,
	ADAFRUIT_QSPI_GENERIC_CMD_SECTOR_ERASE,
	ADAFRUIT_QSPI_GENERIC_CMD_BLOCK64K_ERASE,
	ADAFRUIT_QSPI_GENERIC_CMD_PAGE_PROGRAM,
	ADAFRUIT_QSPI_GENERIC_CMD_QUAD_READ,
	ADAFRUIT_QSPI_GENERIC_CMD_RDID,
};

extern const QSPIInstr cmdSetGeneric[];


#endif /* ADAFRUIT_QSPI_GENERIC_H_ */
