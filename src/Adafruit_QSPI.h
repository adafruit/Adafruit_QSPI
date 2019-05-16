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

#define QSPI_OPTION_NONE 0 ///< no option
#define QSPI_OPTION_INSTREN QSPI_INSTRFRAME_INSTREN ///< enable sending of instruction
#define QSPI_OPTION_ADDREN QSPI_INSTRFRAME_ADDREN ///< enable sending of address
#define QSPI_OPTION_OPCODEEN QSPI_INSTRFRAME_OPTCODEEN ///< enable sending of opcode
#define QSPI_OPTION_DATAEN QSPI_INSTRFRAME_DATAEN ///< enable sending of data

/**************************************************************************/
/*!
    @brief  the transfer format to use
*/
/**************************************************************************/
typedef enum {
	QSPI_IO_FORMAT_SINGLE = QSPI_INSTRFRAME_WIDTH_SINGLE_BIT_SPI_Val,
	QSPI_IO_FORMAT_SINGLE_DUAL_DATA,
	QSPI_IO_FORMAT_SINGLE_QUAD_DATA,
	QSPI_IO_FORMAT_SINGLE_INSTR_DUAL_ADDR_DUAL_DATA,
	QSPI_IO_FORMAT_SINGLE_INSTR_QUAD_ADDR_QUAD_DATA,
	QSPI_IO_FORMAT_DUAL,
	QSPI_IO_FORMAT_QUAD,
} QSPIIOFormat_t;

/**************************************************************************/
/*!
    @brief  the type of transfer
*/
/**************************************************************************/
typedef enum {
	QSPI_READ = QSPI_INSTRFRAME_TFRTYPE_READ_Val,
	QSPI_READ_MEMORY,
	QSPI_WRITE,
	QSPI_WRITE_MEMORY,
} QSPITransferType_t;

/**************************************************************************/
/*!
    @brief  QSPI instruction struct
*/
/**************************************************************************/
typedef struct {
	uint8_t instruction;       ///< the instruction byte
	QSPIIOFormat_t ioFormat;   ///< the data format to use
	uint8_t options;           ///< additional option flags
	QSPITransferType_t type;   ///< the transfer type
	uint8_t dummylen;          ///< the number of dummy cycles that should preceed data transfer
} QSPIInstr;


// Adafruit_QSPI is abstract class provide common API for all ports
class Adafruit_QSPI
{
  public:
    virtual void begin(void) = 0;
    virtual void setClockDivider(uint8_t uc_div) = 0;
    virtual void setAddressLength(uint8_t width_bit); // either 24 or 32 bit address

    virtual void runInstruction(const QSPIInstr *instr, uint32_t addr, uint8_t *txData, uint8_t *rxData, uint32_t size, bool invalidateCache=true) = 0;
    virtual void eraseSector(uint32_t sectorAddr);
    virtual bool readMemory(uint32_t addr, uint8_t *data, uint32_t size);
    virtual bool writeMemory(uint32_t addr, uint8_t *data, uint32_t size);
};

#if defined __SAMD51__
  #include "ports/Adafruit_QSPI_SAMD.h"
#elif defined NRF52840_XXAA
  #include "ports/Adafruit_QSPI_NRF.h"
#else
  #error "MCU is not supported"
#endif

#endif /* ADAFRUIT_QSPI_H_ */
