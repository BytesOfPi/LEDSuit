/*
 * PatternWave.h: https://github.com/BytesOfPi/LEDSuit
 * Copyright (c) 2020 Nathan DeGroff
 * 
 * Overview:
 * This Drawable class defines and draws the matrix component wave pattern.
 * The matrix will draw 1 or 2 wave patterns that flow from one side of the matrix
 * to the other.
 * 
 * Portions of this code are adapted from Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
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

#ifndef PatternWave_H
#define PatternWave_H

#include "Drawable.h"
#include "MatrixUtility.h"

class PatternWave : public Drawable
{
private:
    byte thetaUpdate = 0;
    byte thetaUpdateFrequency = 0;
    byte theta = 0;

    byte hueUpdate = 0;
    byte hueUpdateFrequency = 0;
    byte hue = 0;

    byte rotation = 0;

    uint8_t scale = 256 / MATRIX_WIDTH;

    uint8_t maxX = MATRIX_WIDTH - 1;
    uint8_t maxY = MATRIX_HEIGHT - 1;

    uint8_t waveCount = 1;

public:
    //--------------------------------------------------------------
    // Constructor
    PatternWave(uint8_t w, uint8_t h) : Drawable(MATRIX_PATT_WAVE, w, h) {}

    void shiftPattern()
    {
        random16_add_entropy(random16());
        rotation = random(0, 4);
        waveCount = random(1, 3);
    }

    /* start()
     * Call when starting back up
     */
    void start(GFXcanvas canvas)
    {
        shiftPattern();
    }

    /* drawFrame()
     * Primary method that draws the next frame
     */
    unsigned int drawFrame(GFXcanvas canvas)
    {
        int n = 0;
        fadeToBlackBy(canvas.getBuffer(), NUM_LEDS_MATRIX, 254);

        EVERY_N_SECONDS(7)
        {
            shiftPattern();
        }

        switch (rotation)
        {
        case 0:
            for (int x = 0; x < MATRIX_WIDTH; x++)
            {
                n = quadwave8(x * 2 + theta) / scale;

                canvas.drawPixel(x, n, getColor(pal, x + hue));
                if (waveCount == 2)
                    canvas.drawPixel(x, maxY - n, getColor(pal, x + hue));
            }
            break;

        case 1:
            for (int y = 0; y < MATRIX_HEIGHT; y++)
            {
                n = quadwave8(y * 2 + theta) / scale;

                canvas.drawPixel(n, y, getColor(pal, y + hue));
                if (waveCount == 2)
                    canvas.drawPixel(maxX - n, y, getColor(pal, y + hue));
            }
            break;

        case 2:
            for (int x = 0; x < MATRIX_WIDTH; x++)
            {
                n = quadwave8(x * 2 - theta) / scale;

                canvas.drawPixel(x, n, getColor(pal, x + hue));
                if (waveCount == 2)
                    canvas.drawPixel(x, maxY - n, getColor(pal, x + hue));
            }
            break;

        case 3:
            for (int y = 0; y < MATRIX_HEIGHT; y++)
            {
                n = quadwave8(y * 2 - theta) / scale;

                canvas.drawPixel(n, y, getColor(pal, y + hue));
                if (waveCount == 2)
                    canvas.drawPixel(maxX - n, y, getColor(pal, y + hue));
            }
            break;
        }

        if (thetaUpdate >= thetaUpdateFrequency)
        {
            thetaUpdate = 0;
            theta++;
        }
        else
        {
            thetaUpdate++;
        }

        if (hueUpdate >= hueUpdateFrequency)
        {
            hueUpdate = 0;
            hue++;
        }
        else
        {
            hueUpdate++;
        }

        return 0;
    }
};

#endif