/*
 * PatternCustomBit.h: https://github.com/BytesOfPi/LEDSuit
 * Copyright (c) 2020 Nathan DeGroff
 *
 * Overview:
 * This Drawable class defines and draws the matrix component bitmap pattern.
 * Just a little test on animating bitmaps on the matrix.
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

#ifndef PatternCustomBit_H
#define PatternCustomBit_H

#include "Drawable.h"
#include "MatrixUtility.h"

static const uint8_t PROGMEM // Bitmaps are stored in program memory
    smileyImg[8] =
        {B00111100, // smiley face
         B01000010,
         B10100101,
         B10100101,
         B10000001,
         B10100101,
         B01011010,
         B00111100},
    blinkImg[][8] = {
        {B00111100, // Eye open
         B01000010,
         B01000010,
         B01000010,
         B01011010,
         B01011010,
         B01000010,
         B00111100},
        {B00000000,
         B00111100,
         B01000010,
         B01000010,
         B01011010,
         B01011010,
         B01000010,
         B00111100},
        {B00000000,
         B00000000,
         B00111100,
         B01000010,
         B01011010,
         B01011010,
         B01000010,
         B00111100},
        {B00000000,
         B00000000,
         B00000000,
         B00111100,
         B01011010,
         B01011010,
         B00111100,
         B00000000},
        {B00000000,
         B00000000,
         B00000000,
         B00000000,
         B00111100,
         B01011010,
         B00111100,
         B00000000},
        {B00000000, // Eye shut
         B00000000,
         B00000000,
         B00000000,
         B00000000,
         B01111110,
         B00111100,
         B00000000}};

class PatternCustomBit : public Drawable
{
private:
    const uint8_t BRIGHTNESS_MATRIX = 126;
    uint8_t colorIndex = 0;
public:
    //--------------------------------------------------------------
    // Constructor
    PatternCustomBit(uint8_t w, uint8_t h) : Drawable(MATRIX_PATT_CUSTOM, w, h) {}

    /* blink()
     * Animate blinking eyes
     */
    void blink(GFXcanvas canvas)
    {
        int16_t index = beatsin16(10, 0, 5);
        canvas.drawBitmap(1, 8, (const uint8_t *)blinkImg[index], (int16_t)8, (int16_t)8, CRGB::Blue);
        canvas.drawBitmap(8, 8, (const uint8_t *)blinkImg[index], (int16_t)8, (int16_t)8, CRGB::Blue);
    }
    /*
    * smiley()
    * Smiley Face animation
    */
    void smiley(GFXcanvas canvas)
    {
        colorIndex += 5;
        CRGB color = ColorFromPalette(pal, colorIndex, BRIGHTNESS_MATRIX, LINEARBLEND);
        int16_t thisX = beatsin16(12, -8, CANVAS_WIDTH);
        canvas.drawBitmap(thisX, 0, (const uint8_t *)smileyImg, (int16_t)8, (int16_t)8, color);
    }

    /* drawFrame()
     * Primary method that draws the next frame
     */
    unsigned int drawFrame(GFXcanvas canvas)
    {
        //--------------------------------------------------------------
        // Draw the new animated frame
        canvas.fillScreen(CRGB::Black);
        blink(canvas);
        smiley(canvas);
        return 0;
    }
};
#endif