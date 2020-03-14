/* ################################################################################
 * LED Main
 * The following file defines the main setup and loop for LEDs as well as shared
 * variables and methods
 */
#define DATA_PIN_001 4
#define DATA_PIN_002 16
#define DATA_PIN_003 17

#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS_001 75
#define NUM_LEDS_002 150
#define NUM_LEDS_003 75
#define LED_INDEX_001 NUM_LEDS_001
#define LED_INDEX_002 NUM_LEDS_001 + NUM_LEDS_002
#define LED_INDEX_003 LED_INDEX_002 + NUM_LEDS_003
#define NUM_LEDS 300
CRGB leds[NUM_LEDS];

#define BRIGHTNESS 96
#define FRAMES_PER_SECOND 120

#define TOT_SCROLL 30

uint8_t gHue = 0;                  // rotating "base color" used by many of the patterns
byte offset = 0;                   // rotating offset used to push theater patterns on

// Used for palette style patterns
uint8_t startIndex = 0;
byte currentPalPattern;
CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType currentBlending = LINEARBLEND;

//#################################################################
// Pre defined custom color palettes
const TProgmemPalette16 myCCHS1Palette_p PROGMEM =
    {
        // Other purples are:  //CRGB::Indigo, CRGB::Purple, CRGB::DarkMagenta
        CRGB::Purple, CRGB::Gold, CRGB::Purple, CRGB::Gold,
        CRGB::Black, CRGB::Purple, CRGB::Gold, CRGB::Black,
        CRGB::Purple, CRGB::Gold, CRGB::Purple, CRGB::Gold,
        CRGB::Black, CRGB::Purple, CRGB::Gold, CRGB::Black};
const TProgmemPalette16 myCCHS2Palette_p PROGMEM =
    {
        // Other purples are:  //CRGB::Indigo, CRGB::Purple, CRGB::DarkMagenta
        CRGB::Purple, CRGB::Purple, CRGB::Gold, CRGB::Gold,
        CRGB::Purple, CRGB::Purple, CRGB::Gold, CRGB::Gold,
        CRGB::Purple, CRGB::Purple, CRGB::Gold, CRGB::Gold,
        CRGB::Purple, CRGB::Purple, CRGB::Gold, CRGB::Gold};
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
    {
        // Using CRGB::Gray instead of CRGB::White.  Too bright compared to red and blue
        CRGB::Red, CRGB::Gray, CRGB::Blue, CRGB::Black,
        CRGB::Red, CRGB::Gray, CRGB::Blue, CRGB::Black,
        CRGB::Red, CRGB::Gray, CRGB::Blue, CRGB::Black,
        CRGB::Red, CRGB::Gray, CRGB::Blue, CRGB::Black};
const TProgmemPalette16 myBlueGreenPalette_p PROGMEM =
    {
        CRGB::Blue, CRGB::Green, CRGB::Green,
        CRGB::Blue, CRGB::Green, CRGB::Green,
        CRGB::Blue, CRGB::Green, CRGB::Green,
        CRGB::Blue, CRGB::Green, CRGB::Green,
        CRGB::Blue, CRGB::Green, CRGB::Green,
        CRGB::Blue};
const TProgmemPalette16 myChristmasPalette_p PROGMEM =
    {
        CRGB::Gray, CRGB::Red, CRGB::Green, CRGB::SaddleBrown,
        CRGB::Gray, CRGB::Red, CRGB::Green, CRGB::SaddleBrown,
        CRGB::Gray, CRGB::Red, CRGB::Green, CRGB::SaddleBrown,
        CRGB::Gray, CRGB::Red, CRGB::Green, CRGB::SaddleBrown};


//#################################################################
// Utility functions
#include "Strand/StrandUtility.h"

//#################################################################
// Utility functions
/*
 * LED Outfit chaser correct()
 * The LED outfit starts at the waist so the starting pixel of the LED strands are close
 * to the person's waist.  If you want the chaser to go up one leg and down the other, 
 * we flip the first 75 pixel positions using this function so pixel position 0 is at
 * the foot rather than at waist.
 */
