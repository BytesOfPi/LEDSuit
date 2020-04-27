/*
 * Suit.h: https://github.com/BytesOfPi/LEDSuit
 * Copyright (c) 2020 Nathan DeGroff
 *
 * Overview:
 * This file defines drawing the actual suit LED patterns.  The setup() method is called
 * during the Arduino setup() to initialize the FastLED library and setup the class.  The
 * drawFrame() method controls drawing the current pattern.
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

#define LED_TYPE_SUIT WS2812B
#define COLOR_ORDER GRB

#define DATA_PIN_SUIT_001 4
#define DATA_PIN_SUIT_002 16
#define DATA_PIN_SUIT_003 17

#define NUM_LEDS_SUIT_001 75
#define NUM_LEDS_SUIT_002 150
#define NUM_LEDS_SUIT_003 75

#define LED_INDEX_001 NUM_LEDS_SUIT_001
#define LED_INDEX_002 NUM_LEDS_SUIT_001 + NUM_LEDS_SUIT_002
#define LED_INDEX_003 LED_INDEX_002 + NUM_LEDS_SUIT_003
#define NUM_LEDS 300

#define BRIGHTNESS 96
#define FRAMES_PER_SECOND 120

#define TOT_SCROLL 30

//################################################################################
// Shared variable libraries
#include "../Shared/Share.h"
#include "../Shared/ComponentLED.h"

#include "SuitCommon.h"

/**
 * Suit Class
 * This class controls drawing to the LED suit (arms and legs) on the outfit.
 */
class Suit : public ComponentLED
{
private:
    uint8_t SUIT_STD_COLOR_INDEX_INC = 3;
    uint8_t SUIT_STD_BRIGHTNESS = 175;
    CRGB suitLeds[NUM_LEDS];
    uint8_t suitHue = 0; // rotating "base color" used by many of the patterns
    byte suitOffset = 0; // rotating suitOffset used to push theater patterns on

    // Used for palette style patterns
    uint8_t startIndex = 0;

public:
    //--------------------------------------------------------------
    // Constructor
    Suit() : ComponentLED("Suit", gConfig.currentSuitPattern) {}

    /**
     * setup()
     * This method initializes the LEDs and the class variables.
     */
    void setup()
    {
        // tell FastLED there 3 data pins,
        //    first set starts at 0 and goes NUM_LEDS_SUIT_001 length
        //    first set starts at NUM_LEDS_SUIT_001 and goes NUM_LEDS_SUIT_002 length
        //    first set starts at NUM_LEDS_SUIT_001 + NUM_LEDS_SUIT_002 and goes NUM_LEDS_SUIT_003 length
        FastLED.addLeds<LED_TYPE_SUIT, DATA_PIN_SUIT_001, COLOR_ORDER>(suitLeds, 0, NUM_LEDS_SUIT_001).setCorrection(TypicalLEDStrip);
        FastLED.addLeds<LED_TYPE_SUIT, DATA_PIN_SUIT_002, COLOR_ORDER>(suitLeds, NUM_LEDS_SUIT_001, NUM_LEDS_SUIT_002).setCorrection(TypicalLEDStrip);
        FastLED.addLeds<LED_TYPE_SUIT, DATA_PIN_SUIT_003, COLOR_ORDER>(suitLeds, NUM_LEDS_SUIT_001 + NUM_LEDS_SUIT_002, NUM_LEDS_SUIT_003).setCorrection(TypicalLEDStrip);

        // set master brightness control
        FastLED.setBrightness(BRIGHTNESS);
    }

    /**
     * switchPattern()
     * This method is called every time there is a pattern switch.  This will allow
     * us to reset any stored values that patterns use 
     */
    virtual void switchPattern()
    {
        printCurrentPattern();
    }

