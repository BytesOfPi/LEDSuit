/*
 * Matrix.h: https://github.com/BytesOfPi/LEDSuit
 * Copyright (c) 2020 Nathan DeGroff
 *
 * Overview:
 * This file defines and controls the Matrix component.
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
#include "../Shared/ComponentLED.h"

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

#define MATRIX_NUM_DRAWABLES 10
/**
 * Matrix Class
 * This class controls drawing to the LED matrix on the outfit.
 */
class Matrix : public ComponentLED
{
private:
    //--------------------------------------------------------------
    // Create the new FastLED GFX Canvas
    //GFXcanvas canvas(CANVAS_WIDTH, CANVAS_HEIGHT);
    GFXcanvas *myCanvas = new GFXcanvas(16, 16);
    //--------------------------------------------------------------
    // Create new pattern classes
    Drawable *matrixDraw[MATRIX_NUM_DRAWABLES] = {new PatternFire(CANVAS_WIDTH, CANVAS_HEIGHT),
                                                  new PatternWave(CANVAS_WIDTH, CANVAS_HEIGHT),
                                                  new PatternSpiro(CANVAS_WIDTH, CANVAS_HEIGHT),
                                                  new PatternPlasma(CANVAS_WIDTH, CANVAS_HEIGHT),
                                                  new PatternSwirl(CANVAS_WIDTH, CANVAS_HEIGHT),
                                                  new PatternSpin(CANVAS_WIDTH, CANVAS_HEIGHT),
                                                  new PatternPulse(CANVAS_WIDTH, CANVAS_HEIGHT),
                                                  new PatternIncrementalDrift(CANVAS_WIDTH, CANVAS_HEIGHT),
                                                  new PatternBTS(CANVAS_WIDTH, CANVAS_HEIGHT),
                                                  new PatternCustomBit(CANVAS_WIDTH, CANVAS_HEIGHT)};
    Drawable *thisDrawable = matrixDraw[0];

public:
    //--------------------------------------------------------------
    // Constructor
    Matrix() : ComponentLED("Matrix", gConfig.currentMatrixPattern) {}

    /**
     * setup()
     * This method initializes the LEDs and the class variables.
     */
    void setup()
    {
        FastLED.addLeds<MATRIX_CHIPSET, MATRIX_DATA_PIN, MATRIX_COLOR_ORDER>(myCanvas[0].getBuffer(), CANVAS_NUM_LEDS).setCorrection(TypicalLEDStrip);
        FastLED.setBrightness(MATRIX_BRIGHTNESS);

        myCanvas[0].setTextWrap(false);
        myCanvas[0].setTextSize(1);
    }

    /**
      * checkGlobal()
      * This method checks the global palettes and colors to see if anything
      * changed
      */
    virtual void checkGlobal()
    {
        if(gConfig.changeMatrix)
        {
            Serial.println("Matrix Config Change");
            thisDrawable->setPalette(gConfig.currentMatrixPalette.pal);
            gConfig.changeMatrix = false;
        }
    }

    /**
       * switchPattern()
       * This method is called every time there is a pattern switch.  This will allow
       * us to reset any stored values that patterns use 
       */
    virtual void switchPattern()
    {
        //------------------------------------------------------------
        // Clear the old Drawable
        myCanvas[0].fillScreen(CRGB::Black);
        thisDrawable->stop(myCanvas[0]);

        //------------------------------------------------------------
        // Switch to the new Drawable
        thisDrawable = getDrawable(patt->getPattern());

        //------------------------------------------------------------
        // Initialize the new Drawable
        thisDrawable->start(myCanvas[0]);
        // thisDrawable->setPalette(arrMPal[random8(PALETTE_NUM)].pal);
        thisDrawable->setPalette(gConfig.currentMatrixPalette.pal);
    }

    virtual GFXcanvas getCanvas()
    {
        return *myCanvas;
    }
    /**
     * drawPatt()
     * Given a specific pattern string, draw the related pattern.
     */
    virtual unsigned int drawPattern(String val)
    {
        //------------------------------------------------------------
        // Draw the pattern
        thisDrawable->drawFrame(myCanvas[0]);
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
        // Switch cycle() pattern every 10 seconds
        EVERY_N_SECONDS(10)
        {
            if (patt->isOnCycle())
            {
                patt->cycleNext();
                switchPattern();
            }
            printCurrentPattern();
        }
    }

    /**
     * getDrawable()
     * Given the name of a Drawable, return it.  If none can be found,
     * return the first one.
     */
    Drawable *getDrawable(String val)
    {
        for (int i = 0; i < MATRIX_NUM_DRAWABLES; i++)
        {
            if (matrixDraw[i]->name.equals(val))
            {
                return matrixDraw[i];
            }
        }
        return matrixDraw[0];
    }
};

#endif
