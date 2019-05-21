/*
 * Flash_Generic.cpp
 *
 *  Created on: Jan 2, 2018
 *      Author: deanm
 */

#include "Adafruit_QSPI_Flash.h"

#define ADAFRUIT_QSPI_GENERIC_STATUS_BUSY 0x01

// Include all possible supported flash devices used by Adafruit boards
const external_flash_device possible_devices[] =
{
  GD25Q16C, GD25Q64C,
  S25FL116K, S25FL216K,
  W25Q16FW, W25Q32BV, W25Q64JV_IQ,
  MX25R6435F,
};

enum
{
  EXTERNAL_FLASH_DEVICE_COUNT = sizeof(possible_devices)/sizeof(possible_devices[0])
};

Adafruit_QSPI_Flash::Adafruit_QSPI_Flash(void) : Adafruit_SPIFlash(0)
{
  _flash_dev = NULL;
}

/**************************************************************************/
/*! 
    @brief begin the default QSPI peripheral
    @returns true
*/
/**************************************************************************/
bool Adafruit_QSPI_Flash::begin(void){

	QSPI0.begin();

	uint8_t jedec_ids[3];
	QSPI0.readCommand(QSPI_CMD_READ_JEDEC_ID, jedec_ids, 3);

	for (uint8_t i = 0; i < EXTERNAL_FLASH_DEVICE_COUNT; i++) {
	  const external_flash_device* possible_device = &possible_devices[i];
	  if (jedec_ids[0] == possible_device->manufacturer_id &&
	      jedec_ids[1] == possible_device->memory_type &&
	      jedec_ids[2] == possible_device->capacity) {
	    _flash_dev = possible_device;
	    break;
	  }
	}

	if (_flash_dev == NULL) return false;

  // We don't know what state the flash is in so wait for any remaining writes and then reset.

  // The write in progress bit should be low.
  while ( readStatus() & 0x01 ) {}

  // The suspended write/erase bit should be low.
  while ( readStatus2() & 0x80 ) {}

  QSPI0.runCommand(QSPI_CMD_ENABLE_RESET);
  QSPI0.runCommand(QSPI_CMD_RESET);

  // Wait 30us for the reset
  delayMicroseconds(30);

  // Enable Quad Mode if available
  if (_flash_dev->quad_enable_bit_mask)
  {
    // Verify that QSPI mode is enabled.
    uint8_t status = _flash_dev->single_status_byte ? readStatus() : readStatus2();

    // Check the quad enable bit.
    if ((status & _flash_dev->quad_enable_bit_mask) == 0) {
        writeEnable();

        uint8_t full_status[2] = {0x00, _flash_dev->quad_enable_bit_mask};

        if (_flash_dev->write_status_register_split) {
            QSPI0.writeCommand(QSPI_CMD_WRITE_STATUS2, full_status + 1, 1);
        } else if (_flash_dev->single_status_byte) {
            QSPI0.writeCommand(QSPI_CMD_WRITE_STATUS, full_status + 1, 1);
        } else {
            QSPI0.writeCommand(QSPI_CMD_WRITE_STATUS, full_status, 2);
        }
    }
  }

  if (_flash_dev->has_sector_protection)  {
    writeEnable();

    // Turn off sector protection
    uint8_t data[1] = {0x00};
    QSPI0.writeCommand(QSPI_CMD_WRITE_STATUS, data, 1);
  }

  // Turn off writes in case this is a microcontroller only reset.
  QSPI0.runCommand(QSPI_CMD_WRITE_DISABLE);

  // wait for flash ready
  while ( readStatus() & 0x01 ) {}

  // Adafruit_SPIFlash variables
  currentAddr = 0;
  totalsize = _flash_dev->total_size;

//  type, addrsize are ignored
  pagesize = 256;
  pages = totalsize/256;

	return true;
}

/**************************************************************************/
/*! 
    @brief read the manufacturer ID and device ID
    @param manufID pointer to where to put the manufacturer ID
	@param deviceID pointer to where to put the device ID
*/
/**************************************************************************/
void Adafruit_QSPI_Flash::GetManufacturerInfo (uint8_t *manufID, uint8_t *deviceID)
{
  uint32_t jedec_id = GetJEDECID();

	*deviceID = (uint8_t) (jedec_id & 0xffUL);
	*manufID  = (uint8_t) (jedec_id >> 16);
}