    /**
     * drawPattern()
     * Given a specific pattern string, draw the related pattern.
     */
    virtual unsigned int drawPattern(String val)
    {
        //--------------------------------------------------------------
        // Palettes
        if (val.equals("paletteCCHS"))
            return paletteMove(myCCHS1Palette_p);
        if (val.equals("paletteCCHS2"))
            return paletteMove(myCCHS2Palette_p);
        if (val.equals("paletteRWB"))
            return paletteMove(CloudColors_p);
        if (val.equals("paletteCloud"))
            return paletteMove(myRedWhiteBluePalette_p);
        if (val.equals("paletteLava"))
            return paletteMove(LavaColors_p);
        if (val.equals("paletteHeat"))
            return paletteMove(HeatColors_p);
        if (val.equals("paletteOcean"))
            return paletteMove(OceanColors_p);
        if (val.equals("paletteForest"))
            return paletteMove(ForestColors_p);
        if (val.equals("paletteChristmas"))
            return paletteMove(myChristmasPalette_p);
        if (val.equals("palletteCCHSGlitter"))
        {
            paletteMove(myCCHS2Palette_p);
            addGlitter(suitLeds, NUM_LEDS, 80);
            return 1;
        }
        //--------------------------------------------------------------
        // Miscellaneous
        if (val.equals("twoSplit"))
            return twoSplit();
        if (val.equals("juggle"))
            return juggle();
        if (val.equals("confetti"))
            return confetti();

        //--------------------------------------------------------------
        // BPM
        if (val.equals("bpmParty"))
            return bpmPulse(PartyColors_p);
        if (val.equals("bpmRWB"))
            return bpmPulse(myRedWhiteBluePalette_p);
        if (val.equals("bpmChristmas"))
            return bpmPulse(myChristmasPalette_p);

        //--------------------------------------------------------------
        // Solid
        if (val.equals("solidBlue"))
            return solidBlue();
        if (val.equals("solidRed"))
            return solidRed();
        if (val.equals("solidGreen"))
            return solidGreen();

        //--------------------------------------------------------------
        // Theater
        if (val.equals("theaterCCHS"))
            return theaterCCHS();
        if (val.equals("theaterChristmas"))
            return theaterChristmas();

        //--------------------------------------------------------------
        // Mixins
        if (val.equals("mixinsCCHS"))
            return mixinsCCHS();
        if (val.equals("mixinsLava"))
            return mixinsLava();
        if (val.equals("mixSpecLava"))
            return mixSpecLava();
        if (val.equals("mixSpecOcean"))
            return mixSpecOcean();
        if (val.equals("mixSpecForest"))
            return mixSpecForest();

        return 0;
    }

    virtual void periodicAdjust()
    {
        // Used only for palette patterns to move the pattern along strand
        startIndex = startIndex + 1; /* motion speed */

        // do some periodic updates
        EVERY_N_MILLISECONDS(20) { suitHue++; } // slowly cycle the "base color" through the rainbow

        EVERY_N_MILLISECONDS(100)
        {
            suitOffset++;
            if (suitOffset == TOT_SCROLL)
            {
                suitOffset = 0;
            }
        } // slowly push rotating suitOffset

        //--------------------------------------------------------------
        // Switch cycle() pattern every 10 seconds
        EVERY_N_SECONDS(10)
        {
            patt->cycleNext();
            printCurrentPattern();
        }
    }

    /*
     * mixins()
     * chasers on legs, but pallet on shoulder
     */
    int mixins(CRGBPalette16 pal, TBlendType blendType, uint8_t colorIndex)
    {
        fadeToBlackBy(suitLeds, NUM_LEDS, 90);

        // Shoulder pallete
        for (int i = LED_INDEX_001; i < LED_INDEX_002; i++)
        {
            suitLeds[i] = ColorFromPalette(pal, colorIndex, SUIT_STD_BRIGHTNESS, blendType);
            colorIndex += SUIT_STD_COLOR_INDEX_INC;
        }

        CRGB pals[8] = {ColorFromPalette(pal, 0, SUIT_STD_BRIGHTNESS, NOBLEND),
                        ColorFromPalette(pal, 16, SUIT_STD_BRIGHTNESS, NOBLEND),
                        ColorFromPalette(pal, 32, SUIT_STD_BRIGHTNESS, NOBLEND),
                        ColorFromPalette(pal, 48, SUIT_STD_BRIGHTNESS, NOBLEND),
                        ColorFromPalette(pal, 64, SUIT_STD_BRIGHTNESS, NOBLEND),
                        ColorFromPalette(pal, 80, SUIT_STD_BRIGHTNESS, NOBLEND),
                        ColorFromPalette(pal, 96, SUIT_STD_BRIGHTNESS, NOBLEND),
                        ColorFromPalette(pal, 112, SUIT_STD_BRIGHTNESS, NOBLEND)};
        // Chasers on the legs
        byte dothue = 0;
        for (int i = 0; i < 8; i++)
        {
            suitLeds[beatsin16(24, 0, LED_INDEX_001 - 1, i * 20)] = pals[i];
            suitLeds[beatsin16(24, LED_INDEX_002, LED_INDEX_003 - 1, i * 20)] = pals[i];
            dothue += 32;
        }
        return 1;
    }

