/*
 * PatternSpiro.h: https://github.com/BytesOfPi/LEDSuit
 * Copyright (c) 2020 Nathan DeGroff
 * 
 * Overview:
 * This Drawable class defines and draws the matrix component spiro pattern.
 * It draws dots with trails spinning out from the center and back in.  each time
 * the dot(s) come to the center, they either increase or decrease in number.  The
 * dot's color changes in the course of its flight based on the chosen palette.
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

#ifndef PatternSpiro_H
#define PatternSpiro_H

#include "Drawable.h"
#include "MatrixUtility.h"

class PatternSpiro : public Drawable
{
private:
    byte theta1 = 0;
    byte theta2 = 0;
    byte hueoffset = 0;

    uint8_t radiusx;
    uint8_t radiusy;
    uint8_t minx;
    uint8_t maxx;
    uint8_t miny;
    uint8_t maxy;

    uint8_t spirocount = 1;
    uint8_t spirooffset = 256 / spirocount;
    boolean spiroincrement = false;

    boolean handledChange = false;

public:
    //--------------------------------------------------------------
    // Constructor
    PatternSpiro(uint8_t w, uint8_t h) : Drawable(MATRIX_PATT_SPIRO, w, h)
    {
        //--------------------------------------------------------------
        // Calculate Matrix Data
        radiusx = MATRIX_WIDTH / 4;
        radiusy = MATRIX_HEIGHT / 4;
        minx = MATRIX_CENTER_X - radiusx;
        maxx = MATRIX_CENTER_X + radiusx + 1;
        miny = MATRIX_CENTER_Y - radiusy;
        maxy = MATRIX_CENTER_Y + radiusy + 1;
    }
    /* drawFrame()
     * Primary method that draws the next frame
     */
    unsigned int drawFrame(GFXcanvas canvas)
    {
        boolean change = false;

        //--------------------------------------------------------------
        // Force LEDs to fade to black a few moments after drawn
        fadeToBlackBy(canvas.getBuffer(), NUM_LEDS_MATRIX, 3);

        //--------------------------------------------------------------
        // Calculate X,Y coordinate
        for (int i = 0; i < spirocount; i++)
        {
            uint8_t x = mapsin8(theta1 + i * spirooffset, minx, maxx);
            uint8_t y = mapcos8(theta1 + i * spirooffset, miny, maxy);

            uint8_t x2 = mapsin8(theta2 + i * spirooffset, x - radiusx, x + radiusx);
            uint8_t y2 = mapcos8(theta2 + i * spirooffset, y - radiusy, y + radiusy);

            //--------------------------------------------------------------
            // Pick color from pallette and draw pixel
            canvas.drawPixel(x2, y2, getColor(pal, hueoffset + i * spirooffset));

            //--------------------------------------------------------------
            // if pixel's back in the center, mark it ready for a change
            if ((x2 == MATRIX_CENTER_X && y2 == MATRIX_CENTER_Y))
                change = true;
        }

        theta2 += 2;

        EVERY_N_MILLISECONDS(12)
        {
            theta1 += 1;
        }

        EVERY_N_MILLISECONDS(75)
        {
            //--------------------------------------------------------------
            // If we're ready for a change, but haven't processed yet, time
            // to process
            if (change && !handledChange)
            {
                handledChange = true;

                //--------------------------------------------------------------
                // Determine if we're increasing or decreasing count of spirals
                if (spirocount >= MATRIX_WIDTH || spirocount == 1)
                    spiroincrement = !spiroincrement;

                //--------------------------------------------------------------
                // If we're increasing, either increase by 1 or double the count
                if (spiroincrement)
                {
                    if (spirocount >= 3)
                        spirocount *= 2;
                    else
                        spirocount += 1;
                }
                //--------------------------------------------------------------
                // Do the opposite if decreasing
                else
                {
                    if (spirocount > 3)
                        spirocount /= 2;
                    else
                        spirocount -= 1;
                }

                spirooffset = 256 / spirocount;
            }

            //--------------------------------------------------------------
            // Make sure we're marked to start handling change
            if (!change)
                handledChange = false;
        }

        //--------------------------------------------------------------
        // Start changing the color every 33 milliseconds
        EVERY_N_MILLISECONDS(33)
        {
            hueoffset += 1;
        }

        return 0;
    }
};

#endif
