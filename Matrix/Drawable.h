/*
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

#ifndef Drawable_H
#define Drawable_H

class Drawable
{
public:
    String name;
    uint8_t MATRIX_WIDTH; // or X
    uint8_t MATRIX_HEIGHT; // or Y
    uint8_t MATRIX_CENTER_X;
    uint8_t MATRIX_CENTER_Y;
    int NUM_LEDS_MATRIX;
    CRGBPalette16 pal;

    Drawable(String n, uint8_t w, uint8_t h)
    {
        name = n;
        //--------------------------------------------------------------
        // Store Matrix Data
        MATRIX_WIDTH = w;
        MATRIX_HEIGHT = h;
        MATRIX_CENTER_X = w / 2;
        MATRIX_CENTER_Y = h / 2;
        NUM_LEDS_MATRIX = w * h;

        pal = RainbowColors_p;
    }

    virtual bool isRunnable()
    {
        return false;
    }

    virtual bool isPlaylist()
    {
        return false;
    }

    virtual void setPalette(CRGBPalette16& newPal)
    {
        pal = newPal;
    }

    // a single frame should be drawn as fast as possible, without any delay or blocking
    // return how many millisecond delay is requested before the next call to drawFrame()
    virtual unsigned int drawFrame(GFXcanvas canvas)
    {
        canvas.fillScreen(CRGB::Black);
        return 0;
    };

    virtual void start(GFXcanvas canvas){};
    virtual void stop(GFXcanvas canvas){};
};

#endif
