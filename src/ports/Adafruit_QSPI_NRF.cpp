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
#include "nrfx_qspi.h"

Adafruit_QSPI_NRF QSPI0;

Adafruit_QSPI_NRF::Adafruit_QSPI_NRF(void)
{

}

void Adafruit_QSPI_NRF::begin(void)
{
  // Init QSPI flash
  nrfx_qspi_config_t qspi_cfg = {
    .xip_offset = 0,
    .pins = {
      .sck_pin = g_ADigitalPinMap[PIN_QSPI_SCK],
      .csn_pin = g_ADigitalPinMap[PIN_QSPI_CS],
      .io0_pin = g_ADigitalPinMap[PIN_QSPI_IO0],
      .io1_pin = g_ADigitalPinMap[PIN_QSPI_IO1],
      .io2_pin = g_ADigitalPinMap[PIN_QSPI_IO2],
      .io3_pin = g_ADigitalPinMap[PIN_QSPI_IO3],
    },
    .prot_if = {
      .readoc = NRF_QSPI_READOC_READ4IO,
      .writeoc = NRF_QSPI_WRITEOC_PP4O,
      .addrmode = NRF_QSPI_ADDRMODE_24BIT,
      .dpmconfig = false
    },
    .phy_if = {
      .sck_delay = 10,    // min time CS must stay high before going low again. in unit of 62.5 ns
      .dpmen = false,
      .spi_mode = NRF_QSPI_MODE_0,
      .sck_freq = NRF_QSPI_FREQ_32MDIV16, // start with low 2 Mhz speed
    },
    .irq_priority = 7
  };

  // No callback for blocking API
  nrfx_qspi_init(&qspi_cfg, NULL, NULL);
}

void Adafruit_QSPI_NRF::setClockDivider (uint8_t uc_div)
{
  // TODO implement
}
void Adafruit_QSPI_NRF::setAddressLength (uint8_t width_bit)
{
  // TODO implement
}

void Adafruit_QSPI_NRF::runInstruction (const QSPIInstr *instr, uint32_t addr, uint8_t *txData, uint8_t *rxData, uint32_t size,
                     bool invalidateCache)
{
  nrf_qspi_cinstr_conf_t cfg =
  {
    .opcode = instr->instruction,
    .length = NRF_QSPI_CINSTR_LEN_1B,
    .io2_level = true,
    .io3_level = true,
    .wipwait = false,
    .wren = false
  };

  // not read/write memory up to 8 bytes data only
  uint8_t buffer[8] = { 0 };
  uint8_t len = 0;

  if (instr->has_addr)
  {
    memcpy(buffer, &addr, 3); // 24-bit address
    len += 3;
  }

  if (size)
  {
    if (txData) memcpy(buffer + cfg.length, txData, size);
    len += size;
  }

  len++; // including instruction code

  cfg.length = (nrf_qspi_cinstr_len_t) len;
  nrfx_qspi_cinstr_xfer(&cfg, buffer, buffer);

  if (rxData && size)
  {
    memcpy(rxData, buffer+len-size-1, size);
  }
}

void Adafruit_QSPI_NRF::eraseSector (uint32_t sectorAddr)
{
  // TODO implement
}

bool Adafruit_QSPI_NRF::readMemory (uint32_t addr, uint8_t *data, uint32_t size)
{
  // TODO implement
}

bool Adafruit_QSPI_NRF::writeMemory (uint32_t addr, uint8_t *data, uint32_t size)
{
  // TODO implement
}

#endif
