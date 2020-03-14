/* ################################################################################
 * LED methods
 * The following file defines methods that run and control the LED patterns.
 */

//#################################################################
// Solid non moving patterns
void solidRed() 
{
  // FastLED's built-in rainbow generator
  fill_solid( leds, NUM_LEDS, CHSV( HUE_RED, 200, 200 ) );
}
void solidBlue() 
{
  // FastLED's built-in rainbow generator
  fill_solid( leds, NUM_LEDS, CHSV( HUE_BLUE, 255, 255) );
}

void solidGreen() 
{
  // FastLED's built-in rainbow generator
  fill_solid( leds, NUM_LEDS, CHSV( HUE_GREEN, 255, 255) );
}

void solidBlueGreen() 
{
  // Setting startIndex to zero keeps colors steady.
  fillLEDsFromPaletteColors( myBlueGreenPalette_p, LINEARBLEND, 0 );
}

void solidCCHS()
{
  fillLEDsFromPaletteColors( myCCHS1Palette_p, LINEARBLEND, 0);
}

void rainbowSolid() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, 128, 1 );
}

//#################################################################
// Patterns with slight changes
/*
 * Rainbow pallet with the hue of the rainbow moving slightly
 */
void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

/*
 * Same as rainbow(), but adds random sparkles
 */
void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

/*
 * Color changing spots randomly appear
 */
void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

//#################################################################
// Chasers
/*
 * sinelon()
 * 2 dots closely chase each other back and forth
 */
void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  //fadeToBlackBy( leds, NUM_LEDS, 20);
  //int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  fadeToBlackBy( leds, NUM_LEDS, 80 );//20
  int pos = chaserCorrect( beatsin16( 6, 0, NUM_LEDS-1 ) );
  leds[pos] += CHSV( gHue, 255, 192);

  // second chaser
  pos = chaserCorrect( beatsin16( 6, 0, NUM_LEDS-1, 50 ) );
  leds[pos] += CHSV( HUE_AQUA, 255, 192);
}

/*
 * twoSplit()
 * Small red and blue chaser criss cross at center
 */
void twoSplit()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 110);//20
  uint16_t halfway = NUM_LEDS / 2;
  int pos1 = beatsin16( 6, 0, NUM_LEDS-1 );
  int pos2 = chaserCorrect( NUM_LEDS-1-pos1 );
  pos1 = chaserCorrect( pos1 );
  leds[pos1] += CHSV( HUE_BLUE, 200, 192);
  leds[pos2] += CHSV( HUE_RED, 200, 192);
  //leds[pos1] = CRGB::Gray;
  //leds[pos2] = CRGB::Gray;
}

/*
 * juggle()
 * 8 color changing dots closely chase each other back and forth
 */
