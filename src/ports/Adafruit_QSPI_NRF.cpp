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

#ifdef NRF52840_XXAA

#include "Adafruit_QSPI.h"

Adafruit_QSPI_NRF QSPI0;

void Adafruit_QSPI_NRF::begin(void)
{

}

void Adafruit_QSPI_NRF::setClockDivider (uint8_t uc_div)
{

}
void Adafruit_QSPI_NRF::setAddressLength (uint8_t width_bit)
{
}

void Adafruit_QSPI_NRF::runInstruction (const QSPIInstr *instr, uint32_t addr, uint8_t *txData, uint8_t *rxData, uint32_t size,
                     bool invalidateCache = true)
{
}
void Adafruit_QSPI_NRF::eraseSector (uint32_t sectorAddr)
{
}
bool Adafruit_QSPI_NRF::readMemory (uint32_t addr, uint8_t *data, uint32_t size)
{
}
bool Adafruit_QSPI_NRF::writeMemory (uint32_t addr, uint8_t *data, uint32_t size)
{
}

#endif
