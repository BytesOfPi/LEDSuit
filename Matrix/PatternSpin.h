/*
 * Aurora: https://github.com/pixelmatix/aurora
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

#ifndef PatternSpin_H
#define PatternSpin_H

#include "Drawable.h"
#include "MatrixUtility.h"

class PatternSpin : public Drawable
{
private:
    uint8_t fadeAmount = 190;

public:
    float degrees = 0;
    float radius = 16;

    float speedStart = 1;
    float velocityStart = 0.6;

    float maxSpeed = 30;

    float speed = speedStart;
    float velocity = velocityStart;

    //--------------------------------------------------------------
    // Constructor
    PatternSpin(uint8_t w, uint8_t h) : Drawable("PatternSpin", w, h) {}

    /* start()
     * Call when starting back up
     */
    void start(GFXcanvas canvas)
    {
        speed = speedStart;
        velocity = velocityStart;
        degrees = 0;
    }

    /* drawFrame()
     * Primary method that draws the next frame
     */
    unsigned int drawFrame(GFXcanvas canvas)
    {
        fadeToBlackBy(canvas.getBuffer(), NUM_LEDS_MATRIX, fadeAmount);

        CRGB color = getColor(pal, speed * 8);

        // start position
        int x;
        int y;
        uint8_t x1;
        uint8_t y1;

        float targetDegrees = degrees + speed;
        float targetRadians = radians(targetDegrees);
        int targetX = (int)(radius * cos(targetRadians));
        int targetY = (int)(radius * sin(targetRadians));

        float tempDegrees = degrees;

        do
        {
            float radians = radians(tempDegrees);
            x = (int)(radius * cos(radians));
            y = (int)(radius * sin(radians));

            x1 = map(x, -16, 16, 0, 16);
            y1 = map(y, -16, 16, 0, 16);

            canvas.drawPixel(x1, y1, color);
            canvas.drawPixel(y1, x1, color);

            tempDegrees += 1;
            if (tempDegrees >= 360)
                tempDegrees = 0;
        } while (x != targetX || y != targetY);

        degrees += speed;

        // add velocity to the particle each pass around the accelerator
        if (degrees >= 360)
        {
            degrees = 0;
            speed += velocity;
            if (speed <= speedStart)
            {
                speed = speedStart;
                velocity *= -1;
            }
            else if (speed > maxSpeed)
            {
                speed = maxSpeed - velocity;
                velocity *= -1;
            }
        }
        return 0;
    }
};

#endif