/* Test QSPI read and write functionality. Erase chip, write sequential bytes, verify.
 */
#include "Adafruit_QSPI_S25FL1.h"
#include "Adafruit_QSPI_GD25Q.h"
#include <Adafruit_NeoPixel.h>
#include <Adafruit_DotStar.h>

#if defined(ADAFRUIT_FEATHER_M4_EXPRESS)
  #define NEOPIXPIN     8
  Adafruit_QSPI_GD25Q flash;
#elif defined(ADAFRUIT_TRELLIS_M4_EXPRESS)
  #define NEOPIXPIN     10
  Adafruit_QSPI_GD25Q flash;
#elif defined(ADAFRUIT_GRAND_CENTRAL_M4)
  #define NEOPIXPIN     88
  Adafruit_QSPI_GD25Q flash;
#elif defined(ADAFRUIT_ITSYBITSY_M4_EXPRESS)
  Adafruit_DotStar strip = Adafruit_DotStar(1, 8, 6, DOTSTAR_BGR);
  Adafruit_QSPI_GD25Q flash;
#else
  #define NEOPIXPIN     40
  Adafruit_QSPI_GD25Q flash;
#endif

#if defined(NEOPIXPIN)
  Adafruit_NeoPixel strip = Adafruit_NeoPixel(32, NEOPIXPIN, NEO_GRB + NEO_KHZ800);
#endif

void setup(){
  Serial.begin(115200);
  strip.begin();
#if defined(NEOPIXPIN)
  strip.setBrightness(30);
#endif
  strip.setPixelColor(0, 20, 20, 20);
  strip.show();

  //while (!Serial);
  delay(500);

  Serial.println("Adafruit QSPI Eraser!");
  
  if (!flash.begin()){
    Serial.println("Could not find flash on QSPI bus!");
    strip.setPixelColor(0, 100, 0, 0);
    strip.show();
    while(1);
  }
  Serial.println("Found QSPI Flash");

  Serial.print("Erasing...");
  strip.setPixelColor(0, 100, 100, 0);
  strip.show();
  flash.chipErase();
  Serial.println("done!");
  strip.setPixelColor(0, 0, 100, 0);
  strip.show();
}

void loop(){
  //don't even do anything
}
