/*
*
* Aurora: https://github.com/pixelmatix/aurora
* Copyright (c) 2014 Jason Coon
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

#ifndef PatternIncrementalDrift_H
#define PatternIncrementalDrift_H

#include "Drawable.h"
#include "MatrixUtility.h"

class PatternIncrementalDrift : public Drawable
{
private:
    uint8_t fadeAmount = 235;
public:
    //--------------------------------------------------------------
    // Constructor
    PatternIncrementalDrift(uint8_t w, uint8_t h) : Drawable(MATRIX_PATT_DRIFT, w, h) {}

    /* drawFrame()
     * Primary method that draws the next frame
     */
    unsigned int drawFrame(GFXcanvas canvas)
    {
        //--------------------------------------------------------------
        // Force LEDs to fade to black a few moments after drawn
        //uint8_t dim = beatsin8(2, 230, 250);
        fadeToBlackBy(canvas.getBuffer(), NUM_LEDS_MATRIX, fadeAmount);

        for (int i = 2; i <= MATRIX_WIDTH / 2; i++)
        {
            CRGB color = getColor(pal, (i - 2) * (240 / (MATRIX_WIDTH / 2)));

            uint8_t x = beatcos8((MATRIX_CENTER_X + 1 - i) * 2, MATRIX_CENTER_X - i, MATRIX_CENTER_X + i);
            uint8_t y = beatsin8((MATRIX_CENTER_Y + 1 - i) * 2, MATRIX_CENTER_Y - i, MATRIX_CENTER_Y + i);

            canvas.drawCircle(x, y, 1, color);
        }

        return 0;
    }
};

#endif