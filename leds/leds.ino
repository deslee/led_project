#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 300
#define DATA_PIN 5
#define MSG_LEN 6

#define LED_TYPE    WS2811
#define COLOR_ORDER RGB
#define BRIGHTNESS          255
#define FRAMES_PER_SECOND  120

// Define the array of leds
CRGB leds[NUM_LEDS];

// data
byte fromSerial[MSG_LEN];
int byteIndex = 0;
unsigned long lastSignal = millis();

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { noop, rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };
uint8_t gCurrentPatternNumber = 3; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void setup() {
  delay(3000); // 3 second delay for recovery
  
  Serial.begin(9600);
  while (!Serial) {}
  Serial.write(42);
  
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  fill_rainbow(leds, NUM_LEDS, CRGB::Red);
  //fill_solid(leds, NUM_LEDS, CRGB::Black);
  
  FastLED.show();
}

void loop() {
  if (Serial.available()) {
    lastSignal = millis();
    fromSerial[byteIndex] = Serial.read();
    Serial.write(fromSerial[byteIndex]);
    byteIndex++;
    if (byteIndex == MSG_LEN) {
      switch (fromSerial[0]) {
        case 1:
          // update single LED
          leds[fromSerial[1]].red = fromSerial[4];
          leds[fromSerial[1]].green = fromSerial[3];
          leds[fromSerial[1]].blue = fromSerial[5];
          gCurrentPatternNumber = 0;
          break;
        case 2:
          // fill range of leds
          fill_solid(leds, fromSerial[1] == 0 ? NUM_LEDS : fromSerial[1], CRGB(fromSerial[4], fromSerial[3], fromSerial[5]));
          gCurrentPatternNumber = 0;
          break;
        case 3:
          // rainbow range of leds
          fill_rainbow(leds, fromSerial[1] == 0 ? NUM_LEDS : fromSerial[1], CRGB(fromSerial[4], fromSerial[3], fromSerial[5]));
          gCurrentPatternNumber = 0;
          break;
        case 4:
          nextPattern();
          break;
      }
      byteIndex = 0;
    }
  }
  else if (millis() - lastSignal > 1000) {
    // more than a second has changed. Reset
    byteIndex = 0;
  }

  
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void noop() {}

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ( ARRAY_SIZE( gPatterns) );
  if (gCurrentPatternNumber == 0) {
    gCurrentPatternNumber++;
  }
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}
