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

#include "Wing.h"

#define LED_TYPE_CAPE WS2812B
#define COLOR_ORDER_CAPE GRB

#define TOT_SCROLL_CAPE 30

#define DATA_PIN_CAPE_LEFT 12
#define DATA_PIN_CAPE_RIGHT 14
// #define NUM_LEDS_CAPE_LEFT 458
#define NUM_LEDS_CAPE_LEFT 458
#define NUM_LEDS_CAPE_RIGHT 508

#define NUM_LEDS_CAPE_LEFT_01 54
#define NUM_LEDS_CAPE_LEFT_02 49
#define NUM_LEDS_CAPE_LEFT_03 42
#define NUM_LEDS_CAPE_LEFT_04 40
#define NUM_LEDS_CAPE_LEFT_05 44

#define NUM_LEDS_CAPE_RIGHT_01 58
#define NUM_LEDS_CAPE_RIGHT_02 54
#define NUM_LEDS_CAPE_RIGHT_03 50
#define NUM_LEDS_CAPE_RIGHT_04 46
#define NUM_LEDS_CAPE_RIGHT_05 46

#define NUM_CAPE_PATTERNS 5

class Cape
{
private:
    CRGB capeLedsLeft[NUM_LEDS_CAPE_LEFT];
    CRGB capeLedsRight[NUM_LEDS_CAPE_RIGHT];

    Wing lWing = Wing();
    Wing rWing = Wing();

    uint8_t capeHue = 0; // rotating "base color" used by many of the patterns
    byte capeOffset = 0; // rotating offset used to push theater patterns on
    uint8_t capePatt = 0; // determine which pattern
    uint8_t startIndex = 0;

public:
    Cape() {}

    void switchPattern()
    {
        capePatt = (capePatt == NUM_CAPE_PATTERNS - 1 ) ? 0 : capePatt + 1;
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

        EVERY_N_SECONDS(10) { capePatt = (capePatt == NUM_CAPE_PATTERNS - 1 ) ? 0 : capePatt + 1; }
    }


    void setup()
    {
        // Setup LED arrays
        FastLED.addLeds<LED_TYPE_CAPE, DATA_PIN_CAPE_LEFT, COLOR_ORDER_CAPE>(capeLedsLeft, 0, NUM_LEDS_CAPE_LEFT).setCorrection(TypicalLEDStrip);
        FastLED.addLeds<LED_TYPE_CAPE, DATA_PIN_CAPE_RIGHT, COLOR_ORDER_CAPE>(capeLedsRight, 0, NUM_LEDS_CAPE_RIGHT).setCorrection(TypicalLEDStrip);
        
        // Setup Left and Right Wing
        int leftLens[] = { NUM_LEDS_CAPE_LEFT_01,
                        NUM_LEDS_CAPE_LEFT_02,
                        NUM_LEDS_CAPE_LEFT_03,
                        NUM_LEDS_CAPE_LEFT_04,
                        NUM_LEDS_CAPE_LEFT_05 };
        int rightLens[] = { NUM_LEDS_CAPE_RIGHT_01,
                        NUM_LEDS_CAPE_RIGHT_02,
                        NUM_LEDS_CAPE_RIGHT_03,
                        NUM_LEDS_CAPE_RIGHT_04,
                        NUM_LEDS_CAPE_RIGHT_05 };           
        lWing.setup( capeLedsLeft, NUM_LEDS_CAPE_LEFT, leftLens );
        rWing.setup( capeLedsRight, NUM_LEDS_CAPE_RIGHT, rightLens );
    }

    virtual unsigned int drawFrame()
    {
        // Used only for palette patterns to move the pattern along strand
        startIndex = startIndex + 1; /* motion speed */

        // TODO: Run capeBPM() pattern for now
        switch( capePatt )
        {
            case 0: patternFire(); break;
            case 1: patternSparkle(); break;
            case 2: patternLightsaber(); break;
            case 3: patternJuggle(); break;
            case 4: patternBPM(); break;
        }

        // Periodic adjustment to variables
        periodicAdjust();

        return 0;
    };

    void patternBPM()
    {
        // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
        uint8_t BeatsPerMinute = 62;
        uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
        //for (int i = 0; i < NUM_LEDS_CAPE_LEFT; i++)
        CRGB* newRGB; 
        for (int i = 0; i < NUM_LEDS_CAPE_RIGHT; i++)
        {
            capeLedsRight[i] = ColorFromPalette(PartyColors_p, capeHue + (i * 2), beat - capeHue + (i * 10));
            if ( i < NUM_LEDS_CAPE_LEFT )
            {
            capeLedsLeft[i] = capeLedsRight[i];
            }
        }
    }

    void patternJuggle()
    {
        // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
        lWing.patternJuggle();
        rWing.patternJuggle();
    }

    void patternSparkle()
    {
        // Sparkling field on front and back of cape
        lWing.patternPaletteConfetti(ForestColors_p, OceanColors_p, LINEARBLEND);
        rWing.patternPaletteConfetti(ForestColors_p, OceanColors_p, LINEARBLEND);
    }

    void patternLightsaber()
    {
        // Sparkling field on front and back of cape
        lWing.patternLightsaber();
        rWing.patternLightsaber();
    }

    void patternFire()
    {
        // Fire?
        lWing.patternFire();
        rWing.patternFire();
    }


};

#endif
