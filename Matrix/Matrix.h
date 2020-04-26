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

#ifndef Matrix_H
#define Matrix_H

#define MATRIX_DATA_PIN 5
// #define MATRIX_DATA_PIN 18
#define MATRIX_CHIPSET WS2812B
#define MATRIX_COLOR_ORDER GRB
#define MATRIX_BRIGHTNESS 20

//################################################################################
// Shared variable libraries
#include "../Shared/Share.h"
#include "../Shared/Component.h"

//################################################################################
// Custom Matrix Libraries
#include "PatternSpiro.h"
#include "PatternPlasma.h"
#include "PatternSwirl.h"
#include "PatternSpin.h"
#include "PatternPulse.h"
#include "PatternIncrementalDrift.h"
#include "PatternWave.h"
#include "PatternFire.h"

#include "PatternBTS.h"
#include "PatternCustomBit.h"

#define MATRIX_NUM_PALETTES 7
CRGBPalette16 matrixPalettes[] = {(CRGBPalette16)CloudColors_p, (CRGBPalette16)LavaColors_p, (CRGBPalette16)OceanColors_p,
                                  (CRGBPalette16)ForestColors_p, (CRGBPalette16)RainbowColors_p, (CRGBPalette16)RainbowStripeColors_p,
                                  (CRGBPalette16)HeatColors_p};

class Matrix : public ComponentLED
{
private:
    //--------------------------------------------------------------
    // Create the new FastLED GFX Canvas
    //GFXcanvas canvas(CANVAS_WIDTH, CANVAS_HEIGHT);
    GFXcanvas *myCanvas = new GFXcanvas(16, 16);
    //--------------------------------------------------------------
    // Create new pattern classes
    Drawable *matrixDraw[MATRIX_NUM_PATTERNS] = {new PatternFire(CANVAS_WIDTH, CANVAS_HEIGHT),
                                                 new PatternWave(CANVAS_WIDTH, CANVAS_HEIGHT),
                                                 new PatternSpiro(CANVAS_WIDTH, CANVAS_HEIGHT),
                                                 new PatternPlasma(CANVAS_WIDTH, CANVAS_HEIGHT),
                                                 new PatternSwirl(CANVAS_WIDTH, CANVAS_HEIGHT),
                                                 new PatternSpin(CANVAS_WIDTH, CANVAS_HEIGHT),
                                                 new PatternPulse(CANVAS_WIDTH, CANVAS_HEIGHT),
                                                 new PatternIncrementalDrift(CANVAS_WIDTH, CANVAS_HEIGHT),
                                                 new PatternBTS(CANVAS_WIDTH, CANVAS_HEIGHT),
                                                 new PatternCustomBit(CANVAS_WIDTH, CANVAS_HEIGHT)};
    Drawable *thisPattern = matrixDraw[0];

public:
    //--------------------------------------------------------------
    // Constructor
    Matrix() : ComponentLED("Matrix", currentMatrixPattern, patternMatrix, MATRIX_NUM_PATTERNS, patternMatrixCycleSkip, MATRIX_NUM_CYCLE_SKIP) {}

    Drawable *getPattern(String patt)
    {
        for (int i = 0; i < MATRIX_NUM_PATTERNS; i++)
        {
            if (matrixDraw[i]->name.equals(patt))
                return matrixDraw[i];
        }
        return matrixDraw[0];
    }
    /**
       * switchPattern()
       * This method is called every time there is a pattern switch.  This will allow
       * us to reset any stored values that patterns use 
       */
    virtual void switchPattern()
    {
        myCanvas[0].fillScreen(CRGB::Black);
        thisPattern->stop(myCanvas[0]);
        thisPattern = getPattern(currentPatt.equals("cycle") ? patternMatrix[cyclePattIndex] : currentPatt);

        Serial.print("Matrix: ["); Serial.print(thisPattern->name); Serial.println("]");
        thisPattern->start(myCanvas[0]);
        thisPattern->setPalette(matrixPalettes[random8(MATRIX_NUM_PALETTES)]);
    }
    /**
       * setup()
       * This method initializes the LEDs and the class variables.
       */
    void setup()
    {
        Serial.print("Matrix::setup()");
        FastLED.addLeds<MATRIX_CHIPSET, MATRIX_DATA_PIN, MATRIX_COLOR_ORDER>(myCanvas[0].getBuffer(), CANVAS_NUM_LEDS).setCorrection(TypicalLEDStrip);
        FastLED.setBrightness(MATRIX_BRIGHTNESS);

        myCanvas[0].setTextWrap(false);
        myCanvas[0].setTextSize(1);

        //------------------------------------------------------------
        // Get to next safe cycle
        cycleNext();
    }

    /**
       * drawPatt()
       * Given a specific pattern string, draw the related pattern.
       */
    virtual unsigned int drawPattern(String patt)
    {
        //------------------------------------------------------------
        // return cycle
        if (patt.equals(CAPE_PATT_CYCLE))
            return cycle();

        //------------------------------------------------------------
        // Draw the pattern
        thisPattern->drawFrame(myCanvas[0]);

        return 0;
    }
    virtual void periodicAdjust()
    {
        //--------------------------------------------------------------
        // Switch cycle() pattern every 10 seconds
        EVERY_N_SECONDS(10)
        {
            cycleNext();
        }
    }
};

#endif
