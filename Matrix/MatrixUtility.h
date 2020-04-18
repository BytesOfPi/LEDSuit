/*
 * 
 * ???: https://github.com/???
 * Copyright (c) 2020 Nathan DeGroff
 * 
 * Portions of this code were adopted from Effects.h
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 *     -) mapsin8
 *     -) mapcos8
 *     -) beatcos8
 *     -) mapcos8
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

#include <FastLED.h>
#include <FastLED_GFX.h>

#include "MatrixScroll.h"
#include "MatrixTimer.h"

#ifndef CANVAS_WIDTH
#define CANVAS_WIDTH 16
#define CANVAS_HEIGHT 16
#define CANVAS_CENTER_X 8
#define CANVAS_CENTER_Y 8
#endif

#ifndef MatrixUtility_H
#define MatrixUtility_H

//#################################################################
// Create scroll and timer message classes
MatrixScroll scroll(CANVAS_WIDTH, CANVAS_HEIGHT);
MatrixTimer timer(CANVAS_WIDTH, CANVAS_HEIGHT);

//#################################################################
// Utility functions
/*
 * bleMatrixScroll()
 * This method is called from Bluetooth and directs what actions
 * the global PatternScroll (scroll) class will do.
 */
void bleMatrixScroll(String val)
{
  String msg = "Scroll MATRIX [" + val + "]";
  Serial.println(msg);

  // Clear out existing message
  if (val.startsWith("***"))
  {
    scroll.setMsg("");
    return;
  }
  // Start showing message
  if (val.startsWith("###"))
  {
    scroll.setShowText(true);
    return;
  }
  // Stop showing message
  if (val.startsWith("@@@"))
  {
    scroll.setShowText(false);
    return;
  }
  // Otherwise, just add string to end message
  scroll.addMsg(val);
}

/*
 * bleMatrixPattern()
 * This method is called from Bluetooth and directs what actions
 * the global PatternScroll (scroll) class will do.
 */
void bleMatrixPattern(String val)
{
  // TODO
}

/*
 * bleMatrixTimer()
 * This method is called from Bluetooth and directs what actions
 * the global PatternScroll (scroll) class will do.
 */
void bleMatrixTimer(String val)
{
  String msg = "Timer MATRIX [" + val + "]";
  Serial.println(msg);
  // Set the timer
  if (val.startsWith("***"))
  {
    int i = val.indexOf(':');
    String m = val.substring(3, i);
    String s = val.substring(i + 1);
    timer.setTime(m.toInt(), s.toInt());
    timer.setShow(true);
    return;
  }
  // Start the countdown
  if (val.startsWith("###"))
  {
    Serial.println("Start Countdown: ");
    timer.startCountdown();
    return;
  }
  // Stop the countdown
  if (val.startsWith("==="))
  {
    Serial.println("Pause Countdown: ");
    timer.pauseCountdown();
    return;
  }
  // Stop the countdown
  if (val.startsWith("@@@"))
  {
    Serial.println("Stop Countdown: ");
    timer.stopCountdown();
    return;
  }
}

/* getColor()
 * Quick method to get a color from a particular palette defaulting common values
 */
CRGB getColor(CRGBPalette16 pal, uint8_t index = 0, uint8_t brightness = 126, TBlendType blendType = LINEARBLEND)
// CRGB getColor(uint8_t index = 0, const CRGBPalette16 &pal = RainbowColors_p, uint8_t brightness = 126, TBlendType blendType = LINEARBLEND)
{
  return ColorFromPalette(pal, index, brightness, blendType);
}
/* getColor()
 * Default palette to RainbowColors_p
 */
CRGB getColor(uint8_t index)
{
  return getColor(RainbowColors_p, index);
}

/*
 * scrollText()
 * Take a String and scroll it across the canvas using party palatte colors
 */
int pass = 0;
int nextX = CANVAS_WIDTH;
boolean scrollTextF(String text, uint8_t yCoord, GFXcanvas canvas, CRGB color, boolean background = false)
{
  if (background)
  {
    canvas.setTextColor(color, CRGB::Black);
  }
  else
  {
    canvas.setTextColor(color);
  }

  canvas.setCursor(nextX, yCoord);
  canvas.print(text);
  nextX--;

  if (canvas.getCursorX() == 0)
  {
    nextX = CANVAS_WIDTH;
    pass = (pass < 2) ? pass + 1 : 0;
    return false;
  }

  return true;
}

/* mapsin8()
 * Takes a theta, calculates sin (between 0-255), maps between min max val
 */
uint8_t mapsin8(byte theta, uint8_t minVal, uint8_t maxVal)
{
  //return map( sin8_avr(theta), 0, 255, minVal, maxVal );
  return map(sin8_C(theta), 0, 255, minVal, maxVal);
}
/* mapcos8()
 * Takes a theta, calculates cos (between 0-255), maps between min max val
 */
uint8_t mapcos8(byte theta, uint8_t minVal, uint8_t maxVal)
{
  // return map8( sin8_avr(theta), 0, 255, minVal, maxVal );
  return map(cos8(theta), 0, 255, minVal, maxVal);
}

/* beatcos8()
 * Don't know what this does... I'm sure it's cool... will look at detail later
 */
uint8_t beatcos8(accum88 beats_per_minute, uint8_t lowest = 0, uint8_t highest = 255, uint32_t timebase = 0, uint8_t phase_offset = 0)
{
  uint8_t beat = beat8(beats_per_minute, timebase);
  uint8_t beatcos = cos8(beat + phase_offset);
  uint8_t rangewidth = highest - lowest;
  uint8_t scaledbeat = scale8(beatcos, rangewidth);
  uint8_t result = lowest + scaledbeat;
  return result;
}

/* beattriwave8()
 * Don't know what this does... I'm sure it's cool... will look at detail later
 */
uint8_t beattriwave8(accum88 beats_per_minute, uint8_t lowest = 0, uint8_t highest = 255, uint32_t timebase = 0, uint8_t phase_offset = 0)
{
  uint8_t beat = beat8(beats_per_minute, timebase);
  uint8_t beatcos = triwave8(beat + phase_offset);
  uint8_t rangewidth = highest - lowest;
  uint8_t scaledbeat = scale8(beatcos, rangewidth);
  uint8_t result = lowest + scaledbeat;
  return result;
}

uint8_t XY(uint8_t x, uint8_t y)
{
  if ((x < 0) || (y < 0) || (x >= CANVAS_WIDTH) || (y >= CANVAS_HEIGHT))
    return 0;

  // 02/29/2020 - NATE
  // Adjust for zigzag by swap x on even ys
  if (y % 2 == 0)
  {
    x = CANVAS_WIDTH - x - 1;
  }

  return x + y * CANVAS_WIDTH;
}

#endif