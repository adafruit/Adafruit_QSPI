/*
 * Flash_Generic.cpp
 *
 *  Created on: Jan 2, 2018
 *      Author: deanm
 */

#include "Adafruit_QSPI_Generic.h"

#define ADAFRUIT_QSPI_GENERIC_STATUS_BUSY 0x01

/**************************************************************************/
/*! 
    @brief a command set for a generic QSPI flash device
*/
/**************************************************************************/
const QSPIInstr cmdSetGeneric[] = {
		//Device ID
		{ 0xAB, true, true },
		//Manufacturer ID
		{ 0x90, true, true },
		//read status register
		{ 0x05, false, true },
		//write status
		{ 0x01, false, true },
		//Write Enable
		{ 0x06, false, false },
		//Write Disable
		{ 0x04, false, false },
		//Chip Erase
		{ 0xC7, false, false},
		// Sector Erase
		{ 0x20, true, false },
		// Block Erase 64KB
		{ 0xD8, true, false },
		//Page Program
		{ 0x02, true, true },
		//Quad Read
		{ 0x6B, true, true },
		//Read ID
		{ 0x9F, false, true },
};

/**************************************************************************/
/*! 
    @brief begin the default QSPI peripheral
    @returns true
*/
/**************************************************************************/
bool Adafruit_QSPI_Generic::begin(void){
	currentAddr = 0;
	QSPI0.begin();
	return true;
}

/**************************************************************************/
/*! 
    @brief set the type of flash. Setting the type is necessary for use with a FAT filesystem.
    @param t the type of flash to set.
	@returns true if a valid flash type was passed in, false otherwise.
*/
/**************************************************************************/
bool Adafruit_QSPI_Generic::setFlashType(spiflash_type_t t){
  type = t;

  if (type == SPIFLASHTYPE_W25Q16BV) {
    pagesize = 256;
    addrsize = 24;
    pages = 8192;
    totalsize = pages * pagesize; // 2 MBytes
  } 
  else if (type == SPIFLASHTYPE_25C02) {
    pagesize = 32;
    addrsize = 16;
    pages = 8;
    totalsize = pages * pagesize; // 256 bytes
  } 
  else if (type == SPIFLASHTYPE_W25X40CL) {
    pagesize = 256;
    addrsize = 24;
    pages = 2048;
    totalsize =  pages * pagesize; // 512 Kbytes
  } else if (type == SPIFLASHTYPE_AT25SF041) {
    pagesize = 256;
    addrsize = 24;
    pages = 4096;
    totalsize = pages * pagesize;  // 1 MBytes
  } else if (type == SPIFLASHTYPE_W25Q64) {
	  pagesize = 256;
	  addrsize = 24;
	  pages = 32768;
	  totalsize = pages * pagesize; // 8 MBytes
  }
  else {
    pagesize = 0;
    return false;
  }

  return true;
}

/**************************************************************************/
/*! 
    @brief read the manufacturer ID and device ID
    @param manufID pointer to where to put the manufacturer ID
	@param deviceID pointer to where to put the device ID
*/
/**************************************************************************/
void Adafruit_QSPI_Generic::GetManufacturerInfo (uint8_t *manufID, uint8_t *deviceID)
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
uint32_t Adafruit_QSPI_Generic::GetJEDECID (void)
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
byte Adafruit_QSPI_Generic::readStatus(void)
{
	byte r;
	QSPI0.readCommand(QSPI_CMD_READ_STATUS, &r, 1);
	return r;
}

/**************************************************************************/
/*! 
    @brief perform a chip erase. All data on the device will be erased.
*/
/**************************************************************************/
void Adafruit_QSPI_Generic::chipErase(void)
{
	QSPI0.runCommand(QSPI_CMD_ENABLE_WRITE);
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
void Adafruit_QSPI_Generic::eraseBlock(uint32_t blocknum)
{
	QSPI0.runCommand(QSPI_CMD_ENABLE_WRITE);

//	QSPI0.runInstruction(&cmdSetGeneric[ADAFRUIT_QSPI_GENERIC_CMD_BLOCK64K_ERASE], blocknum*W25Q16BV_BLOCKSIZE, NULL, NULL, 0);
//	QSPI0.
	// TODO not implement

	//wait for busy
	while(readStatus() & ADAFRUIT_QSPI_GENERIC_STATUS_BUSY);
}

/**************************************************************************/
/*! 
    @brief read one byte of data at the passed address
    @param addr the address to read from
    @returns the data byte read
*/
/**************************************************************************/
uint8_t Adafruit_QSPI_Generic::read8(uint32_t addr)
{
	uint8_t ret;
	return readMemory(addr, &ret, sizeof(ret)) ? 0xff : ret;
}

uint16_t Adafruit_QSPI_Generic::read16(uint32_t addr)
{
	uint16_t ret;
	return readMemory(addr, (uint8_t*) &ret, sizeof(ret)) ? 0xffff : ret;
}

uint32_t Adafruit_QSPI_Generic::read32(uint32_t addr)
{
	uint32_t ret;
	return readMemory(addr, (uint8_t*) &ret, sizeof(ret)) ? 0xffffffff : ret;
}


/**************************************************************************/
/*! 
    @brief read a chunk of memory from the device
    @param addr the address to read from
    @param data the pointer to where the read data will be stored
    @param size the number of bytes to read
    @returns true
*/
/**************************************************************************/
bool Adafruit_QSPI_Generic::readMemory(uint32_t addr, uint8_t *data, uint32_t size)
{
	return QSPI0.readMemory(addr, data, size);
}

/**************************************************************************/
/*! 
    @brief write a chunk of memory to the device
    @param addr the address to write to
    @param data a pointer to the data to be written
    @param size the number of bytes to write
    @returns true
*/
/**************************************************************************/
bool Adafruit_QSPI_Generic::writeMemory(uint32_t addr, uint8_t *data, uint32_t size)
{
	return QSPI0.writeMemory(addr, data, size);
}

/**************************************************************************/
/*! 
    @brief read a chunk of memory from the device
    @param address the address to read from
    @param buffer the pointer to where the read data will be stored
    @param len the number of bytes to read
    @returns true
*/
/**************************************************************************/
uint32_t Adafruit_QSPI_Generic::readBuffer (uint32_t address, uint8_t *buffer, uint32_t len)
{
	readMemory(address, buffer, len);
	return len;	
}


/**************************************************************************/
/*! 
    @brief erase a sector of flash
    @param sectorNumber the sector number to erase. The address erased will be (sectorNumber * W25Q16BV_SECTORSIZE)
    @returns true
*/
/**************************************************************************/
bool Adafruit_QSPI_Generic::eraseSector (uint32_t sectorNumber)
{
	uint32_t address = sectorNumber * W25Q16BV_SECTORSIZE;
	QSPI0.eraseSector(address);
	return true;
}


/**************************************************************************/
/*! 
    @brief write a chunk of memory to the device
    @param address the address to write to
    @param buffer a pointer to the data to be written
    @param len the number of bytes to write
    @returns len
*/
/**************************************************************************/
uint32_t Adafruit_QSPI_Generic::writeBuffer (uint32_t address, uint8_t *buffer, uint32_t len)
{
	writeMemory(address, buffer, len);
	return len;
}
