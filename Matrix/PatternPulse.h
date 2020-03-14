/*
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 * 
 * Portions of this code are adapted for LED Costume
 * Copyright (c) 2020 Nathan DeGroff
 *
 * Based at least in part on someone else's work that I can no longer find.
 * Please let me know if you recognize any of this code!
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

#ifndef PatternPulse_H
#define PatternPulse_H

#include "Drawable.h"
#include "MatrixUtility.h"

class PatternPulse : public Drawable
{
private:
    uint8_t fadeAmount = 235;

private:
    uint8_t hue;
    int centerX = 0;
    int centerY = 0;
    int step = -1;
    int maxSteps = 16;
    float fadeRate = 0.8;
    int diff;

public:
    //--------------------------------------------------------------
    // Constructor
    PatternPulse(uint8_t w, uint8_t h) : Drawable("PatternPulse", w, h) {}

    /* drawFrame()
     * Primary method that draws the next frame
     */
    unsigned int drawFrame(GFXcanvas canvas)
    {
        //--------------------------------------------------------------
        // Slow the frames down to every 30 milliseconds
        EVERY_N_MILLISECONDS(30)
        {
            random16_add_entropy(random16());
            //--------------------------------------------------------------
            // Fade the drawn pixels to black if they don't get redrawn
            fadeToBlackBy(canvas.getBuffer(), NUM_LEDS_MATRIX, fadeAmount);

            //--------------------------------------------------------------
            // Pick a random starting point and color for a pulse
            if (step == -1)
            {
                centerX = random(MATRIX_WIDTH);
                centerY = random(MATRIX_HEIGHT);
                hue = random(256); // 170;
                step = 0;
            }

            //--------------------------------------------------------------
            // Start the pulse
            if (step == 0)
            {
                canvas.drawCircle(centerX, centerY, step, getColor(pal, hue));
                step++;
            }
            //--------------------------------------------------------------
            // Fade the pulse out from the center
            else
            {
                if (step < maxSteps)
                {
                    CRGB colorPulse = getColor(pal, hue, pow(fadeRate, step - 2) * 255);
                    // initial pulse
                    canvas.drawCircle(centerX, centerY, step, colorPulse);

                    // secondary pulse
                    if (step > 3)
                    {
                        canvas.drawCircle(centerX, centerY, step - 3, colorPulse);
                    }
                    step++;
                }
                else
                {
                    step = -1;
                }
            }
        }
        return 30;
    }
};

#endif