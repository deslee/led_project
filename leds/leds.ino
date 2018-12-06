#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 150
#define DATA_PIN 5

// Define the array of leds
CRGB leds[NUM_LEDS];

byte fromSerial[4];
int byteIndex = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  resetLeds();
  FastLED.show();
}

void loop() {
  if (Serial.available()) {
    fromSerial[byteIndex] = Serial.read();
    byteIndex++;
    if (byteIndex == 4) {
      leds[fromSerial[0]].red = fromSerial[1];
      leds[fromSerial[0]].green = fromSerial[2];
      leds[fromSerial[0]].blue = fromSerial[3];
      FastLED.show();
      byteIndex = 0;
    }
  }
}

void resetLeds() {
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = CRGB::Black;
  }
}
