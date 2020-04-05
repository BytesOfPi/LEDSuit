/*
 * 
 * ???: https://github.com/???
 * Copyright (c) 2020 Nathan DeGroff
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef SuitUtility_H
#define SuitUtility_H

#include "../BLE/BLEUtility.h"

//---------------------------------------------------------
// Shared global PATTERN_NUMBER.  Tells / controls which
// LED sequence is running
byte PATTERN_NUMBER = 0;

// Define quick function to get number of entries in array
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
// Hiding rainbows ("rainbow", "paletteRainbowStripe", "rainbowSolid", "paletteParty")
SimplePatternList gPatterns = {cycle,
                               paletteCCHS, paletteCCHS2, solidCCHS,
                               palletteCCHSGlitter, twoSplit, juggle,
                               confetti, paletteRWB, paletteCloud,
                               paletteLava, paletteHeat, paletteOcean, paletteForest, paletteChristmas,
                               bpmParty, bpmRWB, bpmChristmas,
                               solidBlueGreen, solidBlue, solidRed, solidGreen,
                               theaterCCHS, theaterChristmas,
                               mixinsCCHS, mixinsLava, mixSpecLava, mixSpecOcean, mixSpecForest,
                               lightning, power};
const int MAX_PATTERN_INDEX = ARRAY_SIZE(gPatterns) - 1;


// Hiding rainbows ("rainbow", "paletteRainbowStripe", "rainbowSolid", "paletteParty")
String gPattName[MAX_PATTERN_INDEX + 1] = {"cycle",
                                           "paletteCCHS", "paletteCCHS2", "solidCCHS",
                                           "palletteCCHSGlitter", "twoSplit", "juggle",
                                           "confetti", "paletteRWB", "paletteCloud",
                                           "paletteLava", "paletteHeat", "paletteOcean", "paletteForest", "paletteChristmas",
                                           "bpmParty", "bpmRWB", "bpmChristmas",
                                           "solidBlueGreen", "solidBlue", "solidRed", "solidGreen",
                                           "theaterCCHS", "theaterChristmas",
                                           "mixinsCCHS", "mixinsLava", "mixSpecLava", "mixSpecOcean", "mixSpecForest",
                                           "lightning", "power"};

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current

#define SAFE_NEXT_PATTERN(val) (val + 1 > MAX_PATTERN_INDEX ? 0 : val + 1)
#define SAFE_PREV_PATTERN(val) (val == 0 ? MAX_PATTERN_INDEX : val - 1)

/*
 * alertPattern()
 * This method will send to serial and to Bluetooth what pattern is selected
 */
void alertPattern()
{
  String msg = "[" + String(PATTERN_NUMBER) + "][" + gPattName[PATTERN_NUMBER] + "]";
  Serial.println(msg);
  bleNotifyStrandMsg(msg);
}

/*
 * nextPattern()
 * This method will increment the pattern to the next in list
 */
void nextPattern()
{
  PATTERN_NUMBER = SAFE_NEXT_PATTERN(PATTERN_NUMBER);

  // alert new pattern change
  alertPattern();
}

/*
 * prevPattern()
 * This method will decrement the pattern to the next in list
 */
void prevPattern()
{
  PATTERN_NUMBER = SAFE_PREV_PATTERN(PATTERN_NUMBER);
  // alert new pattern change
  alertPattern();
}

/*
 * setPattern()
 * This method will take a string pattern and set it if it exists
 */
void setPattern(String patt)
{
  // loop through patterns
  for (int i = 0; i <= MAX_PATTERN_INDEX; i++)
  {
    // if we find the selected pattern, set the index
    if (patt.equalsIgnoreCase(gPattName[i]))
    {
      PATTERN_NUMBER = i;
      alertPattern();
    }
  }
}

/*
 * safePattern()
 * Makes sure the selected pattern doesn't exceed the defined set of methods that can
 * be called in the gPatterns array.  If user selected an item that isn't in the list,
 * it will chop off the tens digit and just use that pattern.
 */
void safePattern()
{
  if (PATTERN_NUMBER > MAX_PATTERN_INDEX)
  {
    Serial.print("Trimming ");
    Serial.print(PATTERN_NUMBER);
    Serial.print(" to ");
    PATTERN_NUMBER = PATTERN_NUMBER % 10;
    // alert new pattern change
    alertPattern();
  }
}

/*
 * bleSuit()
 * This method is called from Bluetooth and directs what actions
 * the global PatternScroll (scroll) class will do.
 */
void bleSuit(String val)
{
    // If phone sent "back", back up to previous LED pattern
    if (val.equals("back"))
    {
      prevPattern();
    }
    // If phone sent "next", cycle to next LED pattern
    else if (val.equals("next"))
    {
      nextPattern();
    }
    // Otherwise, set pattern directly
    else
    {
      setPattern(val);
    }
}

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

#endif