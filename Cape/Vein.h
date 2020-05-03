/*
 * Vein.h: https://github.com/BytesOfPi/LEDSuit
 * Copyright (c) 2020 Nathan DeGroff
 *
 * Overview:
 * This file defines and controls a Vein component.  While each wing is one single
 * line of LEDs, each vein defines a start (from backpack) and an end (tip of wing).
 * This definition of LEDs helps make drawing patterns easier while calculating where
 * a particular LED is on the vein using array math.
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

#ifndef Vein_H
#define Vein_H

// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 55, suggested range 20-100
#define VEIN_COOLING 100

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
// #define VEIN_SPARKING 120
#define VEIN_SPARKING 120

//#################################################################
// Pre defined custom color palettes
const TProgmemPalette16 annaFirePalette_p PROGMEM =
    {
        CRGB::Black,
        CRGB::DarkRed,
        CRGB::Maroon,
        CRGB::Maroon,

        0xFF9900,
        CRGB::Yellow,
        0xFF9900,
        CRGB::Orange,

        0xFF9900,
        CRGB::Orange,
        CRGB::Maroon,
        CRGB::DarkRed,

        CRGB::Maroon,
        CRGB::Orange,
        CRGB::Maroon,
        CRGB::DarkRed};
class Vein
{
private:
    CRGB *ledsV;
    bool reverse;
    uint8_t *heat;
    uint8_t extend = 0;

public:
    int startPos;
    int endPos;
    int numLeds;
    Vein() {}

    void setup(CRGB setLED[], int startPosIn, int numLedsIn, bool rev)
    {
        ledsV = setLED;
        numLeds = numLedsIn;
        reverse = rev;
        startPos = startPosIn;
        endPos = startPosIn + numLedsIn - 1;
        // Create an array of size of LEDs for vein
        heat = (uint8_t *)malloc(numLeds * sizeof(uint8_t));
    }

    /**
     * getRGB()
     * This method gives us the actual memory address of the LED at a given position.
     * This allows us to set the value by setting the CRGB at this place.
     */
    CRGB &getRGB(int index)
    {
        return ledsV[index];
    }

    /**
     * getRGBReverse()
     * This method does the same as getRGB().  If the Vein is marked as reverse, it 
     * will start from end of LED array which allows us to do chasers where one Vein
     * starts where the other ends.
     */
    CRGB &getRGBReverse(int index)
    {
        return ledsV[(reverse) ? numLeds - index - 1 : index];
    }

    /**
     * switchPattern()
     * This method is called every time there is a pattern switch.  This will allow
     * us to reset any stored values that patterns use 
     */
    void switchPattern()
    {
        extend = 0;
    }

    /**
     * patternExtend()
     * This method will take value (extent) between 0-255 and draw  
     */
    void patternExtend(uint8_t extent, boolean revThis = false, CRGB crgb = CHSV(HUE_GREEN, 255, 255))
    {
        uint8_t point = map(extent, 0, 255, 0, numLeds - 1);
        for (uint8_t i = 0; i < point; i++)
        {
            getRGB(revThis ? numLeds - i - 1 : i) = crgb;
        }
    }

    /**
     * patternFire()
     * This method will draw fire extending out from the backpack down the vein
     */
    void patternFire(boolean revThis = false)
    {
        // Step 1.  Cool down every cell a little
        for (uint8_t i = 0; i < numLeds; i++)
        {
            heat[i] = qsub8(heat[i], random8(0, ((VEIN_COOLING * 10) / numLeds) + 2));
        }
        // Step 2.  Heat from each cell drifts 'up' and diffuses a little
        for (uint8_t k = numLeds - 1; k >= 2; k--)
        {
            heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
        }
        // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
        if (random8() < VEIN_SPARKING)
        {
            uint8_t y = random8(7);
            heat[y] = qadd8(heat[y], random8(160, 255));
        }

        // Step 4.  Map from heat cells to LED colors
        for (int j = 0; j < numLeds; j++)
        {
            // Scale the heat value from 0-255 down to 0-240
            // for best results with color palettes.
            byte colorindex = scale8(heat[j], 240); // HeatColors_p, LavaColors_p
            getRGB(revThis ? numLeds - j - 1 : j) = (colorindex != 0) ? ColorFromPalette(annaFirePalette_p, colorindex) : CRGB::Black;
        }
    }

    /**
     * patternLightsaber()
     * This method will take value (extent) between 0-255 and draw  
     */
    void patternLightsaber(bool on, boolean revThis = false)
    {
        int hold = extend;
        //------------------------------------------------------------
        // If saber is on but not fully extended, keep extending
        if (on && extend <= 255)
        {
            extend = min(hold + 10, 255);
            patternExtend(extend, revThis, gConfig.currentCapeColor.rgb);
            return;
        }
        //------------------------------------------------------------
        // If saber is off but not fully extinguished, keep retracting
        if (!on && extend > 0)
        {
            extend = max(hold - 10, 0);
            patternExtend(extend, revThis, gConfig.currentCapeColor.rgb);
            return;
        }
    }
};

#endif