    int mixSpec(CRGBPalette16 pal, TBlendType blendType, uint8_t colorIndex)
    {
        uint8_t brightness = 175;
        // Shoulder pallete
        for (int i = LED_INDEX_001; i < LED_INDEX_002; i++)
        {
            suitLeds[i] = ColorFromPalette(pal, colorIndex, SUIT_STD_BRIGHTNESS, blendType);
            colorIndex += SUIT_STD_COLOR_INDEX_INC;
        }

        CRGB pals[8] = {ColorFromPalette(pal, 0, SUIT_STD_BRIGHTNESS, NOBLEND),
                        ColorFromPalette(pal, 16, SUIT_STD_BRIGHTNESS, NOBLEND),
                        ColorFromPalette(pal, 32, SUIT_STD_BRIGHTNESS, NOBLEND),
                        ColorFromPalette(pal, 48, SUIT_STD_BRIGHTNESS, NOBLEND),
                        ColorFromPalette(pal, 64, SUIT_STD_BRIGHTNESS, NOBLEND),
                        ColorFromPalette(pal, 80, SUIT_STD_BRIGHTNESS, NOBLEND),
                        ColorFromPalette(pal, 96, SUIT_STD_BRIGHTNESS, NOBLEND),
                        ColorFromPalette(pal, 112, SUIT_STD_BRIGHTNESS, NOBLEND)};
        // random colored speckles that blink in and fade smoothly
        fadeToBlackBy(suitLeds, NUM_LEDS, 10);
        int pos1 = random16(NUM_LEDS_SUIT_001);
        int pos2 = random16(NUM_LEDS_SUIT_001);
        suitLeds[pos1] = pals[random8(8)];
        suitLeds[pos2 + LED_INDEX_002] = pals[random8(8)];
        return 1;
    }

    int fillLEDsFromPaletteColors(CRGBPalette16 pal, TBlendType blendType, uint8_t startInd)
    {
        fill_palette(suitLeds, NUM_LEDS, startInd, SUIT_STD_COLOR_INDEX_INC, pal, SUIT_STD_BRIGHTNESS, blendType);
        return 1;
    }

    //#################################################################
    // Solid non moving patterns
    int solidRed()
    {
        // FastLED's built-in rainbow generator
        fill_solid(suitLeds, NUM_LEDS, CHSV(HUE_RED, 200, 200));
        return 1;
    }
    int solidBlue()
    {
        // FastLED's built-in rainbow generator
        fill_solid(suitLeds, NUM_LEDS, CHSV(HUE_BLUE, 255, 255));
        return 1;
    }

    int solidGreen()
    {
        // FastLED's built-in rainbow generator
        fill_solid(suitLeds, NUM_LEDS, CHSV(HUE_GREEN, 255, 255));
        return 1;
    }

    int rainbowSolid()
    {
        // FastLED's built-in rainbow generator
        fill_rainbow(suitLeds, NUM_LEDS, 128, 1);
        return 1;
    }

    //#################################################################
    // Patterns with slight changes
    /*
     * Rainbow pallet with the hue of the rainbow moving slightly
     */
    int rainbow()
    {
        // FastLED's built-in rainbow generator
        fill_rainbow(suitLeds, NUM_LEDS, suitHue, 7);
        return 1;
    }

