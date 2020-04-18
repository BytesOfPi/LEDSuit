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

#include "SuitUtility.h"
#include "SuitCommon.h"
//#################################################################
// Pre defined custom color palettes
const TProgmemPalette16 myCCHS1Palette_p PROGMEM =
    {
        // Other purples are:  //CRGB::Indigo, CRGB::Purple, CRGB::DarkMagenta
        CRGB::Purple, CRGB::Gold, CRGB::Purple, CRGB::Gold,
        CRGB::Black, CRGB::Purple, CRGB::Gold, CRGB::Black,
        CRGB::Purple, CRGB::Gold, CRGB::Purple, CRGB::Gold,
        CRGB::Black, CRGB::Purple, CRGB::Gold, CRGB::Black};
const TProgmemPalette16 myCCHS2Palette_p PROGMEM =
    {
        // Other purples are:  //CRGB::Indigo, CRGB::Purple, CRGB::DarkMagenta
        CRGB::Purple, CRGB::Purple, CRGB::Gold, CRGB::Gold,
        CRGB::Purple, CRGB::Purple, CRGB::Gold, CRGB::Gold,
        CRGB::Purple, CRGB::Purple, CRGB::Gold, CRGB::Gold,
        CRGB::Purple, CRGB::Purple, CRGB::Gold, CRGB::Gold};
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
    {
        // Using CRGB::Gray instead of CRGB::White.  Too bright compared to red and blue
        CRGB::Red, CRGB::Gray, CRGB::Blue, CRGB::Black,
        CRGB::Red, CRGB::Gray, CRGB::Blue, CRGB::Black,
        CRGB::Red, CRGB::Gray, CRGB::Blue, CRGB::Black,
        CRGB::Red, CRGB::Gray, CRGB::Blue, CRGB::Black};
const TProgmemPalette16 myBlueGreenPalette_p PROGMEM =
    {
        CRGB::Blue, CRGB::Green, CRGB::Green,
        CRGB::Blue, CRGB::Green, CRGB::Green,
        CRGB::Blue, CRGB::Green, CRGB::Green,
        CRGB::Blue, CRGB::Green, CRGB::Green,
        CRGB::Blue, CRGB::Green, CRGB::Green,
        CRGB::Blue};
const TProgmemPalette16 myChristmasPalette_p PROGMEM =
    {
        CRGB::Gray, CRGB::Red, CRGB::Green, CRGB::SaddleBrown,
        CRGB::Gray, CRGB::Red, CRGB::Green, CRGB::SaddleBrown,
        CRGB::Gray, CRGB::Red, CRGB::Green, CRGB::SaddleBrown,
        CRGB::Gray, CRGB::Red, CRGB::Green, CRGB::SaddleBrown};

#define NUM_SKIP_CYCLES 2

class Suit
{
private:
    uint8_t SUIT_STD_COLOR_INDEX_INC = 3;
    uint8_t SUIT_STD_BRIGHTNESS = 175;
    CRGB suitLeds[NUM_LEDS];
    uint8_t suitHue = 0; // rotating "base color" used by many of the patterns
    byte suitOffset = 0; // rotating suitOffset used to push theater patterns on

    // Used for palette style patterns
    uint8_t startIndex = 0;

    byte CYCLE_PATTERN_NUMBER = 1; // Make sure not to set to skipped cycle index
    int CYCLE_MILLISECONDS = 7000; // 7 seconds
    String skipCycles[NUM_SKIP_CYCLES] = {"cycle", "lightning"};

public:

    void setup()
    {
        // tell FastLED there 3 data pins,
        //    first set starts at 0 and goes NUM_LEDS_001 length
        //    first set starts at NUM_LEDS_001 and goes NUM_LEDS_002 length
        //    first set starts at NUM_LEDS_001 + NUM_LEDS_002 and goes NUM_LEDS_003 length
        FastLED.addLeds<LED_TYPE, DATA_PIN_001, COLOR_ORDER>(suitLeds, 0, NUM_LEDS_001).setCorrection(TypicalLEDStrip);
        FastLED.addLeds<LED_TYPE, DATA_PIN_002, COLOR_ORDER>(suitLeds, NUM_LEDS_001, NUM_LEDS_002).setCorrection(TypicalLEDStrip);
        FastLED.addLeds<LED_TYPE, DATA_PIN_003, COLOR_ORDER>(suitLeds, NUM_LEDS_001 + NUM_LEDS_002, NUM_LEDS_003).setCorrection(TypicalLEDStrip);

        // set master brightness control
        FastLED.setBrightness(BRIGHTNESS);
    }