int chaserCorrect(int val)
{
  return (val < 75) ? map(val, 0, 74, 74, 0) : val;
}
/*
 * fillRange()
 * There is probably a better way to do this, but this method just loops
 * and sets lights in a range
 */
void fillRange(int pos, int ledEnd, CRGB rgb)
{
  for (; pos < ledEnd; pos++)
  {
    leds[chaserCorrect(pos)] = rgb;
  }
}
/*
 * fillLEDsFromPaletteColors()
 * This method is called for pallet patterns.  
 */
void fillLEDsFromPaletteColors(CRGBPalette16 pal, TBlendType blendType, uint8_t colorIndex)
{
  //uint8_t brightness = 255;
  uint8_t brightness = 175;

  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = ColorFromPalette(pal, colorIndex, brightness, blendType);
    colorIndex += 3;
  }
}
/*
 * setLEDs()
 * This method will just load a range of LEDs with a color
 */
void setLEDs(int ledStart, int ledEnd, CRGB color)
{
  for (int i = ledStart; i < ledEnd; i++)
  {
    leds[i] = color;
  }
}

/*
 * addGlitter()
 * modifies certain random pixels with white to cause sparkle
 */
void addGlitter(fract8 chanceOfGlitter)
{
  if (random8() < chanceOfGlitter)
  {
    leds[random16(NUM_LEDS)] += CRGB::White;
  }
}

/*
 * bpm()
 * Takes a palette and pulses it back and forth each second causing it
 * to slightly move down the chain... each color is gradually faded
 */
void bpm(CRGBPalette16 palette)
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}

/*
 * theater()
 * Display a pallet in theater scroll
 */
void theater(CRGBPalette16 pal)
{
  byte pattOffset = offset % TOT_SCROLL;
  uint8_t brightness = 175;
  CRGB crgb1 = ColorFromPalette(pal, 0, brightness, NOBLEND);
  CRGB crgb2 = ColorFromPalette(pal, 16, brightness, NOBLEND);
  CRGB crgb3 = ColorFromPalette(pal, 32, brightness, NOBLEND);
  CRGB crgb4 = ColorFromPalette(pal, 48, brightness, NOBLEND);

  for (int i = 0; i < NUM_LEDS; i++)
  {
    // Setup theater scroll
    byte val = (i + offset) % TOT_SCROLL;
    CRGB nextVal;
    if (val < 4)
      nextVal = CRGB::Black;
    else if (val < 8)
      nextVal = crgb1;
    else if (val < 13)
      nextVal = crgb2;
    else if (val < 17)
      nextVal = crgb3;
    else if (val < 21)
      nextVal = crgb4;
    else if (val < 24)
      nextVal = CRGB::Black;
    else if (val < 27)
      nextVal = crgb2;
    else
      nextVal = crgb3;

    leds[i] = nextVal;
  }
}

/*
 * mixins()
 * chasers on legs, but pallet on shoulder
 */
void mixins(CRGBPalette16 pal, TBlendType blendType, uint8_t colorIndex)
{
  fadeToBlackBy(leds, NUM_LEDS, 90);
  uint8_t brightness = 175;

  // Shoulder pallete
  for (int i = LED_INDEX_001; i < LED_INDEX_002; i++)
  {
    leds[i] = ColorFromPalette(pal, colorIndex, brightness, blendType);
    colorIndex += 3;
  }

  CRGB pals[8] = {ColorFromPalette(pal, 0, brightness, NOBLEND),
                  ColorFromPalette(pal, 16, brightness, NOBLEND),
                  ColorFromPalette(pal, 32, brightness, NOBLEND),
                  ColorFromPalette(pal, 48, brightness, NOBLEND),
                  ColorFromPalette(pal, 64, brightness, NOBLEND),
                  ColorFromPalette(pal, 80, brightness, NOBLEND),
                  ColorFromPalette(pal, 96, brightness, NOBLEND),
                  ColorFromPalette(pal, 112, brightness, NOBLEND)};
  // Chasers on the legs
  byte dothue = 0;
  for (int i = 0; i < 8; i++)
  {
    leds[beatsin16(24, 0, LED_INDEX_001 - 1, i * 20)] = pals[i];
    leds[beatsin16(24, LED_INDEX_002, LED_INDEX_003 - 1, i * 20)] = pals[i];
    dothue += 32;
  }
}

