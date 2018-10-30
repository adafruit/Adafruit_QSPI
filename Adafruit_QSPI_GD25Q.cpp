/*
 * Adafruit_QSPI_GD25Q.cpp
 *
 *  Created on: Jan 2, 2018
 *      Author: deanm
 */


#include "Adafruit_QSPI_GD25Q.h"

enum {
	GD25Q_READ_STATUS1 = 0,
	GD25Q_READ_STATUS2,
	GD25Q_WRITE_ENABLE_VOLATILE_STATUS,
	GD25Q_READ_IDS,
	GD25Q_WRITE_STATUS2,
};

static const QSPIInstr cmdSetGD25Q[] = {
		//read status
		{ 0x05, false, QSPI_ADDRLEN_NONE, QSPI_OPCODE_LEN_NONE, QSPI_IO_FORMAT_SINGLE, (QSPI_OPTION_INSTREN | QSPI_INSTRFRAME_DATAEN), QSPI_READ, 0 },
		//read status2
		{ 0x35, false, QSPI_ADDRLEN_NONE, QSPI_OPCODE_LEN_NONE, QSPI_IO_FORMAT_SINGLE, (QSPI_OPTION_INSTREN | QSPI_INSTRFRAME_DATAEN), QSPI_READ, 0 },
		//write enable volatile status
		{ 0x50, false, QSPI_ADDRLEN_NONE, QSPI_OPCODE_LEN_NONE, QSPI_IO_FORMAT_SINGLE, (QSPI_OPTION_INSTREN), QSPI_READ, 0 },
		//read ids
		{ 0x9F, false, QSPI_ADDRLEN_NONE, QSPI_OPCODE_LEN_NONE, QSPI_IO_FORMAT_SINGLE, (QSPI_OPTION_INSTREN | QSPI_INSTRFRAME_DATAEN), QSPI_READ, 0 },
		//write status2
		{ 0x31, false, QSPI_ADDRLEN_NONE, QSPI_OPCODE_LEN_NONE, QSPI_IO_FORMAT_SINGLE, (QSPI_OPTION_INSTREN | QSPI_OPTION_DATAEN), QSPI_WRITE, 0 },
};

/**************************************************************************/
/*! 
    @brief additional commands specific to the GD25Q flash device
    @returns true if the device was identified and could be properly set up. False otherwise.
*/
/**************************************************************************/
bool Adafruit_QSPI_GD25Q::begin()
{
	Adafruit_QSPI_Generic::begin();

	// read device ids
	uint8_t ids[3];
	QSPI0.runInstruction(&cmdSetGD25Q[GD25Q_READ_IDS], 0, NULL, ids, 3);
	if (ids[0] != 0xC8)
		 return false;

	switch(ids[2]){
		case(0x17):
			setFlashType(SPIFLASHTYPE_W25Q64);
			break;
		case(0x15):
			setFlashType(SPIFLASHTYPE_W25Q16BV);
			break;
		default:
			return false;
	}
	
	_status.reg = 0;
	//_status.bit.HPF = 1; //enable high performance mode
	_status.bit.QE = 1; //enable quad io

	writeStatus();

	QSPI0.runInstruction(&cmdSetGD25Q[GD25Q_READ_STATUS1], 0, NULL, ((uint8_t *)&_status.reg), 1);
	while(_status.bit.WIP){
		delay(1);
		QSPI0.runInstruction(&cmdSetGD25Q[GD25Q_READ_STATUS1], 0, NULL, ((uint8_t *)&_status.reg), 1);
	}

	QSPI0.runInstruction(&cmdSetGD25Q[GD25Q_READ_STATUS2], 0, NULL, ((uint8_t *)&_status.reg) + 1, 1);
	return (_status.bit.QE);
}

void Adafruit_QSPI_GD25Q::writeStatus()
{
	uint8_t c = (uint8_t)((_status.reg >> 8) & 0xFF);

	QSPI0.runInstruction(&cmdSetGeneric[ADAFRUIT_QSPI_GENERIC_CMD_WRITE_ENABLE]);

	QSPI0.runInstruction(&cmdSetGD25Q[GD25Q_WRITE_STATUS2], 0, &c, NULL, 1);
}

