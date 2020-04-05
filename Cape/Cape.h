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

#ifndef Cape_H
#define Cape_H

#define LED_TYPE_CAPE WS2812B
#define COLOR_ORDER_CAPE GRB

#define TOT_SCROLL_CAPE 30

#define DATA_PIN_CAPE_LEFT 5
#define DATA_PIN_CAPE_RIGHT 18
#define NUM_LEDS_CAPE_LEFT 300
#define NUM_LEDS_CAPE_RIGHT 300

class Cape
{
private:
    CRGB capeLedsLeft[NUM_LEDS_CAPE_LEFT];
    CRGB capeLedsRight[NUM_LEDS_CAPE_RIGHT];
    uint8_t capeHue = 0; // rotating "base color" used by many of the patterns
    byte capeOffset = 0; // rotating offset used to push theater patterns on

public:
    Cape() {}

    void capeBPM()
    {
        // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
        uint8_t BeatsPerMinute = 62;
        uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
        for (int i = 0; i < NUM_LEDS_CAPE_LEFT; i++)
        {
            capeLedsLeft[i] = ColorFromPalette(PartyColors_p, capeHue + (i * 2), beat - capeHue + (i * 10));
            capeLedsRight[i] = capeLedsLeft[i];
        }
    }

    void periodicAdjust()
    {
        // do some periodic updates
        EVERY_N_MILLISECONDS(20) { capeHue++; } // slowly cycle the "base color" through the rainbow

        EVERY_N_MILLISECONDS(100)
        {
            capeOffset++;
            if (capeOffset == TOT_SCROLL_CAPE)
            {
                capeOffset = 0;
            }
        } // slowly push rotating offset
    }

    void setup()
    {
        FastLED.addLeds<LED_TYPE_CAPE, DATA_PIN_CAPE_LEFT, COLOR_ORDER_CAPE>(capeLedsLeft, 0, NUM_LEDS_CAPE_LEFT).setCorrection(TypicalLEDStrip);
        FastLED.addLeds<LED_TYPE_CAPE, DATA_PIN_CAPE_RIGHT, COLOR_ORDER_CAPE>(capeLedsRight, 0, NUM_LEDS_CAPE_RIGHT).setCorrection(TypicalLEDStrip);
    }

    virtual unsigned int drawFrame()
    {
        // TODO: Run capeBPM() pattern for now
        capeBPM();

        // Periodic adjustment to variables
        periodicAdjust();

        return 0;
    };
};

#endif
