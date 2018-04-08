/*
 * Flash_Generic.cpp
 *
 *  Created on: Jan 2, 2018
 *      Author: deanm
 */

#include "Adafruit_QSPI_Generic.h"

#define ADAFRUIT_QSPI_FLASH_GENERIC_STATUS_BUSY 0x01

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
		//Page Program
		{ 0x02, false, QSPI_ADDRLEN_24_BITS, QSPI_OPCODE_LEN_NONE, QSPI_IO_FORMAT_SINGLE, (QSPI_OPTION_INSTREN | QSPI_OPTION_DATAEN | QSPI_OPTION_ADDREN), QSPI_WRITE_MEMORY, 0 },
		//Quad Read
		{ 0x6B, true, QSPI_ADDRLEN_24_BITS, QSPI_OPCODE_LEN_NONE, QSPI_IO_FORMAT_SINGLE_QUAD_DATA, (QSPI_OPTION_INSTREN | QSPI_OPTION_DATAEN | QSPI_OPTION_ADDREN), QSPI_READ_MEMORY, 8 },
};

bool Adafruit_QSPI_Generic::begin(){
	QSPI0.begin();
	return true;
}

byte Adafruit_QSPI_Generic::readID()
{
	byte r;
	QSPI0.runInstruction(&cmdSetGeneric[FLASH_GENERIC_CMD_DEVID], 0, NULL, &r, 1);
	return r;
}

byte Adafruit_QSPI_Generic::readStatus()
{
	byte r;
	QSPI0.runInstruction(&cmdSetGeneric[FLASH_GENERIC_READ_STATUS], 0, NULL, &r, 1);
	return r;
}

void Adafruit_QSPI_Generic::chipErase()
{
	byte r;
	QSPI0.runInstruction(&cmdSetGeneric[FLASH_GENERIC_CMD_WRITE_ENABLE], 0, NULL, &r, 1);

	QSPI0.runInstruction(&cmdSetGeneric[FLASH_GENERIC_CMD_CHIP_ERASE], 0, NULL, &r, 1);

	//wait for busy
	while(readStatus() & FLASH_GENERIC_STATUS_BUSY);
}

byte Adafruit_QSPI_Generic::read8(uint32_t addr)
{
	byte ret;
	QSPI0.runInstruction(&cmdSetGeneric[FLASH_GENERIC_CMD_QUAD_READ], addr, NULL, &ret, 1);
	return ret;
}

bool Adafruit_QSPI_Generic::readMemory(uint32_t addr, uint8_t *data, uint32_t size)
{
	QSPI0.runInstruction(&cmdSetGeneric[FLASH_GENERIC_CMD_QUAD_READ], addr, NULL, data, size);
	return true;
}

bool Adafruit_QSPI_Generic::writeMemory(uint32_t addr, uint8_t *data, uint32_t size)
{
	byte r;
	uint16_t toWrite = 0;

	//write one page at a time
	while(size){

		QSPI0.runInstruction(&cmdSetGeneric[FLASH_GENERIC_CMD_WRITE_ENABLE], 0, NULL, &r, 1);

		if(size > 256) toWrite = 256;
		else toWrite = size;
		size -= toWrite;

		QSPI0.runInstruction(&cmdSetGeneric[FLASH_GENERIC_CMD_PAGE_PROGRAM], addr, data, NULL, toWrite);

		data += toWrite;
		addr += toWrite;

		while(readStatus() & FLASH_GENERIC_STATUS_BUSY);
	}

	return true;
}