/**************************************************************************/
/*! 
    @brief read JEDEC ID information from the device
	@returns the read id as a uint32
*/
/**************************************************************************/
uint32_t Adafruit_QSPI_Flash::GetJEDECID (void)
{
	uint8_t ids[3];
	QSPI0.readCommand(QSPI_CMD_READ_JEDEC_ID, ids, 3);

	return (ids[0] << 16) | (ids[1] << 8) | ids[2];
}

/**************************************************************************/
/*! 
    @brief read the generic status register.
    @returns the status register reading
*/
/**************************************************************************/
uint8_t Adafruit_QSPI_Flash::readStatus(void)
{
	uint8_t r;
	QSPI0.readCommand(QSPI_CMD_READ_STATUS, &r, 1);
	return r;
}

uint8_t Adafruit_QSPI_Flash::readStatus2(void)
{
	uint8_t r;
	QSPI0.readCommand(QSPI_CMD_READ_STATUS2, &r, 1);
	return r;
}

bool Adafruit_QSPI_Flash::writeEnable(void)
{
  return QSPI0.runCommand(QSPI_CMD_WRITE_ENABLE);
}

// Read flash contents into buffer
uint32_t Adafruit_QSPI_Flash::readBuffer (uint32_t address, uint8_t *buffer, uint32_t len)
{
  return QSPI0.readMemory(address, buffer, len) ? len : 0;
}

// Write buffer into flash
uint32_t Adafruit_QSPI_Flash::writeBuffer (uint32_t address, uint8_t *buffer, uint32_t len)
{
	return QSPI0.writeMemory(address, buffer, len) ? len : 0;
}

uint8_t Adafruit_QSPI_Flash::read8(uint32_t addr)
{
	uint8_t ret;
	return readBuffer(addr, &ret, sizeof(ret)) ? 0xff : ret;
}

uint16_t Adafruit_QSPI_Flash::read16(uint32_t addr)
{
	uint16_t ret;
	return readBuffer(addr, (uint8_t*) &ret, sizeof(ret)) ? 0xffff : ret;
}

uint32_t Adafruit_QSPI_Flash::read32(uint32_t addr)
{
	uint32_t ret;
	return readBuffer(addr, (uint8_t*) &ret, sizeof(ret)) ? 0xffffffff : ret;
}

/**************************************************************************/
/*! 
    @brief perform a chip erase. All data on the device will be erased.
*/
/**************************************************************************/
void Adafruit_QSPI_Flash::chipErase(void)
{
	writeEnable();
	QSPI0.runCommand(QSPI_CMD_ERASE_CHIP);

	//wait for busy
	while(readStatus() & ADAFRUIT_QSPI_GENERIC_STATUS_BUSY);
}

/**************************************************************************/
/*! 
    @brief erase a block of data
    @param blocknum the number of the block to erase.
*/
/**************************************************************************/
void Adafruit_QSPI_Flash::eraseBlock(uint32_t blocknum)
{
	writeEnable();

//	QSPI0.runInstruction(&cmdSetGeneric[ADAFRUIT_QSPI_GENERIC_CMD_BLOCK64K_ERASE], blocknum*W25Q16BV_BLOCKSIZE, NULL, NULL, 0);
//	QSPI0.
	// TODO not implement

	//wait for busy
	while(readStatus() & ADAFRUIT_QSPI_GENERIC_STATUS_BUSY);
}

/**************************************************************************/
/*! 
    @brief erase a sector of flash
    @param sectorNumber the sector number to erase. The address erased will be (sectorNumber * W25Q16BV_SECTORSIZE)
    @returns true
*/
/**************************************************************************/
bool Adafruit_QSPI_Flash::eraseSector (uint32_t sectorNumber)
{
	uint32_t address = sectorNumber * W25Q16BV_SECTORSIZE;
	QSPI0.eraseSector(address);
	return true;
}
