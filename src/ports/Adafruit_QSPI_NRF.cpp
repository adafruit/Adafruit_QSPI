/**
 * @file Adafruit_QSPI_NRF.cpp
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach for Adafruit Industries LLC
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
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
    .xip_offset   = 0,
    .pins = {
      .sck_pin    = g_ADigitalPinMap[sck],
      .csn_pin    = g_ADigitalPinMap[cs],
      .io0_pin    = g_ADigitalPinMap[io0],
      .io1_pin    = g_ADigitalPinMap[io1],
      .io2_pin    = g_ADigitalPinMap[io2],
      .io3_pin    = g_ADigitalPinMap[io3],
    },
    .prot_if = {
      .readoc     = NRF_QSPI_READOC_READ4O, // 0x6B read command
      .writeoc    = NRF_QSPI_WRITEOC_PP4O,  // 0x32 write command
      .addrmode   = NRF_QSPI_ADDRMODE_24BIT,
      .dpmconfig  = false
    },
    .phy_if  = {
      .sck_delay  = 10,
      .dpmen      = false,
      .spi_mode   = NRF_QSPI_MODE_0,
      .sck_freq   = NRF_QSPI_FREQ_32MDIV16, // start with low 2 Mhz speed
    },
    .irq_priority = 7
  };

  // No callback for blocking API
  nrfx_qspi_init(&qspi_cfg, NULL, NULL);
}

void Adafruit_QSPI_NRF::setClockDivider (uint8_t uc_div)
{
  // delay is set to one freq period
  uint8_t delay = 1;

  if ( uc_div ) delay = (1UL << (uc_div-1));

  NRF_QSPI->IFCONFIG1 &= ~(QSPI_IFCONFIG1_SCKFREQ_Msk | QSPI_IFCONFIG1_SCKDELAY_Msk);
  NRF_QSPI->IFCONFIG1 |= (uc_div << QSPI_IFCONFIG1_SCKFREQ_Pos) | (delay << QSPI_IFCONFIG1_SCKDELAY_Pos);
}

void Adafruit_QSPI_NRF::setClockSpeed(uint32_t clock_hz)
{
  // Start at 16 MHz and go down.
  // At 32 MHz nRF52840 doesn't work reliably !!!
  // clkdiv = 0 is 32 Mhz
  // clkdiv = 1 is 16 MHz, etc.
  uint8_t clkdiv = 1;
  while ( (32000000UL / (clkdiv + 1) > clock_hz) && (clkdiv < 16) ) {
    clkdiv++;
  }

  setClockDivider(clkdiv);
}

bool Adafruit_QSPI_NRF::runCommand(uint8_t command)
{
  nrf_qspi_cinstr_conf_t cinstr_cfg =
  {
    .opcode    = command,
    .length    = NRF_QSPI_CINSTR_LEN_1B,
    .io2_level = true,
    .io3_level = true,
    .wipwait   = false,
    .wren      = false
  };

  return nrfx_qspi_cinstr_xfer(&cinstr_cfg, NULL, NULL) == NRFX_SUCCESS;
}

bool Adafruit_QSPI_NRF::readCommand(uint8_t command, uint8_t* response, uint32_t len)
{
  nrf_qspi_cinstr_conf_t cinstr_cfg =
  {
    .opcode    = command,
    .length    = (nrf_qspi_cinstr_len_t) (len + 1),
    .io2_level = true,
    .io3_level = true,
    .wipwait   = false,
    .wren      = false
  };
  return nrfx_qspi_cinstr_xfer(&cinstr_cfg, NULL, response) == NRFX_SUCCESS;
}

bool Adafruit_QSPI_NRF::writeCommand(uint8_t command, uint8_t const* data, uint32_t len)
{
  nrf_qspi_cinstr_conf_t cinstr_cfg =
  {
      .opcode    = command,
      .length    =(nrf_qspi_cinstr_len_t) (len + 1),
      .io2_level = true,
      .io3_level = true,
      .wipwait   = false,
      .wren      = false // We do this manually.
  };
  return nrfx_qspi_cinstr_xfer(&cinstr_cfg, data, NULL) == NRFX_SUCCESS;
}

bool Adafruit_QSPI_NRF::eraseSector (uint32_t sectorAddr)
{
  return NRFX_SUCCESS == nrfx_qspi_erase(NRF_QSPI_ERASE_LEN_4KB, sectorAddr);
}

bool Adafruit_QSPI_NRF::readMemory (uint32_t addr, uint8_t *data, uint32_t len)
{
  return NRFX_SUCCESS == nrfx_qspi_read(data, len, addr);
}

bool Adafruit_QSPI_NRF::writeMemory (uint32_t addr, uint8_t *data, uint32_t len)
{
  return NRFX_SUCCESS == nrfx_qspi_write(data, len, addr);
}

#endif