    /*
     * Same as rainbow(), but adds random sparkles
     */
    int rainbowWithGlitter()
    {
        // built-in FastLED rainbow, plus some random sparkly glitter
        rainbow();
        return addGlitter(suitLeds, NUM_LEDS, 80);
    }

    /*
     * Color changing spots randomly appear
     */
    int confetti()
    {
        // random colored speckles that blink in and fade smoothly
        fadeToBlackBy(suitLeds, NUM_LEDS, 10);
        int pos = random16(NUM_LEDS);
        suitLeds[pos] += CHSV(suitHue + random8(64), 200, 255);
        return 1;
    }
    //#################################################################
    // Chasers
    /*
    * sinelon()
    * 2 dots closely chase each other back and forth
    */
    int sinelon()
    {
        fadeToBlackBy(suitLeds, NUM_LEDS, 80); //20
        int pos = chaserCorrect(beatsin16(6, 0, NUM_LEDS - 1));
        suitLeds[pos] += CHSV(suitHue, 255, 192);

        // second chaser
        pos = chaserCorrect(beatsin16(6, 0, NUM_LEDS - 1, 50));
        suitLeds[pos] += CHSV(HUE_AQUA, 255, 192);
        return 1;
    }

    /*
    * twoSplit()
    * Small red and blue chaser criss cross at center
    */
    int twoSplit()
    {
        // a colored dot sweeping back and forth, with fading trails
        fadeToBlackBy(suitLeds, NUM_LEDS, 110); //20
        uint16_t halfway = NUM_LEDS / 2;
        int pos1 = beatsin16(6, 0, NUM_LEDS - 1);
        int pos2 = chaserCorrect(NUM_LEDS - 1 - pos1);
        pos1 = chaserCorrect(pos1);
        suitLeds[pos1] += CHSV(HUE_BLUE, 200, 192);
        suitLeds[pos2] += CHSV(HUE_RED, 200, 192);
        return 1;
    }

    /*
    * juggle()
    * 8 color changing dots closely chase each other back and forth
    */
    int juggle()
    {
        byte dothue = 0;
        fadeToBlackBy(suitLeds, NUM_LEDS, 90); //20
        for (int i = 0; i < 8; i++)
        {
            suitLeds[chaserCorrect(beatsin16(8, 0, NUM_LEDS - 1, i * 20))] |= CHSV(dothue, 200, 255);
            dothue += 32;
        }
        return 1;
    }
    //#################################################################
    // Pulsing
    /*
    * bpmPulse()
    * Pulsing colors
    */
    int bpmPulse(CRGBPalette16 pal)
    {
        return bpm(suitLeds, NUM_LEDS, suitHue, pal);
    }
    //#################################################################
    // Moving pallets
    /*
    * paletteMove()
    * Display moving palette
    */
    int paletteMove(CRGBPalette16 pal)
    {
        return fillLEDsFromPaletteColors(pal, LINEARBLEND, startIndex);
    }

    //#################################################################
    // Theater scroll
    /*
    * theaterCCHS()
    * Display CCHS theater scroll bars
    */
    int theaterCCHS()
    {
        theater(suitLeds, NUM_LEDS, suitOffset, myCCHS1Palette_p);
    }
    /*
    * theaterChristmas()
    * Display Christmas theater scroll bars
    */
    int theaterChristmas()
    {
        return theater(suitLeds, NUM_LEDS, suitOffset, myChristmasPalette_p);
    }
    int mixinsCCHS()
    {
        return mixins(myCCHS1Palette_p, LINEARBLEND, startIndex);
    }
    int mixinsLava()
    {
        return mixins(LavaColors_p, LINEARBLEND, startIndex);
    }
    int mixSpecLava()
    {
        return mixSpec(LavaColors_p, LINEARBLEND, startIndex);
    }
    int mixSpecOcean()
    {
        return mixSpec(OceanColors_p, LINEARBLEND, startIndex);
    }
    int mixSpecForest()
    {
        return mixSpec(ForestColors_p, LINEARBLEND, startIndex);
    }

};

#endif