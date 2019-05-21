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
		{ 0x05, false, true },
		//read status2
		{ 0x35, false, true },
		//write enable volatile status
		{ 0x50, false, false },
		//read ids
		{ 0x9F, false, true },
		//write status2
		{ 0x31, false, true },
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
//	QSPI0.runInstruction(&cmdSetGD25Q[GD25Q_READ_IDS], 0, NULL, ids, 3);
	QSPI0.readCommand(QSPI_CMD_READ_JEDEC_ID, ids, 3);
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

	// wait while busy
	while(readStatus() & 0x01) delay(1);

//	QSPI0.runInstruction(&cmdSetGD25Q[GD25Q_READ_STATUS2], 0, NULL, ((uint8_t *)&_status.reg) + 1, 1);
	QSPI0.readCommand(0x35, ((uint8_t *)&_status.reg) + 1, 1);
	return (_status.bit.QE);
}

void Adafruit_QSPI_GD25Q::writeStatus()
{
	uint8_t c = (uint8_t)((_status.reg >> 8) & 0xFF);

//	QSPI0.runInstruction(&cmdSetGeneric[ADAFRUIT_QSPI_GENERIC_CMD_WRITE_ENABLE]);

//	QSPI0.runInstruction(&cmdSetGD25Q[GD25Q_WRITE_STATUS2], 0, &c, NULL, 1);

	QSPI0.runCommand(QSPI_CMD_ENABLE_WRITE);
	QSPI0.writeCommand(0x31, &c, 1);
}

