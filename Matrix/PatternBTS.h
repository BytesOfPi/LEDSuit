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

#ifndef PatternBTS_H
#define PatternBTS_H

#include "Drawable.h"
#include "MatrixUtility.h"

#define BTS_LOGO_CNT 5
static const uint8_t PROGMEM                   // Bitmaps are stored in program memory
    btsLogoBmp[][32] = {{B00010000, B00001000, // BTS Logo
                         B00011000, B00011000,
                         B00011100, B00111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011100, B00111000,
                         B00011000, B00011000,
                         B00010000, B00001000},
                        {B00000000, B00000000,
                         B00011000, B00011000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011000, B00011000,
                         B00000000, B00000000},
                        {B00000000, B00000000,
                         B00001100, B00110000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00001100, B00110000,
                         B00000000, B00000000},
                        {B00000000, B00000000,
                         B00000110, B01100000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00000110, B01100000,
                         B00000000, B00000000},
                        {B00000010, B01000000, // BTS Army Logo
                         B00000110, B01100000,
                         B00001110, B01110000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00011110, B01111000,
                         B00001110, B01110000,
                         B00000110, B01100000,
                         B00000010, B01000000}};

#define BTS_NAME_MAX 8
static const String btsMembers[] = {
    "BTS",
    "KIM NAMJOON",
    "KIM SEOKJIN",
    "MIN YOONGI",
    "JUNG HOSEOK",
    "PARK JIMIN",
    "KIM TAEHYUNG",
    "JEON JUNGKOOK"};

class PatternBTS : public Drawable
{
private:
    const uint8_t START_TEXT_ROW = 5;
    uint8_t btsMemIndex = 0;
    uint8_t colorIndex = 0;

public:
    //--------------------------------------------------------------
    // Constructor
    PatternBTS(uint8_t w, uint8_t h) : Drawable(MATRIX_PATT_BTS, w, h) {}

    /* btsLogo()
     * Draw the morphing BTS Logo to BTS Army Logo
     */
    void btsLogo(GFXcanvas canvas)
    {
        int16_t index = beatsin16(10, 0, BTS_LOGO_CNT - 1);
        canvas.drawBitmap(0, 0, (const uint8_t *)btsLogoBmp[index],
                          MATRIX_WIDTH, MATRIX_HEIGHT, CHSV(HUE_BLUE, 255, 125));
    }

    /* btsNames()
     * Scroll the band member names
     */
    void btsNames(GFXcanvas canvas)
    {
        //--------------------------------------------------------------
        // If we are currently scrolling a message... don't bother...
        if (scroll.isShowText())
            return;
        //--------------------------------------------------------------
        // If we hit the end of one member, move the index to the next
        colorIndex += 3;
        if (!scroll.scrollText(btsMembers[btsMemIndex], START_TEXT_ROW, canvas,
                               getColor(pal, colorIndex)))
        {
            btsMemIndex++;
            if (btsMemIndex >= BTS_NAME_MAX)
            {
                btsMemIndex = 0;
            }
        }
    }

    /* start()
     * Call when starting back up
     */
    void start(GFXcanvas canvas)
    {
        btsMemIndex = 0;
        //--------------------------------------------------------------
        // If we are not currently scrolling a message, reset cursor
        if (!scroll.isShowText())
            scroll.reset();
    }

    /* drawFrame()
     * Primary method that draws the next frame
     */
    unsigned int drawFrame(GFXcanvas canvas)
    {
        //--------------------------------------------------------------
        // Slow the frames down to every 50 milliseconds
        EVERY_N_MILLISECONDS(50)
        {
            canvas.fillScreen(CRGB::Black);
            btsLogo(canvas);
            btsNames(canvas);
        }
        return 0;
    }
};
#endif