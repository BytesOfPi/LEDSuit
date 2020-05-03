/*
 * ComponentLED.h: https://github.com/BytesOfPi/LEDSuit
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

#ifndef ComponentLED_H
#define ComponentLED_H

//################################################################################
// Shared variable libraries
#include "../Shared/Share.h"

/**
 * ComponentLED Class
 * This class is the base of the outfit's LED component classes.  It
 * coordinates the routine tasks done by each component so they all
 * function similarly.
 */
class ComponentLED
{
public:
    String name;
    CPattern *patt;

    /**
      * Constructor
      * Initialize the component with required data
      */
    ComponentLED(String nameIn, CPattern *pattIn)
    {
        name = nameIn;
        //--------------------------------------------------------------
        // Store pattern data
        patt = pattIn;
    }

    /**
      * getCurrentPatt()
      * This method return's the component's current selected pattern.
      */
    virtual String getCurrentPatt() { return patt->getPattern(); };

    /**
      * drawFrame()
      * This method is called every time in the arduino loop() to draw
      * the component's next frame.
      */
    virtual unsigned int drawFrame()
    {
        //------------------------------------------------------------
        // Adjust pattern periodically
        periodicAdjust();

        //------------------------------------------------------------
        // If pattern isn't cycle and it's a new pattern, we need to
        // alert everyone and switch
        if (!patt->isOnCycle() && patt->isNewPattern())
        {
            switchPattern();
        }

        //------------------------------------------------------------
        // Draw the current pattern
        return drawPattern(getCurrentPatt());
    };

    /**
      * printCurrentPattern()
      * This method output's the current component's pattern information
      */
    virtual void printCurrentPattern()
    {
        // TODO: 
        // Serial.println(patt->toString());
    }

    /**
      * drawPattern()
      * This method is called to draw the component 
      */
    virtual unsigned int drawPattern(String val) { return 0; }

    /**
      * switchPattern()
      * This method is called every time there is a pattern switch.  This will allow
      * us to reset any stored values that patterns use
      */
    virtual void switchPattern() { printCurrentPattern(); }

    /**
      * periodicAdjust()
      * This method is called every time drawFrame() is called and sets up periodic
      * adjustments.
      */
    virtual void periodicAdjust()
    {
        //--------------------------------------------------------------
        // Switch cycle() pattern every 10 seconds
        EVERY_N_SECONDS(10)
        {
            printCurrentPattern();
            patt->cycleNext();
        }
    }
};

#endif