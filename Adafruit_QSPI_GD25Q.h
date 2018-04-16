/*
 * Adafruit_GD25Q.h
 *
 *  Created on: Jan 2, 2018
 *      Author: deanm
 */

#ifndef ADAFRUIT_QSPI_GD25Q_H_
#define ADAFRUIT_QSPI_GD25Q_H_

#include "Adafruit_QSPI_Generic.h"

class Adafruit_QSPI_GD25Q : public Adafruit_QSPI_Generic {

public:
	Adafruit_QSPI_GD25Q() : Adafruit_QSPI_Generic() {}
	~Adafruit_QSPI_GD25Q() {}

	bool begin();

	void writeStatus(byte s1, byte s2, byte s3);
};



#endif /* ADAFRUIT_QSPI_GD25Q_H_ */