    virtual unsigned int drawFrame()
    {
        // Make PATTERN_NUMBER safe for array
        safePattern();

        // Call the current pattern function once, updating the 'suitLeds' array
        drawPattern(gPattName[PATTERN_NUMBER]);

        // Periodic adjustment to variables
        periodicAdjust();

        return 0;
    };

    void drawPattern(String patt)
    {
        if(patt.equals("cycle")) { cycle(); return; }
        if(patt.equals("paletteCCHS")) { paletteCCHS(); return; }
        if(patt.equals("paletteCCHS2")) { paletteCCHS2(); return; }
        if(patt.equals("solidCCHS")) { solidCCHS(); return; }
        if(patt.equals("palletteCCHSGlitter")) { palletteCCHSGlitter(); return; }
        if(patt.equals("twoSplit")) { twoSplit(); return; }
        if(patt.equals("juggle")) { juggle(); return; }
        if(patt.equals("confetti")) { confetti(); return; }
        if(patt.equals("paletteRWB")) { paletteRWB(); return; }
        if(patt.equals("paletteCloud")) { paletteCloud(); return; }
        if(patt.equals("paletteLava")) { paletteLava(); return; }
        if(patt.equals("paletteHeat")) { paletteHeat(); return; }
        if(patt.equals("paletteOcean")) { paletteOcean(); return; }
        if(patt.equals("paletteForest")) { paletteForest(); return; }
        if(patt.equals("paletteChristmas")) { paletteChristmas(); return; }
        if(patt.equals("bpmParty")) { bpmParty(); return; }
        if(patt.equals("bpmRWB")) { bpmRWB(); return; }
        if(patt.equals("bpmChristmas")) { bpmChristmas(); return; }
        if(patt.equals("solidBlueGreen")) { solidBlueGreen(); return; }
        if(patt.equals("solidBlue")) { solidBlue(); return; }
        if(patt.equals("solidRed")) { solidRed(); return; }
        if(patt.equals("solidGreen")) { solidGreen(); return; }
        if(patt.equals("theaterCCHS")) { theaterCCHS(); return; }
        if(patt.equals("theaterChristmas")) { theaterChristmas(); return; }
        if(patt.equals("mixinsCCHS")) { mixinsCCHS(); return; }
        if(patt.equals("mixinsLava")) { mixinsLava(); return; }
        if(patt.equals("mixSpecLava")) { mixSpecLava(); return; }
        if(patt.equals("mixSpecOcean")) { mixSpecOcean(); return; }
        if(patt.equals("mixSpecForest")) { mixSpecForest(); return; }
    }

