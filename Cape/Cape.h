/*
 * Cape.h: https://github.com/BytesOfPi/LEDSuit
 * Copyright (c) 2020 Nathan DeGroff
 *
 * Overview:
 * This file defines and controls the Cape component.
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

#include "../Shared/Share.h"

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

//################################################################################
// Shared variable libraries
#include "../Shared/Share.h"
#include "../Shared/ComponentLED.h"

class Cape : public ComponentLED
{
private:
    CRGB capeLedsLeft[NUM_LEDS_CAPE_LEFT];
    CRGB capeLedsRight[NUM_LEDS_CAPE_RIGHT];

    Wing lWing = Wing();
    Wing rWing = Wing();

    uint8_t capeHue = 0;        // rotating "base color" used by many of the patterns
    byte capeOffset = 0;        // rotating offset used to push theater patterns on
    uint8_t startIndex = 0;

public:
    //--------------------------------------------------------------
    // Constructor
    Cape() : ComponentLED("Cape", gConfig.currentCapePattern) {}

    /**
     * setup()
     * This method initializes the LEDs and the class variables.
     */
    void setup()
    {
        // Setup LED arrays
        FastLED.addLeds<LED_TYPE_CAPE, DATA_PIN_CAPE_LEFT, COLOR_ORDER_CAPE>(capeLedsLeft, 0, NUM_LEDS_CAPE_LEFT).setCorrection(TypicalLEDStrip);
        FastLED.addLeds<LED_TYPE_CAPE, DATA_PIN_CAPE_RIGHT, COLOR_ORDER_CAPE>(capeLedsRight, 0, NUM_LEDS_CAPE_RIGHT).setCorrection(TypicalLEDStrip);

        // Setup Left and Right Wing
        int leftLens[] = {NUM_LEDS_CAPE_LEFT_01,
                          NUM_LEDS_CAPE_LEFT_02,
                          NUM_LEDS_CAPE_LEFT_03,
                          NUM_LEDS_CAPE_LEFT_04,
                          NUM_LEDS_CAPE_LEFT_05};
        int rightLens[] = {NUM_LEDS_CAPE_RIGHT_01,
                           NUM_LEDS_CAPE_RIGHT_02,
                           NUM_LEDS_CAPE_RIGHT_03,
                           NUM_LEDS_CAPE_RIGHT_04,
                           NUM_LEDS_CAPE_RIGHT_05};
        lWing.setup(capeLedsLeft, NUM_LEDS_CAPE_LEFT, leftLens, "Left");
        rWing.setup(capeLedsRight, NUM_LEDS_CAPE_RIGHT, rightLens, "Right");
    }

    /**
     * switchPattern()
     * This method is called every time there is a pattern switch.  This will allow
     * us to reset any stored values that patterns use 
     */
    virtual void switchPattern()
    {
        lWing.switchPattern();
        rWing.switchPattern();
    }

    /**
     * drawPatt()
     * Given a specific pattern string, draw the related pattern.
     */
    virtual unsigned int drawPattern(String val)
    {
        //------------------------------------------------------------
        // Draw the pattern
        if (val.equals(CAPE_PATT_LIGHTSABER)) return patternLightsaber();
        if (val.equals(CAPE_PATT_SPARKLE)) return patternSparkle();
        if (val.equals(CAPE_PATT_EXTEND)) return patternExtend();
        if (val.equals(CAPE_PATT_JUGGLE)) return patternJuggle();
        if (val.equals(CAPE_PATT_BPM)) return patternBPM();
        if (val.equals(CAPE_PATT_FIRE)) return patternFire();

        return 0;
    }

    /**
     * periodicAdjust()
     * This method defines the periodic adjustments to class variables that
     * keep the patterns moving 
     */
    virtual void periodicAdjust()
    {
        //--------------------------------------------------------------
        // Used only for palette patterns to move the pattern along strand
        startIndex = startIndex + 1; /* motion speed */

        //--------------------------------------------------------------
        // slowly cycle the "base color" through the rainbow
        EVERY_N_MILLISECONDS(20) { capeHue++; }

        //--------------------------------------------------------------
        // slowly push rotating offset
        EVERY_N_MILLISECONDS(100)
        {
            capeOffset++;
            if (capeOffset == TOT_SCROLL_CAPE)
            {
                capeOffset = 0;
            }
        }

        //--------------------------------------------------------------
        // Switch cycle() pattern every 10 seconds
        EVERY_N_SECONDS(10)
        {
            patt->cycleNext();
            printCurrentPattern();
        }
    }

    //#################################################################
    // PATTERNS
    //#################################################################
    int patternBPM()
    {
        // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
        uint8_t BeatsPerMinute = 62;
        uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
        //for (int i = 0; i < NUM_LEDS_CAPE_LEFT; i++)
        CRGB *newRGB;
        for (int i = 0; i < NUM_LEDS_CAPE_RIGHT; i++)
        {
            capeLedsRight[i] = ColorFromPalette(gConfig.currentCapePalette.pal, capeHue + (i * 2), beat - capeHue + (i * 10));
            if (i < NUM_LEDS_CAPE_LEFT)
            {
                capeLedsLeft[i] = capeLedsRight[i];
            }
        }
        return 1;
    }

    int patternJuggle()
    {
        // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
        lWing.patternJuggle();
        rWing.patternJuggle();
        return 1;
    }

    int patternSparkle()
    {
        // Sparkling field on front and back of cape
        lWing.patternPaletteConfetti(gConfig.currentCapePalette.pal, gConfig.currentCapeSecPalette.pal, LINEARBLEND);
        rWing.patternPaletteConfetti(gConfig.currentCapePalette.pal, gConfig.currentCapeSecPalette.pal, LINEARBLEND);
        return 1;
    }

    int patternExtend()
    {
        // Sparkling field on front and back of cape
        lWing.patternExtend();
        rWing.patternExtend();
        return 1;
    }

    int patternFire()
    {
        // Fire?
        lWing.patternFire();
        rWing.patternFire();
        return 1;
    }

    int patternLightsaber()
    {
        //------------------------------------------------------------
        // Fade to black fast
        fadeToBlackBy(capeLedsLeft, NUM_LEDS_CAPE_LEFT, 80);
        fadeToBlackBy(capeLedsRight, NUM_LEDS_CAPE_RIGHT, 80);

        //------------------------------------------------------------
        // Using beatsin8, get a position between -5 and 35
        // Constrain the highs and lows to 0 and 30 (stay at end longer)
        // Map that range between 0 and 10 to turn on correct sabers
        uint8_t beat = constrain((int)map(beatsin8(6, 0, 40), 40, 0, 0, 40) - 5, 0, 30);
        beat = map(beat, 0, 30, 0, 10);
        uint16_t myLeftMask = 0, myRightMask = 0;
        uint8_t minLeft = min((int)beat, 5);

        //------------------------------------------------------------
        // Turn on left veins if position between 1-5
        for (int i = 1; i <= minLeft; i++)
        {
            myLeftMask |= frontMask[i - 1];
            myLeftMask |= backMask[i - 1];
        }
        //------------------------------------------------------------
        // Turn on right veins if position between 6-10
        for (int i = 6; i <= beat; i++)
        {
            myRightMask |= frontMask[10 - i];
            myRightMask |= backMask[10 - i];
        }

        //------------------------------------------------------------
        // Draw sabers
        lWing.patternLightsaber(myLeftMask);
        rWing.patternLightsaber(myRightMask);
        return 1;
    }
};

#endif
