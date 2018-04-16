/*
 * Adafruit_QSPI_GD25Q.cpp
 *
 *  Created on: Jan 2, 2018
 *      Author: deanm
 */


#include "Adafruit_QSPI_GD25Q.h"

enum {
	GD25Q_READ_STATUS_2 = 0,
	GD25Q_READ_STATUS_3,
	GD25Q_WRITE_ENABLE_STATUS,
};

static const QSPIInstr cmdSetGD25Q[] = {
		//read status 2
		{ 0x35, false, QSPI_ADDRLEN_24_BITS, QSPI_OPCODE_LEN_NONE, QSPI_IO_FORMAT_SINGLE, (QSPI_OPTION_INSTREN | QSPI_OPTION_DATAEN | QSPI_OPTION_ADDREN), QSPI_READ, 0 },
		//read status 3
		{ 0x33, false, QSPI_ADDRLEN_24_BITS, QSPI_OPCODE_LEN_NONE, QSPI_IO_FORMAT_SINGLE, (QSPI_OPTION_INSTREN | QSPI_OPTION_DATAEN | QSPI_OPTION_ADDREN), QSPI_READ, 0 },
		//write enable status
		{ 0x50, false, QSPI_ADDRLEN_NONE, QSPI_OPCODE_LEN_NONE, QSPI_IO_FORMAT_SINGLE, (QSPI_OPTION_INSTREN), QSPI_READ, 0 },
};

bool Adafruit_QSPI_GD25Q::begin()
{
	Adafruit_QSPI_Generic::begin();

	//enable quad
	writeStatus(0x22, 0x00);

	if (readDeviceID() != 0x14) return false;
	if (readManufacturerID() != 0xC8) return false;

	uint8_t s2;
	QSPI0.runInstruction(&cmdSetGD25Q[GD25Q_READ_STATUS_2], 0, NULL, &s2, 1);
Serial.println(s2,HEX);
	return (s2 == 0x02);
}

void Adafruit_QSPI_GD25Q::writeStatus(byte s1, byte s2, byte s3)
{
	uint8_t c[] = {s1, s2, s3};

	uint8_t dummy;
	QSPI0.runInstruction(&cmdSetGeneric[ADAFRUIT_QSPI_GENERIC_CMD_WRITE_ENABLE], 0, NULL, &dummy, 1);
	QSPI0.runInstruction(&cmdSetGD25Q[GD25Q_WRITE_ENABLE_STATUS], 0, NULL, &dummy, 1);

	QSPI0.runInstruction(&cmdSetGeneric[ADAFRUIT_QSPI_GENERIC_CMD_WRITE_STATUS], 0, c, NULL, 3);
}

