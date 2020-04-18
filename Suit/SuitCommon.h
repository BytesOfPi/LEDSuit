/*
 * SuitCommon.h: https://github.com/BytesOfPi/LEDSuit
 * Copyright (c) 2020 Nathan DeGroff
 * 
 * Overview:
 * This file defines generic common methods used in the Suit class.  Since they are
 * common methods, they were moved out of Suit class to make it less cluttered.
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
void fillRange(CRGB myLeds[], int pos, int ledEnd, CRGB rgb)
{
  for (; pos < ledEnd; pos++)
  {
    myLeds[chaserCorrect(pos)] = rgb;
  }
}

/*
 * setLEDs()
 * This method will just load a range of LEDs with a color
 */
void setLEDs(CRGB myLeds[], int ledStart, int ledEnd, CRGB color)
{
  for (int i = ledStart; i < ledEnd; i++)
  {
    myLeds[i] = color;
  }
}

/*
 * addGlitter()
 * modifies certain random pixels with white to cause sparkle
 */
void addGlitter(CRGB myLeds[], int numLeds, fract8 chanceOfGlitter)
{
  if (random8() < chanceOfGlitter)
  {
    myLeds[random16(numLeds)] += CRGB::White;
  }
}

/*
 * bpm()
 * Takes a palette and pulses it back and forth each second causing it
 * to slightly move down the chain... each color is gradually faded
 */
void bpm(CRGB myLeds[], int numLeds, uint8_t myHue, CRGBPalette16 palette)
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
  for (int i = 0; i < numLeds; i++)
  {
    myLeds[i] = ColorFromPalette(palette, myHue + (i * 2), beat - myHue + (i * 10));
  }
}

/*
 * theater()
 * Display a pallet in theater scroll
 */
void theater(CRGB myLeds[], int numLeds, int offset, CRGBPalette16 pal)
{
  byte pattOffset = offset % TOT_SCROLL;
  uint8_t brightness = 175;
  CRGB crgb1 = ColorFromPalette(pal, 0, brightness, NOBLEND);
  CRGB crgb2 = ColorFromPalette(pal, 16, brightness, NOBLEND);
  CRGB crgb3 = ColorFromPalette(pal, 32, brightness, NOBLEND);
  CRGB crgb4 = ColorFromPalette(pal, 48, brightness, NOBLEND);

  for (int i = 0; i < numLeds; i++)
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

    myLeds[i] = nextVal;
  }
}