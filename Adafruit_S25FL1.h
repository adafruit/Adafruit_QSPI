/*
 * Adafruit_S25FL1.h
 *
 *  Created on: Jan 2, 2018
 *      Author: deanm
 */

#ifndef ADAFRUIT_ZEROQSPI_ADAFRUIT_S25FL1_H_
#define ADAFRUIT_ZEROQSPI_ADAFRUIT_S25FL1_H_

#include "Flash_Generic.h"

class Adafruit_S25FL1 : public Adafruit_Flash_Generic {

public:
	Adafruit_S25FL1() : Adafruit_Flash_Generic() {}
	~Adafruit_S25FL1() {}

	bool begin();

	void writeStatus(byte s1, byte s2, byte s3);
};



#endif /* ADAFRUIT_ZEROQSPI_ADAFRUIT_S25FL1_H_ */
