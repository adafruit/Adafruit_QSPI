/*!
 * @file Adafruit_QSPI.h
 *
 * This is part of Adafruit's QSPI Peripheral driver for the Arduino platform.  It is
 * designed specifically to work with the Adafruit M4 Express development boards.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Written by Dean Miller, Ha Thach for Adafruit Industries.
 *
 * BSD license, all text here must be included in any redistribution.
 *
 */

#ifndef ADAFRUIT_QSPI_H_
#define ADAFRUIT_QSPI_H_

#include <Arduino.h>

// Default Read & Write command for read/write flash memory
enum
{
  QSPI_CMD_READ_4OUT  = 0x6B, // 1 line address, 4 line data
  QSPI_CMD_WRITE_4OUT = 0x32, // 1 line address, 4 line data

  QSPI_CMD_READ_JEDEC_ID = 0x9f,
//  QSPI_CMD_READ_DEVICE_ID = 0xAB,
  QSPI_CMD_READ_MANFACTURER_ID = 0x90,

  QSPI_CMD_PAGE_PROGRAM = 0x02,
  QSPI_CMD_READ_STATUS = 0x05,
  QSPI_CMD_WRITE_STATUS = 0x01,
  QSPI_CMD_ENABLE_WRITE = 0x06,
  QSPI_CMD_ERASE_SECTOR = 0x020,
  QSPI_CMD_ERASE_CHIP   = 0xC7,
};

/**************************************************************************/
/*!
    @brief  QSPI instruction struct
*/
/**************************************************************************/
typedef struct {
	uint8_t instruction;       ///< the instruction byte
	bool has_addr;
	bool has_data;
} QSPIInstr;


// Adafruit_QSPI is abstract class provide common API for all ports
class Adafruit_QSPI
{
  public:
    virtual void begin(int sck, int cs, int io0, int io1, int io2, int io3);

    virtual void setClockDivider(uint8_t uc_div) = 0;
    virtual void setClockSpeed(uint32_t clock_hz) = 0;

    virtual bool runCommand(uint8_t command) = 0;
    virtual bool readCommand(uint8_t comamnd, uint8_t* response, uint32_t len) = 0;
    virtual bool writeCommand(uint8_t command, uint8_t const* data, uint32_t len) = 0;

    virtual void eraseSector(uint32_t sectorAddr);
    virtual bool readMemory(uint32_t addr, uint8_t *data, uint32_t size) = 0;
    virtual bool writeMemory(uint32_t addr, uint8_t *data, uint32_t size) = 0;
};

#if defined __SAMD51__
  #include "ports/Adafruit_QSPI_SAMD.h"
#elif defined NRF52840_XXAA
  #include "ports/Adafruit_QSPI_NRF.h"
#else
  #error "MCU is not supported"
#endif

#endif /* ADAFRUIT_QSPI_H_ */
