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
		{ 0xAB, false, QSPI_ADDRLEN_24_BITS, QSPI_OPCODE_LEN_NONE, QSPI_IO_FORMAT_SINGLE, (QSPI_OPTION_INSTREN | QSPI_OPTION_DATAEN | QSPI_OPTION_ADDREN), QSPI_READ, 0 },
		//Manufacturer ID
		{ 0x90, false, QSPI_ADDRLEN_24_BITS, QSPI_OPCODE_LEN_NONE, QSPI_IO_FORMAT_SINGLE, (QSPI_OPTION_INSTREN | QSPI_OPTION_DATAEN | QSPI_OPTION_ADDREN), QSPI_READ, 0 },
		//read status register
		{ 0x05, false, QSPI_ADDRLEN_NONE, QSPI_OPCODE_LEN_NONE, QSPI_IO_FORMAT_SINGLE, (QSPI_OPTION_INSTREN | QSPI_OPTION_DATAEN), QSPI_READ, 0 },
		//write status
		{ 0x01, false, QSPI_ADDRLEN_NONE, QSPI_OPCODE_LEN_NONE, QSPI_IO_FORMAT_SINGLE, (QSPI_OPTION_INSTREN | QSPI_OPTION_DATAEN), QSPI_WRITE, 0 },
		//Write Enable
		{ 0x06, false, QSPI_ADDRLEN_NONE, QSPI_OPCODE_LEN_NONE, QSPI_IO_FORMAT_SINGLE, (QSPI_OPTION_INSTREN), QSPI_READ, 0 },
		//Write Disable
		{ 0x04, false, QSPI_ADDRLEN_NONE, QSPI_OPCODE_LEN_NONE, QSPI_IO_FORMAT_SINGLE, (QSPI_OPTION_INSTREN), QSPI_READ, 0 },
		//Chip Erase
		{ 0xC7, false, QSPI_ADDRLEN_NONE, QSPI_OPCODE_LEN_NONE, QSPI_IO_FORMAT_SINGLE, (QSPI_OPTION_INSTREN), QSPI_READ, 0 },
		// Sector Erase
		{ 0x20, false, QSPI_ADDRLEN_24_BITS, QSPI_OPCODE_LEN_NONE, QSPI_IO_FORMAT_SINGLE, (QSPI_OPTION_INSTREN | QSPI_OPTION_ADDREN), QSPI_READ, 0 },
		// Block Erase 64KB
		{ 0xD8, false, QSPI_ADDRLEN_24_BITS, QSPI_OPCODE_LEN_NONE, QSPI_IO_FORMAT_SINGLE, (QSPI_OPTION_INSTREN | QSPI_OPTION_ADDREN), QSPI_READ, 0 },
		//Page Program
		{ 0x02, false, QSPI_ADDRLEN_24_BITS, QSPI_OPCODE_LEN_NONE, QSPI_IO_FORMAT_SINGLE, (QSPI_OPTION_INSTREN | QSPI_OPTION_DATAEN | QSPI_OPTION_ADDREN), QSPI_WRITE_MEMORY, 0 },
		//Quad Read
		{ 0x6B, true, QSPI_ADDRLEN_24_BITS, QSPI_OPCODE_LEN_NONE, QSPI_IO_FORMAT_SINGLE_QUAD_DATA, (QSPI_OPTION_INSTREN | QSPI_OPTION_DATAEN | QSPI_OPTION_ADDREN), QSPI_READ_MEMORY, 8 },
		//Read ID
		{ 0x9F, false, QSPI_ADDRLEN_NONE, QSPI_OPCODE_LEN_NONE, QSPI_IO_FORMAT_SINGLE, (QSPI_OPTION_INSTREN | QSPI_OPTION_DATAEN), QSPI_READ, 0 },
};

