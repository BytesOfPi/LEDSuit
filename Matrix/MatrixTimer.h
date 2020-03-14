/*
 * ???: https://github.com/???
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

#ifndef MatrixTimer_H
#define MatrixTimer_H

#include "Drawable.h"

class MatrixTimer : public Drawable
{
private:
    int minStart = 0;
    int secStart = 5;
    int minute;
    int sec;
    boolean countdown = false;
    boolean show = false;

public:
    //--------------------------------------------------------------
    // Constructor
    MatrixTimer(uint8_t w, uint8_t h) : Drawable("MatrixTimer", w, h) {}

    void setTime(int minVal, int secVal)
    {
        minStart = minVal;
        secStart = secVal;
        minute = minStart;
        sec = secStart;
    }

    boolean isShow()
    {
        return show;
    }
    void setShow( boolean setShow )
    {
        show = setShow;
    }

    boolean isCountdown()
    {
        return countdown;
    }
    void startCountdown()
    {
        minute = minStart;
        sec = secStart;
        countdown = true;
        show = true;
    }

    void pauseCountdown()
    {
        countdown = !countdown;
        show = true;
    }

    void stopCountdown()
    {
        pauseCountdown();
        show = false;
    }

    void tick()
    {
        if (!countdown)
            return;
        if (sec > 0)
        {
            sec -= 1;
        }
        else if (minute > 0)
        {
            minute -= 1;
            sec = 59;
        }
    }
    void timeUp(GFXcanvas canvas)
    {
        canvas.fillCircle(7, 7, 6, CRGB::Red);
    }

    void drawTime(GFXcanvas canvas)
    {
        canvas.setTextColor(CRGB::Blue, CRGB::Black);
        canvas.setCursor(3, 0);
        canvas.print(( minute < 10 ? "0" : "" ) + String(minute));
        canvas.setTextColor(CRGB::Green, CRGB::Black);
        canvas.setCursor(3, 7);
        canvas.print(( sec < 10 ? "0" : "" ) + String(sec));
    }
    /* drawFrame()
     * Primary method that draws the next frame
     */
    unsigned int drawFrame(GFXcanvas canvas)
    {
        //--------------------------------------------------------------
        // Slow the frames down to every second
        EVERY_N_MILLISECONDS(1000)
        {
            tick();
        }

        canvas.fillScreen(CRGB::Black);
        if (countdown && minute == 0 && sec == 0)
            timeUp(canvas);
        else
            drawTime(canvas);

        return 0;
    }
};

#endif