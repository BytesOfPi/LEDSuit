/*
 * 
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

#ifndef Suit_H
#define Suit_H

class Suit
{
private:
    CRGB leds;
    uint8_t gHue = 0; // rotating "base color" used by many of the patterns
    byte offset = 0;  // rotating offset used to push theater patterns on

    // Used for palette style patterns
    uint8_t startIndex = 0;
    byte currentPalPattern;
    CRGBPalette16 currentPalette;
    CRGBPalette16 targetPalette;
    TBlendType currentBlending = LINEARBLEND;

public:
    void setup()
    {
        FastLED.addLeds<LED_TYPE, DATA_PIN_001, COLOR_ORDER>(leds, 0, NUM_LEDS_001).setCorrection(TypicalLEDStrip);
        FastLED.addLeds<LED_TYPE, DATA_PIN_002, COLOR_ORDER>(leds, NUM_LEDS_001, NUM_LEDS_002).setCorrection(TypicalLEDStrip);
        FastLED.addLeds<LED_TYPE, DATA_PIN_003, COLOR_ORDER>(leds, NUM_LEDS_001 + NUM_LEDS_002, NUM_LEDS_003).setCorrection(TypicalLEDStrip);
    }

    virtual unsigned int drawFrame(GFXcanvas canvas)
    {
        // Used only for palette patterns to move the pattern along strand
        startIndex = startIndex + 1; /* motion speed */

        // Make PATTERN_NUMBER safe for array
        safePattern();

        // TODO: FIX ME
        bpmTemp();

        // Call the current pattern function once, updating the 'leds' array
        gPatterns[PATTERN_NUMBER]();

        // do some periodic updates
        EVERY_N_MILLISECONDS(20) { gHue++; } // slowly cycle the "base color" through the rainbow

        EVERY_N_MILLISECONDS(100)
        {
            offset++;
            if (offset == TOT_SCROLL)
            {
                offset = 0;
            }
        } // slowly push rotating offset
        return 0;
    };
};

#endif