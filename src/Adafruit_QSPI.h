/*!
 * @file Adafruit_QSPI.h
 *
 * This is part of Adafruit's QSPI Peripheral driver for the Arduino platform.  It is
 * designed to work with the Adafruit development boards.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Written by Dean Miller, Ha Thach for Adafruit Industries.
 *
 * MIT license, all text here must be included in any redistribution.
 *
 */

#ifndef ADAFRUIT_QSPI_H_
#define ADAFRUIT_QSPI_H_

#include <Arduino.h>

/// QSPI command code
enum
{
  QSPI_CMD_QUAD_READ         = 0x6B, // 1 line address, 4 line data

  QSPI_CMD_READ_JEDEC_ID     = 0x9f,

  QSPI_CMD_PAGE_PROGRAM      = 0x02,
  QSPI_CMD_QUAD_PAGE_PROGRAM = 0x32, // 1 line address, 4 line data

  QSPI_CMD_READ_STATUS       = 0x05,
  QSPI_CMD_READ_STATUS2      = 0x35,

  QSPI_CMD_WRITE_STATUS      = 0x01,
  QSPI_CMD_WRITE_STATUS2     = 0x31,

  QSPI_CMD_ENABLE_RESET      = 0x66,
  QSPI_CMD_RESET             = 0x99,

  QSPI_CMD_WRITE_ENABLE      = 0x06,
  QSPI_CMD_WRITE_DISABLE     = 0x04,

  QSPI_CMD_ERASE_SECTOR      = 0x20,
  QSPI_CMD_ERASE_BLOCK       = 0xD8,
  QSPI_CMD_ERASE_CHIP        = 0xC7,
};

/// Abstract class provide common APIs for all mcu ports (e.g samd51, nrf52 etc ..)
class Adafruit_QSPI
{
  public:

    /**
     * Enable and configure QSPI peripheral clock and pins
     * @param pinSCK  SCK pin
     * @param pinCS   Chip select pin
     * @param pinIO0  DATA0 pin
     * @param pinIO1  DATA1 pin
     * @param pinIO2  DATA2 pin
     * @param pinIO3  DATA3 pin
     */
    virtual void begin(int pinSCK, int pinCS, int pinIO0, int pinIO1, int pinIO2, int pinIO3) = 0;

    /// Enable QSPI with default pins in variant.h
    void begin(void)
    {
      begin(PIN_QSPI_SCK, PIN_QSPI_CS, PIN_QSPI_IO0, PIN_QSPI_IO1, PIN_QSPI_IO2, PIN_QSPI_IO3);
    }

    /// Set clock speed in hertz
    /// @param clock_hz clock speed in hertz
    virtual void setClockSpeed(uint32_t clock_hz) = 0;

    /// Set clock  divider
    /// @param uc_div clock divider
    virtual void setClockDivider(uint8_t uc_div) = 0;

    /// Execute a single byte command e.g Reset, Write Enable
    /// @param command command code
    /// @return true if success
    virtual bool runCommand(uint8_t command) = 0;

    /// Execute a command with response data e.g Read Status, Read JEDEC
    /// @param command    command code
    /// @param response   buffer to hold data
    /// @param len        number of bytes to read
    /// @return true if success
    virtual bool readCommand(uint8_t command, uint8_t* response, uint32_t len) = 0;

    /// Execute a command with data e.g Write Status,
    /// @param command    command code
    /// @param data       writing data
    /// @param len        number of bytes to read
    /// @return true if success
    virtual bool writeCommand(uint8_t command, uint8_t const* data, uint32_t len) = 0;

    /// Erase external flash by address
    /// @param command  can be sector erase (0x20) or block erase 0xD8
    /// @param address  adddress to be erased
    /// @return true if success
    virtual bool eraseCommand(uint8_t command, uint32_t address) = 0;

    /// Read data from external flash contents. Typically it is implemented by quad read command 0x6B
    /// @param addr       address to read
    /// @param buffer     buffer to hold data
    /// @param len        number of byte to read
    /// @return true if success
    virtual bool readMemory(uint32_t addr, uint8_t *buffer, uint32_t len) = 0;

    /// Write data to external flash contents, flash sector must be previously erased by \ref eraseSector() first.
    /// Typically it uses quad write command 0x32
    /// @param addr       address to read
    /// @param data       writing data
    /// @param len        number of byte to read
    /// @return true if success
    virtual bool writeMemory(uint32_t addr, uint8_t *data, uint32_t len) = 0;
};

#if defined __SAMD51__
  #include "ports/Adafruit_QSPI_SAMD.h"
#elif defined NRF52840_XXAA
  #include "ports/Adafruit_QSPI_NRF.h"
#else
  #error "MCU is not supported"
#endif

#endif /* ADAFRUIT_QSPI_H_ */