void mixSpec(CRGBPalette16 pal, TBlendType blendType, uint8_t colorIndex)
{
  uint8_t brightness = 175;
  // Shoulder pallete
  for (int i = LED_INDEX_001; i < LED_INDEX_002; i++)
  {
    leds[i] = ColorFromPalette(pal, colorIndex, brightness, blendType);
    colorIndex += 3;
  }

  CRGB pals[8] = {ColorFromPalette(pal, 0, brightness, NOBLEND),
                  ColorFromPalette(pal, 16, brightness, NOBLEND),
                  ColorFromPalette(pal, 32, brightness, NOBLEND),
                  ColorFromPalette(pal, 48, brightness, NOBLEND),
                  ColorFromPalette(pal, 64, brightness, NOBLEND),
                  ColorFromPalette(pal, 80, brightness, NOBLEND),
                  ColorFromPalette(pal, 96, brightness, NOBLEND),
                  ColorFromPalette(pal, 112, brightness, NOBLEND)};
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy(leds, NUM_LEDS, 10);
  int pos1 = random16(NUM_LEDS_001);
  int pos2 = random16(NUM_LEDS_001);
  leds[pos1] = pals[random8(8)];
  leds[pos2 + LED_INDEX_002] = pals[random8(8)];
}
//#################################################################
// setup() and loop()
/*
 * ledSetup()
 * This method is invoked at the start to set up the LEDs and necessary values
 */
void ledSetup()
{
  delay(3000); // 3 second delay to allow capacitor to fill up and LEDs to juice up.

  // tell FastLED about the LED strip configuration
  // FastLED.addLeds<LED_TYPE, DATA_PIN_001, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );

  // tell FastLED there 3 data pins,
  //    first set starts at 0 and goes NUM_LEDS_001 length
  //    first set starts at NUM_LEDS_001 and goes NUM_LEDS_002 length
  //    first set starts at NUM_LEDS_001 + NUM_LEDS_002 and goes NUM_LEDS_003 length
  FastLED.addLeds<LED_TYPE, DATA_PIN_001, COLOR_ORDER>(leds, 0, NUM_LEDS_001).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN_002, COLOR_ORDER>(leds, NUM_LEDS_001, NUM_LEDS_002).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN_003, COLOR_ORDER>(leds, NUM_LEDS_001 + NUM_LEDS_002, NUM_LEDS_003).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  // Initialize first palette pattern
  confetti();

  randomSeed(analogRead(0));
}

/*
 * ledLoop()
 * This method is invoked each time to draw LEDs
 */
void ledLoop()
{
  // Used only for palette patterns to move the pattern along strand
  startIndex = startIndex + 1; /* motion speed */

  // Make PATTERN_NUMBER safe for array
  safePattern();

  // Call the current pattern function once, updating the 'leds' array
  gPatterns[PATTERN_NUMBER]();

  //--------------------------------------------------------------
  // Hold off on drawing till Matrix and LED loop have drawn next
  // frame
  //FastLED.show();

  //--------------------------------------------------------------
  // No need to keep a delay to keep framerate modest.  Rely upon
  // EVERY_N_??? methods to keep framerates slower if needed.
  // FastLED.delay(1000/FRAMES_PER_SECOND);

  // do some periodic updates
  EVERY_N_MILLISECONDS(20) { gHue++; } // slowly cycle the "base color" through the rainbow

  EVERY_N_MILLISECONDS(100)
  {
    offset++;
    if (offset == TOT_SCROLL)
    {
      offset = 0;
    }
  } // slowly push rotating offset
}