void juggle() {
  byte dothue = 0;
  fadeToBlackBy( leds, NUM_LEDS, 90); //20
  for( int i = 0; i < 8; i++) {
    leds[chaserCorrect( beatsin16( 8, 0, NUM_LEDS-1, i*20 ) )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

boolean powerColor = true;
int powerPos = NUM_LEDS / 2;
void power() {
  int powerPosReverse = NUM_LEDS-1-powerPos;
  leds[chaserCorrect(powerPos)] += powerColor ? CHSV( gHue, 255, 192 ) : CHSV(255, 0, 0);
  leds[chaserCorrect(powerPosReverse)] += powerColor ? CHSV( gHue, 255, 192 ) : CHSV(255, 0, 0);
  // If we reach the end, start back at middle and flip whether we are coloring
  // or clearing
  if ( powerPos == 0 || powerPosReverse )
  {
     Serial.println("power switch: [" + String(powerColor) + "] " );
     powerPos = NUM_LEDS / 2;
     powerColor = !powerColor;
  }
  
}
//#################################################################
// Pulsing
/*
 * bpmParty()
 * Pulsing Party colors
 */
void bpmParty()
{
  bpm( PartyColors_p );
}
/*
 * bpmRWB()
 * Pulsing Red-White-Blue
 */
void bpmRWB()
{
  bpm( myRedWhiteBluePalette_p );
}
/*
 * bpmChristmas()
 * Pulsing Christmas
 */
void bpmChristmas()
{
  bpm( myChristmasPalette_p );
}

//#################################################################
// Moving pallets
/*
 * paletteParty()
 * Display Party colors
 */
void paletteParty()
{
  // No blend...
  // fillLEDsFromPaletteColors( PartyColors_p, NOBLEND, startIndex);
  // Stands still...
  // fillLEDsFromPaletteColors( PartyColors_p, LINEARBLEND, 0);
  fillLEDsFromPaletteColors( PartyColors_p, LINEARBLEND, startIndex );
}

/*
 * paletteCCHS()
 * Display CCHS colors (moving)
 */
void paletteCCHS() {
  fillLEDsFromPaletteColors( myCCHS1Palette_p, LINEARBLEND, startIndex );
}
/*
 * palletteCCHSGlitter()
 * Display second set of CCHS colors (moving)
 */
void paletteCCHS2() {
  fillLEDsFromPaletteColors( myCCHS2Palette_p, LINEARBLEND, startIndex );
}

/*
 * palletteCCHSGlitter()
 * Display CCHS colors with random sparkles
 */
void palletteCCHSGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  paletteCCHS2();
  addGlitter(80);
}

/*
 * paletteRWB()
 * Display Red-White-Blue palette
 */
void paletteRWB() {
  fillLEDsFromPaletteColors( myRedWhiteBluePalette_p, LINEARBLEND, startIndex );
}
/*
 * paletteCloud()
 * Display Cloud palette
 */
void paletteCloud() {
  fillLEDsFromPaletteColors( CloudColors_p, LINEARBLEND, startIndex );
}
/*
 * paletteLava()
 * Display Lava palette
 */
void paletteLava() {
  fillLEDsFromPaletteColors( LavaColors_p, LINEARBLEND, startIndex );
}
/*
 * paletteHeat()
 * Display Heat palette
 */
void paletteHeat() {
  fillLEDsFromPaletteColors( HeatColors_p, LINEARBLEND, startIndex );
}
/*
 * paletteOcean()
 * Display Ocean palette
 */
void paletteOcean() {
  fillLEDsFromPaletteColors( OceanColors_p, LINEARBLEND, startIndex );
}
/*
 * paletteForest()
 * Display Forest palette
 */
void paletteForest() {
  fillLEDsFromPaletteColors( ForestColors_p, LINEARBLEND, startIndex );
}
/*
 * paletteRainbowStripe()
 * Display RainbowStripe palette
 */
void paletteRainbowStripe()
{
  fillLEDsFromPaletteColors( RainbowStripeColors_p, LINEARBLEND, startIndex );
}
/*
 * paletteChristmas()
 * Display Christmas palette
 */
void paletteChristmas()
{
  fillLEDsFromPaletteColors( myChristmasPalette_p, LINEARBLEND, startIndex );
}

//#################################################################
// Theater scroll
/*
 * theaterCCHS()
 * Display CCHS theater scroll bars
 */
void theaterCCHS()
{
  theater( myCCHS1Palette_p );
}
/*
 * theaterChristmas()
 * Display Christmas theater scroll bars
 */
void theaterChristmas()
{
  theater( myChristmasPalette_p );
}

void mixinsCCHS()
{
  mixins( myCCHS1Palette_p, LINEARBLEND, startIndex );
}

void mixinsLava()
{
  mixins( LavaColors_p, LINEARBLEND, startIndex );
}

void mixSpecLava()
{
  mixSpec( LavaColors_p, LINEARBLEND, startIndex );
}
void mixSpecOcean()
{
  mixSpec( OceanColors_p, LINEARBLEND, startIndex );
}
void mixSpecForest()
{
  mixSpec( ForestColors_p, LINEARBLEND, startIndex );
}


//#################################################################
// Lightning
#define LTNG_MIN_FLASH 3
#define LTNG_MAX_FLASH 8
#define LTNG_MIN_FREQ_SEC 2
#define LTNG_MAX_FREQ_SEC 10
int nextStrike = 5000;

int LTNG_FLASH_RANGE[8] = { 131, 112, 93, 74, 55, 36, 17, 0 };

/*
 * strike()
 * draw the strike
 */
void strike()
{
  FastLED.clear();
  FastLED.show();
  unsigned int dimmer = 1;
  //uint8_t val001 = random(NUM_LEDS);  // random point
  // uint8_t val002 = random(NUM_LEDS);  // random point
  // uint8_t ledStart = min(val001, val002);  // Determine starting location of flash
  // uint8_t ledEnd = max(val001, val002);  // Determine length of flash (not to go beyond NUM_LEDS-1)
  
  int numFlashes = random8(LTNG_MIN_FLASH, LTNG_MAX_FLASH);
  // Loop through random number of flashes
  for (int flashCounter = 0; flashCounter < numFlashes; flashCounter++)
  {
    if (flashCounter == 0) dimmer = 5;    // the brightness of the leader is scaled down by a factor of 5
    else dimmer = random8(2, 4);          // return strokes are brighter than the leader

    int ledStart01 = LTNG_FLASH_RANGE[map(flashCounter, 0, numFlashes - 1, 0, 7)];
    int ledEnd01 = ledStart01 + min( 150 - ledStart01 - 1
    
    , 75 );
    int ledStart02 = NUM_LEDS - ledEnd01 - 1;
    int ledEnd02 = NUM_LEDS - ledStart01 - 1;
    
    Serial.println("flash: [" + String( ledStart01 ) + "][" + String( ledEnd01 ) +  "][" + String( ledStart02 ) + "][" + String( ledEnd02 ) +  "]" );

    
    // Light Up
    fillRange( ledStart01, ledEnd01, CHSV(255, 0, 255 / dimmer) );
    fillRange( ledStart02, ledEnd02, CHSV(255, 0, 255 / dimmer) );
    FastLED.show();                       // Show a section of LED's
    delay(random8(4, 30));                // each flash only lasts 4-10 milliseconds
    // Clear the section of LED's
    fillRange( ledStart01, ledEnd01, CHSV(255, 0, 0) );
    fillRange( ledStart02, ledEnd02, CHSV(255, 0, 0) );
    FastLED.show();
    if (flashCounter == 0) delay (150);   // longer delay until next flash after the leader
    delay(50 + random8(100));             // shorter delay between strokes
  }
}

/*
 * lightning()
 * show random lightning strike
 */
void lightning()
{
  EVERY_N_MILLIS_I( myTimer, 20 )
  {
    strike();
    nextStrike = random(LTNG_MIN_FREQ_SEC, LTNG_MAX_FREQ_SEC) * 1000;
    Serial.println("next strike: [" + String( nextStrike/1000 ) + "] seconds" );
    myTimer.setPeriod(nextStrike);
  }
}

//#################################################################
// Cycle patterns
byte CYCLE_PATTERN_NUMBER = 1;  // Make sure not to set to skipped cycle index
int CYCLE_MILLISECONDS = 7000;  // 7 seconds
String skipCycles[2] = { "cycle", "lightning" };
boolean skipCycle()
{
  //------------------------------------------------------------
  // Loop through skip cycles
  for( int i = 0; i < ARRAY_SIZE(skipCycles); i++ )
  {
    //------------------------------------------------------------
    // if the pattern is in list of skip cycles, return true
    if (gPattName[CYCLE_PATTERN_NUMBER].equals(skipCycles[i])) { return true; }
  }
  return false;
}
void cycle()
{
  EVERY_N_MILLIS_I( cycleTimer, CYCLE_MILLISECONDS )
  {
     //------------------------------------------------------------
     // Cycle to the next safe pattern... if that pattern is itself
     // skip it... (avoid recursive call)
     CYCLE_PATTERN_NUMBER = SAFE_NEXT_PATTERN(CYCLE_PATTERN_NUMBER);
     while( skipCycle() )
     {
        CYCLE_PATTERN_NUMBER = SAFE_NEXT_PATTERN(CYCLE_PATTERN_NUMBER);
     }
     
    Serial.println("Running Pattern: [" + gPattName[CYCLE_PATTERN_NUMBER] + "] " );
  }
  // Call the rotated pattern pattern
  gPatterns[CYCLE_PATTERN_NUMBER]();
}
