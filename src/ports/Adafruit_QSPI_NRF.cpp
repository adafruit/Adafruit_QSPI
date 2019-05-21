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

void Adafruit_QSPI_NRF::begin(int sck, int cs, int io0, int io1, int io2, int io3)
{
  // Init QSPI flash
  nrfx_qspi_config_t qspi_cfg = {
    .xip_offset = 0,
    .pins = {
      .sck_pin = g_ADigitalPinMap[sck],
      .csn_pin = g_ADigitalPinMap[cs],
      .io0_pin = g_ADigitalPinMap[io0],
      .io1_pin = g_ADigitalPinMap[io1],
      .io2_pin = g_ADigitalPinMap[io2],
      .io3_pin = g_ADigitalPinMap[io3],
    },
    .prot_if = {
      .readoc = NRF_QSPI_READOC_READ4O, // 0x6B read command
      .writeoc = NRF_QSPI_WRITEOC_PP4O, // 0x32 write command
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
  NRF_QSPI->IFCONFIG1 &= ~QSPI_IFCONFIG1_SCKFREQ_Msk;
  NRF_QSPI->IFCONFIG1 |=  uc_div << QSPI_IFCONFIG1_SCKFREQ_Pos;
}

void Adafruit_QSPI_NRF::setClockSpeed(uint32_t clock_hz)
{
  uint8_t sckfreq = 0;
  while ( (32000000UL / (sckfreq + 1) > clock_hz) && (sckfreq < 16) ) {
    sckfreq++;
  }

  NRF_QSPI->IFCONFIG1 &= ~QSPI_IFCONFIG1_SCKFREQ_Msk;
  NRF_QSPI->IFCONFIG1 |=  sckfreq << QSPI_IFCONFIG1_SCKFREQ_Pos;
}

bool Adafruit_QSPI_NRF::runCommand(uint8_t command)
{
  nrf_qspi_cinstr_conf_t cinstr_cfg =
  {
    .opcode = command,
    .length = NRF_QSPI_CINSTR_LEN_1B,
    .io2_level = true,
    .io3_level = true,
    .wipwait = false,
    .wren = false
  };

  return nrfx_qspi_cinstr_xfer(&cinstr_cfg, NULL, NULL) == NRFX_SUCCESS;
}

bool Adafruit_QSPI_NRF::readCommand(uint8_t command, uint8_t* response, uint32_t len)
{
  nrf_qspi_cinstr_conf_t cinstr_cfg =
  {
    .opcode = command,
    .length = (nrf_qspi_cinstr_len_t) (len + 1),
    .io2_level = true,
    .io3_level = true,
    .wipwait = false,
    .wren = false
  };
  return nrfx_qspi_cinstr_xfer(&cinstr_cfg, NULL, response) == NRFX_SUCCESS;
}

bool Adafruit_QSPI_NRF::writeCommand(uint8_t command, uint8_t const* data, uint32_t len)
{
  nrf_qspi_cinstr_conf_t cinstr_cfg =
  {
      .opcode = command,
      .length =(nrf_qspi_cinstr_len_t) (len + 1),
      .io2_level = true,
      .io3_level = true,
      .wipwait = false,
      .wren = false // We do this manually.
  };
  return nrfx_qspi_cinstr_xfer(&cinstr_cfg, data, NULL) == NRFX_SUCCESS;
}

void Adafruit_QSPI_NRF::eraseSector (uint32_t sectorAddr)
{
  nrfx_qspi_erase(NRF_QSPI_ERASE_LEN_4KB, sectorAddr);
}

bool Adafruit_QSPI_NRF::readMemory (uint32_t addr, uint8_t *data, uint32_t size)
{
  return NRFX_SUCCESS == nrfx_qspi_read(data, size, addr);
}

bool Adafruit_QSPI_NRF::writeMemory (uint32_t addr, uint8_t *data, uint32_t size)
{
  return NRFX_SUCCESS == nrfx_qspi_write(data, size, addr);
}

#endif
