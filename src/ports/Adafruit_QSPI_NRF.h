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

    virtual bool eraseCommand(uint8_t command, uint32_t address);
    virtual bool readMemory(uint32_t addr, uint8_t *data, uint32_t len);
    virtual bool writeMemory(uint32_t addr, uint8_t *data, uint32_t len);
};

extern Adafruit_QSPI_NRF QSPI0;

#endif /* ADAFRUIT_QSPI_NRF_H_ */
