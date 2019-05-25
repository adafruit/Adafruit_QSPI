/* Test QSPI read and write functionality. Erase chip, write sequential bytes, verify.
 */
#include "Adafruit_QSPI_Flash.h"
#include <Adafruit_NeoPixel.h>

Adafruit_QSPI_Flash flash;

#if defined(ADAFRUIT_FEATHER_M4_EXPRESS)
  #define NEOPIXPIN     8
#elif defined(ADAFRUIT_TRELLIS_M4_EXPRESS)
  #define NEOPIXPIN     10
#elif defined(ADAFRUIT_GRAND_CENTRAL_M4)
  #define NEOPIXPIN     88
#elif defined(ADAFRUIT_ITSYBITSY_M4_EXPRESS)
  #include <Adafruit_DotStar.h>
  Adafruit_DotStar strip = Adafruit_DotStar(1, 8, 6, DOTSTAR_BGR);
#elif defined(ADAFRUIT_PYGAMER_M4_EXPRESS) || defined(ADAFRUIT_PYBADGE_M4_EXPRESS)
  #define NEOPIXPIN     8
#else
  #define NEOPIXPIN     40
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
