/*!
 * @mainpage Adafruit QSPI library
 *
 * @section intro_sec Introduction
 *
 * This is documentation for Adafruit's library for interfacing with the QSPI
 * peripheral on specific microcontrollers. Currently only the SAMD51 is supported.
 * This is intened to be used with the Adafruit Metro M4 Express, Adafruit Feather M4 Express,
 * and other M4 Express development boards.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * @section author Author
 *
 * Written by Ha Thach for Adafruit Industries.
 *
 * @section license License
 *
 * BSD license, all text here must be included in any redistribution.
 *
 */

#ifndef ADAFRUIT_QSPI_NRF_H_
#define ADAFRUIT_QSPI_NRF_H_

class Adafruit_QSPI_NRF : Adafruit_QSPI
{
  public:
    virtual void begin(void);

    virtual void setClockDivider(uint8_t uc_div);
    virtual void setAddressLength(uint8_t width_bit); // either 24 or 32 bit address

    virtual void runInstruction(const QSPIInstr *instr, uint32_t addr, uint8_t *txData, uint8_t *rxData, uint32_t size, bool invalidateCache=true);
    virtual void eraseSector(uint32_t sectorAddr);
    virtual bool readMemory(uint32_t addr, uint8_t *data, uint32_t size);
    virtual bool writeMemory(uint32_t addr, uint8_t *data, uint32_t size);
};

extern Adafruit_QSPI_NRF QSPI0;

#endif /* ADAFRUIT_QSPI_NRF_H_ */
