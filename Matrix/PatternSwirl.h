/*
 * PatternSwirl.h: https://github.com/BytesOfPi/LEDSuit
 * Copyright (c) 2020 Nathan DeGroff
 * 
 * Overview:
 * This Drawable class defines and draws the matrix component swirl pattern.
 * 
 * 
 * Portions of this code are adapted from Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 *
 * Portions of this code are adapted from SmartMatrixSwirl by Mark Kriegsman: https://gist.github.com/kriegsman/5adca44e14ad025e6d3b
 * https://www.youtube.com/watch?v=bsGBT-50cts
 * Copyright (c) 2014 Mark Kriegsman
 * 
 * Portions of this code are adapted for LED Costume
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

#ifndef PatternSwirl_H
#define PatternSwirl_H

#include "Drawable.h"
#include "MatrixUtility.h"

class PatternSwirl : public Drawable
{
private:
    const uint8_t borderWidth = 2;

public:
    //--------------------------------------------------------------
    // Constructor
    PatternSwirl(uint8_t w, uint8_t h) : Drawable(MATRIX_PATT_SWIRL, w, h) {}

    /* drawFrame()
     * Primary method that draws the next frame
     */
    unsigned int drawFrame(GFXcanvas canvas)
    {
        // Apply some blurring to whatever's already on the matrix
        // Note that we never actually clear the matrix, we just constantly
        // blur it repeatedly.  Since the blurring is 'lossy', there's
        // an automatic trend toward black -- by design.
        uint8_t blurAmount = beatsin8(2, 10, 255);
        fadeToBlackBy(canvas.getBuffer(), NUM_LEDS_MATRIX, blurAmount);

        // Use two out-of-sync sine waves
        uint8_t i = beatsin8(13, borderWidth, MATRIX_HEIGHT - borderWidth);
        uint8_t j = beatsin8(20, borderWidth, MATRIX_WIDTH - borderWidth);
        // Also calculate some reflections
        uint8_t ni = (MATRIX_WIDTH - 1) - i;
        uint8_t nj = (MATRIX_WIDTH - 1) - j;

        // The color of each point shifts over time, each at a different speed.
        uint16_t ms = millis();
        canvas.drawCircle(i, j, 1, getColor(ms / 11)); // possibly use pal
        canvas.drawCircle(j, i, 1, getColor(ms / 13));
        canvas.drawCircle(ni, nj, 1, getColor(ms / 17));
        canvas.drawCircle(nj, ni, 1, getColor(ms / 29));
        canvas.drawCircle(i, nj, 1, getColor(ms / 37));
        canvas.drawCircle(ni, j, 1, getColor(ms / 41));

        return 0;
    }
};

#endif