/*
 * Flash_Generic.cpp
 *
 *  Created on: Jan 2, 2018
 *      Author: deanm
 */

#include "Flash_Generic.h"

#define FLASH_GENERIC_STATUS_BUSY 0x01

enum {
	FLASH_GENERIC_CMD_DEVID = 0,
	FLASH_GENERIC_CMD_MFGID,
	FLASH_GENERIC_READ_STATUS,
	FLASH_GENERIC_CMD_WRITE_ENABLE,
	FLASH_GENERIC_CMD_WRITE_DISABLE,
	FLASH_GENERIC_CMD_CHIP_ERASE,
};

static const QSPIInstr cmdSet[] = {
		//Device ID
		{ 0xAB, false, QSPI_ADDRLEN_24_BITS, QSPI_OPCODE_LEN_NONE, QSPI_IO_FORMAT_SINGLE, (QSPI_OPTION_INSTREN | QSPI_OPTION_DATAEN | QSPI_OPTION_ADDREN), QSPI_READ, 0 },
		//Manufacturer ID
		{ 0x90, false, QSPI_ADDRLEN_24_BITS, QSPI_OPCODE_LEN_NONE, QSPI_IO_FORMAT_SINGLE, (QSPI_OPTION_INSTREN | QSPI_OPTION_DATAEN | QSPI_OPTION_ADDREN), QSPI_READ, 0 },
		//read status register
		{ 0x05, false, QSPI_ADDRLEN_NONE, QSPI_OPCODE_LEN_NONE, QSPI_IO_FORMAT_SINGLE, (QSPI_OPTION_INSTREN | QSPI_OPTION_DATAEN), QSPI_READ, 0 },
		//Write Enable
		{ 0x06, false, QSPI_ADDRLEN_NONE, QSPI_OPCODE_LEN_NONE, QSPI_IO_FORMAT_SINGLE, (QSPI_OPTION_INSTREN), QSPI_READ, 0 },
		//Write Disable
		{ 0x04, false, QSPI_ADDRLEN_NONE, QSPI_OPCODE_LEN_NONE, QSPI_IO_FORMAT_SINGLE, (QSPI_OPTION_INSTREN), QSPI_READ, 0 },
		//Chip Erase
		{ 0xC7, false, QSPI_ADDRLEN_NONE, QSPI_OPCODE_LEN_NONE, QSPI_IO_FORMAT_SINGLE, (QSPI_OPTION_INSTREN), QSPI_READ, 0 },
};

bool Adafruit_Flash_Generic::begin(){
	QSPI0.begin();
	return true;
}

byte Adafruit_Flash_Generic::readID()
{
	byte r;
	QSPI0.runInstruction(&cmdSet[FLASH_GENERIC_CMD_DEVID], 0, NULL, &r, 1);
	return r;
}

byte Adafruit_Flash_Generic::readStatus()
{
	byte r;
	QSPI0.runInstruction(&cmdSet[FLASH_GENERIC_READ_STATUS], 0, NULL, &r, 1);
	return r;
}

void Adafruit_Flash_Generic::chipErase()
{
	byte r;
	QSPI0.runInstruction(&cmdSet[FLASH_GENERIC_CMD_CHIP_ERASE], 0, NULL, &r, 1);

	//wait for busy
	while(readStatus() & FLASH_GENERIC_STATUS_BUSY);
}

byte Adafruit_Flash_Generic::read8(uint32_t addr)
{
	return 0;
}

bool Adafruit_Flash_Generic::write8(uint32_t addr, byte data)
{
	return true;
}

bool Adafruit_Flash_Generic::readMemory(uint32_t addr, uint32_t *data, uint32_t size)
{
	return true;
}

bool Adafruit_Flash_Generic::writeMemory(uint32_t addr, uint32_t *data, uint32_t size)
{
	return true;
}
