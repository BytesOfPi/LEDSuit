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

#ifndef MatrixScroll_H
#define MatrixScroll_H

#include "Drawable.h"

class MatrixScroll : public Drawable
{
private:
    const uint8_t START_TEXT_ROW = 5;
    uint8_t colorIndex = 0;
    String message = "";
    boolean display = false;
    int pass = 0;
    int nextX = 0;
    int cursorX = 0;

public:
    //--------------------------------------------------------------
    // Constructor
    MatrixScroll(uint8_t w, uint8_t h) : Drawable("MatrixScroll", w, h) {}

    void reset()
    {
        pass = 0;
        nextX = MATRIX_WIDTH;
    }

    void setShowText(boolean show)
    {
        if (show == true)
        {
            reset();
        }
        display = show;
    }
    boolean isShowText()
    {
        return display;
    }

    void setMsg(String msg)
    {
        message = msg;
    }
    void addMsg(String msg)
    {
        message += msg;
    }

    void writeMsg(String text, uint8_t yCoord, GFXcanvas canvas, CRGB color, boolean background = true)
    {
        //--------------------------------------------------------------
        // Set text background color
        if (background)
        {
            canvas.setTextColor(color, CRGB::Black);
        }
        else
        {
            canvas.setTextColor(color);
        }

        Serial.println(text);
        //--------------------------------------------------------------
        // Set cursor and print text
        canvas.setCursor(nextX, yCoord);
        canvas.print(text);
        cursorX = canvas.getCursorX();
    }

    boolean scrollText(String text, uint8_t yCoord, GFXcanvas canvas, CRGB color, boolean background = true)
    {
        //--------------------------------------------------------------
        // Write the text out
        this->writeMsg(text, yCoord, canvas, color, background);

        //--------------------------------------------------------------
        // Scroll the text to next position and keep track of passes
        nextX--;
        if (cursorX <= 0)
        {
            Serial.print("Moving on");
            nextX = MATRIX_WIDTH;
            pass = (pass < 2) ? pass + 1 : 0;
            return false;
        }

        return true;
    }
    /* getColor()
    * Quick method to get a color from a particular palette defaulting common values
    */
    CRGB getColor(CRGBPalette16 pal, uint8_t index = 0, uint8_t brightness = 126, TBlendType blendType = LINEARBLEND)
    // CRGB getColor(uint8_t index = 0, const CRGBPalette16 &pal = RainbowColors_p, uint8_t brightness = 126, TBlendType blendType = LINEARBLEND)
    {
        return ColorFromPalette(pal, index, brightness, blendType);
    }
    /* drawFrame()
     * Primary method that draws the next frame
     */
    unsigned int drawFrame(GFXcanvas canvas)
    {
        boolean write = true;
        //--------------------------------------------------------------
        // If not showing message... stop early
        if (!display)
            return 0;

        //--------------------------------------------------------------
        // Slow the frames down to every 75 milliseconds
        EVERY_N_MILLISECONDS(75)
        {
            colorIndex += 3;
            scrollText(message, START_TEXT_ROW, canvas, this->getColor(pal, colorIndex), true);
            write = false;
        }

        if (write)
            this->writeMsg(message, START_TEXT_ROW, canvas, this->getColor(pal, colorIndex), true);

        return 0;
    }
};

#endif