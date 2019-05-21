/* Test QSPI read and write functionality. Erase chip, write sequential bytes, verify.
 */

#include "Adafruit_QSPI_Flash.h"

Adafruit_QSPI_Flash flash;

#define PROGSIZE 512

uint8_t progData[PROGSIZE];
uint8_t readData[PROGSIZE];

void setup(){
  Serial.begin(115200);
  while(!Serial) {
    delay(10);
  }

  Serial.println("Adafruit QSPI test!");
  
  if (!flash.begin()){
    Serial.println("Could not find flash on QSPI bus!");
    while(1);
  }
  Serial.println("Found QSPI Flash");

  for (int i=0; i<PROGSIZE; i++){
    progData[i] = i;
  }

  Serial.print("Erasing...");
  flash.chipErase();
  Serial.println("done!");

  Serial.print("Writing...");
  flash.writeBuffer(0, progData, sizeof(progData));
  Serial.println("done!");

  Serial.print("Reading...");
  flash.readBuffer(0, readData, sizeof(readData));
  Serial.println("done!");

  for (int i=0; i<PROGSIZE; i++){
    if(readData[i] != progData[i]){
      Serial.print("ERROR: mismatch found at index ");
      Serial.println(i);
      while(1);
    }
  }

  Serial.println("Verify successful!");
}

void loop(){
  //don't even do anything
}
