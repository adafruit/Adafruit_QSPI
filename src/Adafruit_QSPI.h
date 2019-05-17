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

/**************************************************************************/
/*!
    @brief  QSPI instruction struct
*/
/**************************************************************************/
typedef struct {
	uint8_t instruction;       ///< the instruction byte
	uint8_t dummylen;          ///< the number of dummy cycles that should preceed data transfer

	bool has_addr;
	bool has_data;
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
