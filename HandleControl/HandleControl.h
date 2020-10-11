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

#ifndef Button_H
#define Button_H

#define DATA_PIN_TRIGGER 27
#define DATA_PIN_HANDLE1 26

//################################################################################
// Shared variable libraries
#include "../Shared/Share.h"

#define CONTROL_HANDLE_OUTFIT 0
#define CONTROL_HANDLE_CAPE 1
#define CONTROL_HANDLE_MATRIX 2
#define CONTROL_HANDLE_SUIT 3

#define CONTROL_HANDLE_VALS 4
uint16_t chStates[] = {CONTROL_HANDLE_OUTFIT, CONTROL_HANDLE_CAPE, CONTROL_HANDLE_MATRIX, CONTROL_HANDLE_SUIT};

class HandleControl
{
private:
    unsigned long hand1DebounceTime = 0;   // the last time the output pin was toggled
    unsigned long hand1DebounceDelay = 50; // the debounce time; increase if the output flickers
    int hand1ButtonState;
    int hand1ButtonStateLast = HIGH; // PINS start high.. when connected to ground (pressed) they go LOW

    unsigned long triggerDebounceTime = 0;   // the last time the output pin was toggled
    unsigned long triggerDebounceDelay = 50; // the debounce time; increase if the output flickers
    int trigButtonState;
    int trigButtonStateLast = HIGH; // PINS start high.. when connected to ground (pressed) they go LOW

    uint16_t chState = 0;

    unsigned long clearTime = 0;
    unsigned long clearDelay = 1000;

public:
    //--------------------------------------------------------------
    // Constructor
    HandleControl() {}

    /**
     * setup()
     * This method initializes the LEDs and the class variables.
     */
    void setup()
    {
        //--------------------------------------------------------------
        // Setup Handle button pins to pullup
        pinMode(DATA_PIN_TRIGGER, INPUT_PULLUP);
        pinMode(DATA_PIN_HANDLE1, INPUT_PULLUP);
    }

    boolean clearDelayPassed()
    {
        if ((millis() - clearTime) < clearDelay)
        {
            return false;
        }
        // Both buttons are pressed
        if (*trigButtonState == LOW && *hand1ButtonState == LOW)
        {
            Serial.println("CLEAR STATE");
            clearTime = millis();
            return false;
        }
        return true;
    }

    /**
     * triggerPress()
     * This method checks to see if trigger button is pressed
     */
    void triggerPress()
    {
        if (clearDelayPassed() &&
            debouncePress(DATA_PIN_TRIGGER, &triggerDebounceTime, triggerDebounceDelay, &trigButtonState, &trigButtonStateLast))
        {
            Serial.println("Trigger Pressed");
            changeValue();
        }
    }

    /**
     * triggerPress()
     * This method checks to see if trigger button is pressed
     */
    void handle1Press()
    {
        if (clearDelayPassed() &&
            debouncePress(DATA_PIN_HANDLE1, &hand1DebounceTime, hand1DebounceDelay, &hand1ButtonState, &hand1ButtonStateLast))
        {
            Serial.println("Handle 1 Pressed");
            changeState();
        }
    }

    /**
     * debouncePress()
     * This method will debounce button presses to make sure it captures a single momentary
     * button press.  You may need to tweak the delay if the button press isn't registering fast
     * enough or is too fast per press.
     */
    boolean debouncePress(int pin, unsigned long *time, unsigned long delay, int *state, int *stateLast)
    {
        int reading = digitalRead(pin);
        boolean press = false;

        // If the switch changed, due to noise or pressing:
        if (reading != *stateLast)
        {
            // reset the debouncing timer
            *time = millis();
        }

        // save the reading. Next time through the loop, it'll be the stateLast:
        *stateLast = reading;

        // check if time is long enough
        if ((millis() - *time) > delay)
        {
            // whatever the reading is at, it's been there for longer than the debounce
            // delay, so take it as the actual current state:

            // if the button state has changed:
            if (reading != *state)
            {
                *state = reading;

                // only toggle the LED if the new button state is LOW
                if (*state == LOW)
                {
                    return true;
                }
            }
        }
        return false;
    }

    void clearState()
    {
    }

    void changeState()
    {
        chState = (chState == CONTROL_HANDLE_VALS - 1) ? 0 : chState + 1;
        Serial.println("Change OUTFIT value");
        switch (chState)
        {
        case CONTROL_HANDLE_OUTFIT:
            Serial.println("OUTFIT STATE");
            break;
        case CONTROL_HANDLE_CAPE:
            Serial.println("CAPE STATE");
            break;
        case CONTROL_HANDLE_MATRIX:
            Serial.println("MATRIX STATE");
            break;
        case CONTROL_HANDLE_SUIT:
            Serial.println("SUIT STATE");
            break;
        }
    }

    void changeValue()
    {
        switch (chState)
        {
        case CONTROL_HANDLE_OUTFIT:
            Serial.println("Change OUTFIT value");
            gConfig.currentSuitPattern->bumpNext();
            gConfig.changeSuit = true;
            break;
        case CONTROL_HANDLE_CAPE:
            Serial.println("Change CAPE value");
            gConfig.currentCapePattern->bumpNext();
            gConfig.changeCape = true;
            break;
        case CONTROL_HANDLE_MATRIX:
            Serial.println("Change MATRIX value");
            gConfig.currentMatrixPattern->bumpNext();
            gConfig.changeMatrix = true;
            break;
        case CONTROL_HANDLE_SUIT:
            Serial.println("Change SUIT value");
            gConfig.currentSuitPattern->bumpNext();
            gConfig.changeSuit = true;
            break;
        }
    }
};

#endif