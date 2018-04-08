/*
 * Adafruit_S25FL1.h
 *
 *  Created on: Jan 2, 2018
 *      Author: deanm
 */

#ifndef ADAFRUIT_QSPI_S25FL1_H_
#define ADAFRUIT_QSPI_S25FL1_H_

#include "Adafruit_QSPI_Generic.h"

class Adafruit_QSPI_S25FL1 : public Adafruit_QSPI_Generic {

public:
	Adafruit_QSPI_S25FL1() : Adafruit_QSPI_Generic() {}
	~Adafruit_QSPI_S25FL1() {}

	bool begin();

	void writeStatus(byte s1, byte s2, byte s3);
};



#endif /* ADAFRUIT_QSPI_S25FL1_H_ */