    void periodicAdjust()
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
    }

    /*
     * mixins()
     * chasers on legs, but pallet on shoulder
     */
    void mixins(CRGBPalette16 pal, TBlendType blendType, uint8_t colorIndex)
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
    }

    void mixSpec(CRGBPalette16 pal, TBlendType blendType, uint8_t colorIndex)
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
        int pos1 = random16(NUM_LEDS_001);
        int pos2 = random16(NUM_LEDS_001);
        suitLeds[pos1] = pals[random8(8)];
        suitLeds[pos2 + LED_INDEX_002] = pals[random8(8)];
    }

    void fillLEDsFromPaletteColors(CRGBPalette16 pal, TBlendType blendType, uint8_t startInd)
    {
        fill_palette(suitLeds, NUM_LEDS, startInd, SUIT_STD_COLOR_INDEX_INC, pal, SUIT_STD_BRIGHTNESS, blendType);
    }

    //#################################################################
    // Solid non moving patterns
    void solidRed()
    {
        // FastLED's built-in rainbow generator
        fill_solid(suitLeds, NUM_LEDS, CHSV(HUE_RED, 200, 200));
    }
    void solidBlue()
    {
        // FastLED's built-in rainbow generator
        fill_solid(suitLeds, NUM_LEDS, CHSV(HUE_BLUE, 255, 255));
    }

    void solidGreen()
    {
        // FastLED's built-in rainbow generator
        fill_solid(suitLeds, NUM_LEDS, CHSV(HUE_GREEN, 255, 255));
    }

    void solidBlueGreen()
    {
        // Setting startIndex to zero keeps colors steady.
        fillLEDsFromPaletteColors(myBlueGreenPalette_p, LINEARBLEND, 0);
    }

    void solidCCHS()
    {
        fillLEDsFromPaletteColors(myCCHS1Palette_p, LINEARBLEND, 0);
    }

    void rainbowSolid()
    {
        // FastLED's built-in rainbow generator
        fill_rainbow(suitLeds, NUM_LEDS, 128, 1);
    }

    //#################################################################
    // Patterns with slight changes
    /*
     * Rainbow pallet with the hue of the rainbow moving slightly
     */
    void rainbow()
    {
        // FastLED's built-in rainbow generator
        fill_rainbow(suitLeds, NUM_LEDS, suitHue, 7);
    }

    /*
     * Same as rainbow(), but adds random sparkles
     */
    void rainbowWithGlitter()
    {
        // built-in FastLED rainbow, plus some random sparkly glitter
        rainbow();
        addGlitter(suitLeds, NUM_LEDS, 80);
    }

    /*
     * Color changing spots randomly appear
     */
    void confetti()
    {
        // random colored speckles that blink in and fade smoothly
        fadeToBlackBy(suitLeds, NUM_LEDS, 10);
        int pos = random16(NUM_LEDS);
        suitLeds[pos] += CHSV(suitHue + random8(64), 200, 255);
    }
    //#################################################################
    // Chasers
    /*
    * sinelon()
    * 2 dots closely chase each other back and forth
    */
    void sinelon()
    {
        fadeToBlackBy(suitLeds, NUM_LEDS, 80); //20
        int pos = chaserCorrect(beatsin16(6, 0, NUM_LEDS - 1));
        suitLeds[pos] += CHSV(suitHue, 255, 192);

        // second chaser
        pos = chaserCorrect(beatsin16(6, 0, NUM_LEDS - 1, 50));
        suitLeds[pos] += CHSV(HUE_AQUA, 255, 192);
    }

    /*
    * twoSplit()
    * Small red and blue chaser criss cross at center
    */
    void twoSplit()
    {
        // a colored dot sweeping back and forth, with fading trails
        fadeToBlackBy(suitLeds, NUM_LEDS, 110); //20
        uint16_t halfway = NUM_LEDS / 2;
        int pos1 = beatsin16(6, 0, NUM_LEDS - 1);
        int pos2 = chaserCorrect(NUM_LEDS - 1 - pos1);
        pos1 = chaserCorrect(pos1);
        suitLeds[pos1] += CHSV(HUE_BLUE, 200, 192);
        suitLeds[pos2] += CHSV(HUE_RED, 200, 192);
    }

    /*
    * juggle()
    * 8 color changing dots closely chase each other back and forth
    */
    void juggle()
    {
        byte dothue = 0;
        fadeToBlackBy(suitLeds, NUM_LEDS, 90); //20
        for (int i = 0; i < 8; i++)
        {
            suitLeds[chaserCorrect(beatsin16(8, 0, NUM_LEDS - 1, i * 20))] |= CHSV(dothue, 200, 255);
            dothue += 32;
        }
    }
    //#################################################################
    // Pulsing
    /*
    * bpmParty()
    * Pulsing Party colors
    */
    void bpmParty()
    {
        bpm(suitLeds, NUM_LEDS, suitHue, PartyColors_p);
    }
    /*
    * bpmRWB()
    * Pulsing Red-White-Blue
    */
    void bpmRWB()
    {
        bpm(suitLeds, NUM_LEDS, suitHue, myRedWhiteBluePalette_p);
    }
    /*
    * bpmChristmas()
    * Pulsing Christmas
    */
    void bpmChristmas()
    {
        bpm(suitLeds, NUM_LEDS, suitHue, myChristmasPalette_p);
    }
    //#################################################################
    // Moving pallets
    /*
 * paletteParty()
 * Display Party colors
 */
    void paletteParty()
    {
        fillLEDsFromPaletteColors(PartyColors_p, LINEARBLEND, startIndex);
    }

    /*
    * paletteCCHS()
    * Display CCHS colors (moving)
    */
    void paletteCCHS()
    {
        fillLEDsFromPaletteColors(myCCHS1Palette_p, LINEARBLEND, startIndex);
    }
    /*
    * palletteCCHSGlitter()
    * Display second set of CCHS colors (moving)
    */
    void paletteCCHS2()
    {
        fillLEDsFromPaletteColors(myCCHS2Palette_p, LINEARBLEND, startIndex);
    }

    /*
    * palletteCCHSGlitter()
    * Display CCHS colors with random sparkles
    */
    void palletteCCHSGlitter()
    {
        // built-in FastLED rainbow, plus some random sparkly glitter
        paletteCCHS2();
        addGlitter(suitLeds, NUM_LEDS, 80);
    }

    /*
    * paletteRWB()
    * Display Red-White-Blue palette
    */
    void paletteRWB()
    {
        fillLEDsFromPaletteColors(myRedWhiteBluePalette_p, LINEARBLEND, startIndex);
    }
    /*
    * paletteCloud()
    * Display Cloud palette
    */
    void paletteCloud()
    {
        fillLEDsFromPaletteColors(CloudColors_p, LINEARBLEND, startIndex);
    }
    /*
    * paletteLava()
    * Display Lava palette
    */
    void paletteLava()
    {
        fillLEDsFromPaletteColors(LavaColors_p, LINEARBLEND, startIndex);
    }
    /*
    * paletteHeat()
    * Display Heat palette
    */
    void paletteHeat()
    {
        fillLEDsFromPaletteColors(HeatColors_p, LINEARBLEND, startIndex);
    }
    /*
    * paletteOcean()
    * Display Ocean palette
    */
    void paletteOcean()
    {
        fillLEDsFromPaletteColors(OceanColors_p, LINEARBLEND, startIndex);
    }
    /*
    * paletteForest()
    * Display Forest palette
    */
    void paletteForest()
    {
        fillLEDsFromPaletteColors(ForestColors_p, LINEARBLEND, startIndex);
    }
    /*
    * paletteRainbowStripe()
    * Display RainbowStripe palette
    */
    void paletteRainbowStripe()
    {
        fillLEDsFromPaletteColors(RainbowStripeColors_p, LINEARBLEND, startIndex);
    }
    /*
    * paletteChristmas()
    * Display Christmas palette
    */
    void paletteChristmas()
    {
        fillLEDsFromPaletteColors(myChristmasPalette_p, LINEARBLEND, startIndex);
    }
    //#################################################################
    // Theater scroll
    /*
    * theaterCCHS()
    * Display CCHS theater scroll bars
    */
    void theaterCCHS()
    {
        theater(suitLeds, NUM_LEDS, suitOffset, myCCHS1Palette_p);
    }
    /*
    * theaterChristmas()
    * Display Christmas theater scroll bars
    */
    void theaterChristmas()
    {
        theater(suitLeds, NUM_LEDS, suitOffset, myChristmasPalette_p);
    }
    void mixinsCCHS()
    {
        mixins(myCCHS1Palette_p, LINEARBLEND, startIndex);
    }
    void mixinsLava()
    {
        mixins(LavaColors_p, LINEARBLEND, startIndex);
    }
    void mixSpecLava()
    {
        mixSpec(LavaColors_p, LINEARBLEND, startIndex);
    }
    void mixSpecOcean()
    {
        mixSpec(OceanColors_p, LINEARBLEND, startIndex);
    }
    void mixSpecForest()
    {
        mixSpec(ForestColors_p, LINEARBLEND, startIndex);
    }

    //#################################################################
    // Cycle patterns
    boolean skipCycle()
    {
        //------------------------------------------------------------
        // Loop through skip cycles
        for (int i = 0; i < NUM_SKIP_CYCLES; i++)
        {
            //------------------------------------------------------------
            // if the pattern is in list of skip cycles, return true
            if (gPattName[CYCLE_PATTERN_NUMBER].equals(skipCycles[i]))
            {
                return true;
            }
        }
        return false;
    }

    void cycle()
    {
        EVERY_N_MILLIS_I(cycleTimer, CYCLE_MILLISECONDS)
        {
            //------------------------------------------------------------
            // Cycle to the next safe pattern... if that pattern is itself
            // skip it... (avoid recursive call)
            CYCLE_PATTERN_NUMBER = SAFE_NEXT_PATTERN(CYCLE_PATTERN_NUMBER);
            while (skipCycle())
            {
                CYCLE_PATTERN_NUMBER = SAFE_NEXT_PATTERN(CYCLE_PATTERN_NUMBER);
            }

            Serial.println("Running Pattern: [" + gPattName[CYCLE_PATTERN_NUMBER] + "] ");
        }
        // Call the rotated pattern pattern
        drawPattern(gPattName[CYCLE_PATTERN_NUMBER]);
    }
};

#endif