/**************************************************************************/
/*! 
    @brief begin the default QSPI peripheral
    @returns true
*/
/**************************************************************************/
bool Adafruit_QSPI_Generic::begin(){
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
    @brief read the device id
    @returns the read device ID
*/
/**************************************************************************/
byte Adafruit_QSPI_Generic::readDeviceID()
{
	byte r;
	QSPI0.runInstruction(&cmdSetGeneric[ADAFRUIT_QSPI_GENERIC_CMD_DEVID], 0, NULL, &r, 1);
	return r;
}

/**************************************************************************/
/*! 
    @brief read the manufacturer ID
    @returns the read manufacturer ID
*/
/**************************************************************************/
byte Adafruit_QSPI_Generic::readManufacturerID()
{
	byte r;
	QSPI0.runInstruction(&cmdSetGeneric[ADAFRUIT_QSPI_GENERIC_CMD_MFGID], 0, NULL, &r, 1);
	return r;
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
	*deviceID = readDeviceID();
	*manufID = readManufacturerID();
}

/**************************************************************************/
/*! 
    @brief read JEDEC ID information from the device
	@returns the read id as a uint32
*/
/**************************************************************************/
uint32_t Adafruit_QSPI_Generic::GetJEDECID (void)
{
	uint32_t id = 0;
	QSPI0.runInstruction(&cmdSetGeneric[ADAFRUIT_QSPI_GENERIC_CMD_RDID], 0, NULL, (uint8_t *)&id, 3);
	return ((id >> 16) & 0xFF) | (id & 0x00FF00) | ((id & 0xFF) << 16);
}

/**************************************************************************/
/*! 
    @brief read the generic status register.
    @returns the status register reading
*/
/**************************************************************************/
byte Adafruit_QSPI_Generic::readStatus()
{
	byte r;
	QSPI0.runInstruction(&cmdSetGeneric[ADAFRUIT_QSPI_GENERIC_READ_STATUS], 0, NULL, &r, 1);
	return r;
}

/**************************************************************************/
/*! 
    @brief perform a chip erase. All data on the device will be erased.
*/
/**************************************************************************/
void Adafruit_QSPI_Generic::chipErase()
{
	byte r;
	QSPI0.runInstruction(&cmdSetGeneric[ADAFRUIT_QSPI_GENERIC_CMD_WRITE_ENABLE], 0, NULL, &r, 1);

	QSPI0.runInstruction(&cmdSetGeneric[ADAFRUIT_QSPI_GENERIC_CMD_CHIP_ERASE], 0, NULL, &r, 1);

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
	byte r;
	QSPI0.runInstruction(&cmdSetGeneric[ADAFRUIT_QSPI_GENERIC_CMD_WRITE_ENABLE], 0, NULL, &r, 1);

	QSPI0.runInstruction(&cmdSetGeneric[ADAFRUIT_QSPI_GENERIC_CMD_BLOCK64K_ERASE], blocknum*W25Q16BV_BLOCKSIZE, NULL, &r, 1);

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
byte Adafruit_QSPI_Generic::read8(uint32_t addr)
{
	byte ret;
	QSPI0.runInstruction(&cmdSetGeneric[ADAFRUIT_QSPI_GENERIC_CMD_QUAD_READ], addr, NULL, &ret, 1);
	return ret;
}

/**************************************************************************/
/*! 
    @brief read a chunk of memory from the device
    @param addr the address to read from
    @param data the pointer to where the read data will be stored
    @param size the number of bytes to read
	@param invalidateCache manual cache management. Only use this parameter if you know what you're doing. Defaults to true.
    @returns true
*/
/**************************************************************************/
bool Adafruit_QSPI_Generic::readMemory(uint32_t addr, uint8_t *data, uint32_t size, bool invalidateCache)
{
	QSPI0.runInstruction(&cmdSetGeneric[ADAFRUIT_QSPI_GENERIC_CMD_QUAD_READ], addr, NULL, data, size, invalidateCache);
	return true;
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
	byte r;
	uint16_t toWrite = 0;

	//write one page at a time
	while(size){

		QSPI0.runInstruction(&cmdSetGeneric[ADAFRUIT_QSPI_GENERIC_CMD_WRITE_ENABLE], 0, NULL, &r, 1);

		if(size > 256) toWrite = 256;
		else toWrite = size;
		size -= toWrite;

		QSPI0.runInstruction(&cmdSetGeneric[ADAFRUIT_QSPI_GENERIC_CMD_PAGE_PROGRAM], addr, data, NULL, toWrite);

		data += toWrite;
		addr += toWrite;

		while(readStatus() & ADAFRUIT_QSPI_GENERIC_STATUS_BUSY);
	}

	return true;
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
	byte r;
	QSPI0.runInstruction(&cmdSetGeneric[ADAFRUIT_QSPI_GENERIC_CMD_WRITE_ENABLE], 0, NULL, &r, 1);

	QSPI0.runInstruction(&cmdSetGeneric[ADAFRUIT_QSPI_GENERIC_CMD_SECTOR_ERASE], address, NULL, &r, 1);

	//wait for busy
	while(readStatus() & ADAFRUIT_QSPI_GENERIC_STATUS_BUSY);

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
