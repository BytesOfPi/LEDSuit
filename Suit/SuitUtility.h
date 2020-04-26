/*
 * SuitUtility.h: https://github.com/BytesOfPi/LEDSuit
 * Copyright (c) 2020 Nathan DeGroff
 *
 * Overview:
 * This file defines the methods used when communicating with BLE controller application.
 * It allows the BLE to see which pattern is selected and to control which pattern is
 * displayed.
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

//---------------------------------------------------------
// Make sure BLE Utility has been called for bleNotifyStrandMsg()
#include "../BLE/BLEUtility.h"

//---------------------------------------------------------
// Shared global PATTERN_NUMBER.  Tells / controls which
// LED sequence is running
byte PATTERN_NUMBER = 0;

#define TOT_PATTERN_NUM 27
#define MAX_PATTERN_INDEX TOT_PATTERN_NUM - 1

// Hiding rainbows ("rainbow", "paletteRainbowStripe", "rainbowSolid", "paletteParty")
String gPattName[] = {"cycle",
                      "paletteCCHS", "paletteCCHS2",
                      "palletteCCHSGlitter", "twoSplit", "juggle",
                      "confetti", "paletteRWB", "paletteCloud",
                      "paletteLava", "paletteHeat", "paletteOcean", "paletteForest", "paletteChristmas",
                      "bpmParty", "bpmRWB", "bpmChristmas",
                      "solidBlue", "solidRed", "solidGreen",
                      "theaterCCHS", "theaterChristmas",
                      "mixinsCCHS", "mixinsLava", "mixSpecLava", "mixSpecOcean", "mixSpecForest"};

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
 * nextSuitPattern()
 * This method will increment the pattern to the next in list
 */
void nextSuitPattern()
{
  PATTERN_NUMBER = SAFE_NEXT_PATTERN(PATTERN_NUMBER);

  // alert new pattern change
  alertPattern();
}

/*
 * prevSuitPattern()
 * This method will decrement the pattern to the next in list
 */
void prevSuitPattern()
{
  PATTERN_NUMBER = SAFE_PREV_PATTERN(PATTERN_NUMBER);
  // alert new pattern change
  alertPattern();
}

/*
 * setPattern()
 * This method will take a string pattern and set it if it exists
 */
void setSuitPattern(String patt)
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
 * bleSuitPattern()
 * This method is called from Bluetooth and directs what pattern
 * the suit will take on.
 */
void bleSuitPattern(String val)
{
    // If phone sent "back", back up to previous LED pattern
    if (val.equals("back"))
    {
      prevSuitPattern();
    }
    // If phone sent "next", cycle to next LED pattern
    else if (val.equals("next"))
    {
      nextSuitPattern();
    }
    // Otherwise, set pattern directly
    else
    {
      setSuitPattern(val);
    }
}

#endif