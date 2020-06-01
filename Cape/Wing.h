/*
 * Wing.h: https://github.com/BytesOfPi/LEDSuit
 * Copyright (c) 2020 Nathan DeGroff
 *
 * Overview:
 * This file defines and controls a Wing component.  The Wing is either the left
 * or right half of the cape.  It is a single chain of LEDs that loop from the top
 * to the middle back extending out in Veins.  The component tracks the LED loop for
 * patterns like chasers and full bpm patterns, but is divided into Veins for other
 * complex patterns like fire or lightsaber.
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

#ifndef Wing_H
#define Wing_H

#include "Vein.h"

#define NUM_VEIN 5

class Wing
{
private:
    CRGB *myLeds;
    int numLeds[NUM_VEIN];
    int totLeds;
    int totSideLeds;
    String name;

public:
    Vein veinsFront[NUM_VEIN];
    Vein veinsBack[NUM_VEIN];

    Wing() {}

    void setup(CRGB myLedsIn[], int totLedsIn, int *numLedsIn, String nameIn)
    {
        //--------------------------------------------------------------
        // Store params into class
        name = nameIn;
        myLeds = myLedsIn;
        totLeds = totLedsIn;
        totSideLeds = totLedsIn / 2;
        memcpy(numLeds, numLedsIn, sizeof(numLedsIn[0]) * 5);

        //--------------------------------------------------------------
        // Setup the start indexes of front & back veins
        int indexFront[] = {numLedsIn[0] * 2,
                            (numLedsIn[0] * 2) + (numLedsIn[1] * 2),
                            (numLedsIn[0] * 2) + (numLedsIn[1] * 2) + (numLedsIn[2] * 2),
                            (numLedsIn[0] * 2) + (numLedsIn[1] * 2) + (numLedsIn[2] * 2) + (numLedsIn[3] * 2)};
        int indexBack[] = {numLedsIn[0],
                           (numLedsIn[0] * 2) + numLedsIn[1],
                           (numLedsIn[0] * 2) + (numLedsIn[1] * 2) + numLedsIn[2],
                           (numLedsIn[0] * 2) + (numLedsIn[1] * 2) + (numLedsIn[2] * 2) + numLedsIn[3],
                           (numLedsIn[0] * 2) + (numLedsIn[1] * 2) + (numLedsIn[2] * 2) + (numLedsIn[3] * 2) + numLedsIn[4]};
        int startPos[] = {0,
                          numLedsIn[0],
                          numLedsIn[0] + numLedsIn[1],
                          numLedsIn[0] + numLedsIn[1] + numLedsIn[2],
                          numLedsIn[0] + numLedsIn[1] + numLedsIn[2] + numLedsIn[3]};
        //--------------------------------------------------------------
        // Create Array of veins
        veinsFront[0].setup(&myLeds[0], startPos[0], numLedsIn[0], false);
        veinsFront[1].setup(&myLeds[indexFront[0]], startPos[1], numLedsIn[1], true);
        veinsFront[2].setup(&myLeds[indexFront[1]], startPos[2], numLedsIn[2], false);
        veinsFront[3].setup(&myLeds[indexFront[2]], startPos[3], numLedsIn[3], true);
        veinsFront[4].setup(&myLeds[indexFront[3]], startPos[4], numLedsIn[4], false);

        veinsBack[0].setup(&myLeds[indexBack[0]], startPos[0], numLedsIn[0], false);
        veinsBack[1].setup(&myLeds[indexBack[1]], startPos[1], numLedsIn[1], true);
        veinsBack[2].setup(&myLeds[indexBack[2]], startPos[2], numLedsIn[2], false);
        veinsBack[3].setup(&myLeds[indexBack[3]], startPos[3], numLedsIn[3], true);
        veinsBack[4].setup(&myLeds[indexBack[4]], startPos[4], numLedsIn[4], false);

        //--------------------------------------------------------------
        // Add entropy to random generator
        random16_add_entropy(random16());
    }

    /**
     * getRGB()
     * This method gives us the actual memory address of the LED at a given position.
     * This allows us to set the value by setting the CRGB at this place.
     */
    CRGB &getRGB(int index, Vein veins[])
    {
        for (int i = 0; i < NUM_VEIN; i++)
        {
            if (index <= veins[i].endPos)
            {
                return veins[i].getRGB(index - veins[i].startPos);
            }
        }
        return veins[0].getRGB(0);
    }

    /**
     * getRGBReverse()
     * This method does the same as getRGB().  If the Vein is marked as reverse, it 
     * will start from end of LED array which allows us to do chasers where one Vein
     * starts where the other ends.
     */
    CRGB &getRGBReverse(int index, Vein veins[])
    {
        for (int i = 0; i < NUM_VEIN; i++)
        {
            if (index <= veins[i].endPos)
            {
                return veins[i].getRGBReverse(index - veins[i].startPos);
            }
        }
        return veins[0].getRGBReverse(0);
    }

    /**
     * switchPattern()
     * This method is called every time there is a pattern switch.  This will allow
     * us to reset any stored values that patterns use 
     */
    void switchPattern()
    {
        for (int i = 0; i < NUM_VEIN; i++)
        {
            veinsFront[i].switchPattern();
            veinsBack[i].switchPattern();
        }
    }

    //#################################################################
    // PATTERNS
    //#################################################################
    /**
     * patternJuggle()
     * This method causes this side of the cape to race a multicolored chaser
     * back and forth.  This runs on both sides of the cape.
     */
    void patternJuggle()
    {
        byte dothue = 0;
        fadeToBlackBy(myLeds, totLeds, 90); //20
        for (int i = 0; i < 8; i++)
        {
            int index = beatsin16(8, 0, totSideLeds - 1, i * 20);
            CRGB val = CHSV(dothue, 200, 255);
            getRGBReverse(index, veinsFront) |= val;
            getRGBReverse(index, veinsBack) |= val;
            dothue += 32;
        }
    }

    /**
     * patternPaletteConfetti()
     * This method causes this side of the cape to sparkle colors of a given palette.
     * It can use a different pallet for the front or back side of the cape.
     */
    void patternPaletteConfetti(CRGBPalette16 palFront, CRGBPalette16 palBack, TBlendType blendType)
    {
        uint8_t brightness = 175;
        CRGB palsF[8] = {ColorFromPalette(palFront, 0, brightness, NOBLEND),
                         ColorFromPalette(palFront, 16, brightness, NOBLEND),
                         ColorFromPalette(palFront, 32, brightness, NOBLEND),
                         ColorFromPalette(palFront, 48, brightness, NOBLEND),
                         ColorFromPalette(palFront, 64, brightness, NOBLEND),
                         ColorFromPalette(palFront, 80, brightness, NOBLEND),
                         ColorFromPalette(palFront, 96, brightness, NOBLEND),
                         ColorFromPalette(palFront, 112, brightness, NOBLEND)};
        CRGB palsB[8] = {ColorFromPalette(palBack, 0, brightness, NOBLEND),
                         ColorFromPalette(palBack, 16, brightness, NOBLEND),
                         ColorFromPalette(palBack, 32, brightness, NOBLEND),
                         ColorFromPalette(palBack, 48, brightness, NOBLEND),
                         ColorFromPalette(palBack, 64, brightness, NOBLEND),
                         ColorFromPalette(palBack, 80, brightness, NOBLEND),
                         ColorFromPalette(palBack, 96, brightness, NOBLEND),
                         ColorFromPalette(palBack, 112, brightness, NOBLEND)};
        //------------------------------------------------------------
        // random colored speckles that blink in and fade smoothly
        fadeToBlackBy(myLeds, totLeds, 10);
        // int pos1 = random16(0, totSideLeds - 1);
        // int pos2 = random16(0, totSideLeds - 1);
        int pos1 = random16(0, totSideLeds - 2);
        int pos2 = random16(0, totSideLeds - 2);

        int randIndex = random8(8);
        CRGB fPal = palsF[randIndex];
        CRGB bPal = palsB[randIndex];
        getRGB(pos1, veinsFront) = fPal;
        getRGB(pos2, veinsFront) = fPal;
        getRGB(pos1, veinsBack) = bPal;
        getRGB(pos2, veinsBack) = bPal;
    }

    /**
     * patternExtend()
     * This method causes veins of green to shoot out from backpack and back
     */
    void patternExtend()
    {
        fadeToBlackBy(myLeds, totLeds, 90); //20
            //------------------------------------------------------------
        // get a position between 0 and 255 for 8 beats per min
        uint8_t range = beatsin8(30, 0, 255);

        //------------------------------------------------------------
        // get a position between 0 and 255
        for (uint8_t i = 0; i < NUM_VEIN; i++)
        {
            veinsFront[i].patternExtend(range, false, gConfig.currentCapeColor.rgb);
            veinsBack[i].patternExtend(range, true, gConfig.currentCapeColor.rgb);
        }
    }

    /**
     * patternFire()
     * This method draws fire?
     */
    void patternFire()
    {
        //------------------------------------------------------------
        // Make sure flames fade to black
        fadeToBlackBy(myLeds, totLeds, 90); //20
        //------------------------------------------------------------
        // loop through veins and draw
        for (int x = 0; x < NUM_VEIN; x++)
        {
            veinsFront[x].patternFire();
            veinsBack[x].patternFire(true);
        }
    }

    void patternLightsaber(uint16_t mask)
    {
        //------------------------------------------------------------
        // Loop through veins and turn on if mask is set for that
        // particular vein
        for (int x = 0; x < NUM_VEIN; x++)
        {
            veinsFront[x].patternLightsaber(mask & frontMask[x]);
            veinsBack[x].patternLightsaber(mask & backMask[x], true);
        }
    }
};

#endif
