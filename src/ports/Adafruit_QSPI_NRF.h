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
    Adafruit_QSPI_NRF(void);

    virtual void begin(int sck, int cs, int io0, int io1, int io2, int io3);
    using Adafruit_QSPI::begin;

    void end(void);

    virtual void setClockDivider(uint8_t uc_div);
    virtual void setClockSpeed(uint32_t clock_hz);

    virtual bool runCommand(uint8_t command);
    virtual bool readCommand(uint8_t command, uint8_t* response, uint32_t len);
    virtual bool writeCommand(uint8_t command, uint8_t const* data, uint32_t len);

    virtual bool eraseSector(uint32_t sectorAddr);
    virtual bool readMemory(uint32_t addr, uint8_t *data, uint32_t size);
    virtual bool writeMemory(uint32_t addr, uint8_t *data, uint32_t size);
};

extern Adafruit_QSPI_NRF QSPI0;

#endif /* ADAFRUIT_QSPI_NRF_H_ */
