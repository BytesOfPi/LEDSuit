/*
 * GlobalConfig.h: https://github.com/BytesOfPi/LEDSuit
 * Copyright (c) 2020 Nathan DeGroff
 *
 * Overview:
 * This class defines the global values shared between classes.
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

#ifndef GlobalConfig_H
#define GlobalConfig_H

//#################################################################
// Include CPattern class
#include "CPattern.h"
#include "PaletteColor.h"

#define NO_CHANGE "XXX"

#define BLE_SUIT_TYPE 0x00
#define BLE_MATRIX_TYPE 0x01
#define BLE_CAPE_TYPE 0x02
#define BLE_CAPE_TYPE_2 0x03

class GlobalConfig
{
private:
    void setupSuit(String val, MPal pal = PALETTE_PARTY, MCol col = COLOR_BLUE)
    {
        currentSuitPattern->setPattern(val);
        currentSuitPalette = pal;
        currentSuitColor = col;
        Serial.print("Suit[");
        Serial.print(val);
        Serial.print("][");
        Serial.print(pal.name);
        Serial.print("][");
        Serial.print(col.name);
        Serial.println("]");
    }
    void setupMatrix(String val, MPal pal = PALETTE_PARTY, MCol col = COLOR_BLUE)
    {
        currentMatrixPattern->setPattern(val);
        currentMatrixPalette = pal;
        currentMatrixColor = col;
        Serial.print("Matrix[");
        Serial.print(val);
        Serial.print("][");
        Serial.print(pal.name);
        Serial.print("][");
        Serial.print(col.name);
        Serial.println("]");
    }
    void setupCape(String val, MPal pal = PALETTE_PARTY, MPal pal2 = PALETTE_PARTY, MCol col = COLOR_BLUE)
    {
        currentCapePattern->setPattern(val);
        currentCapePalette = pal;
        currentCapeSecPalette = pal2;
        currentCapeColor = col;
        Serial.print("Cape[");
        Serial.print(val);
        Serial.print("][");
        Serial.print(pal.name);
        Serial.print("][");
        Serial.print(pal2.name);
        Serial.print("][");
        Serial.print(col.name);
        Serial.println("]");
    }

    void setupPalette(String val, uint8_t type)
    {
        for (int i = 0; i < PALETTE_NUM; i++)
        {
            if (arrMPal[i].name.equals(val))
            {
                Serial.print("PALETTE ");
                Serial.print(String(i));
                Serial.print(" ");
                Serial.print(val);
                switch (type)
                {
                case BLE_SUIT_TYPE:
                    currentSuitPalette = arrMPal[i];
                    return;
                case BLE_MATRIX_TYPE:
                    currentMatrixPalette = arrMPal[i];
                    return;
                case BLE_CAPE_TYPE:
                    currentCapePalette = arrMPal[i];
                    return;
                case BLE_CAPE_TYPE_2:
                    currentCapeSecPalette = arrMPal[i];
                    return;
                }
            }
        }
        Serial.println("PALETTE: COULDN't FIND");
    }
    void setupColor(String val, uint8_t type)
    {
        for (int i = 0; i < COLOR_NUM; i++)
        {
            if (arrMCol[i].name.equals(val))
            {
                Serial.print("COLOR ");
                Serial.print(String(i));
                Serial.print(" ");
                Serial.print(val);
                switch (type)
                {
                case BLE_SUIT_TYPE:
                    currentSuitColor = arrMCol[i];
                    return;
                case BLE_MATRIX_TYPE:
                    currentMatrixColor = arrMCol[i];
                    return;
                case BLE_CAPE_TYPE:
                    currentCapeColor = arrMCol[i];
                    return;
                }
            }
        }
        Serial.println("COLOR: COULDN't FIND");
    }

public:
    boolean change = false;
    String holdSuitPal = NO_CHANGE;
    String holdSuitCol = NO_CHANGE;
    String holdMatrixPal = NO_CHANGE;
    String holdMatrixCol = NO_CHANGE;
    String holdCapePal = NO_CHANGE;
    String holdCapeSecPal = NO_CHANGE;
    String holdCapeCol = NO_CHANGE;

    String holdSuitPattern = NO_CHANGE;
    String holdMatrixPattern = NO_CHANGE;
    String holdCapePattern = NO_CHANGE;

    //--------------------------------------------------------------
    // Current Cape configuration
    CPattern *currentCapePattern;
    MPal currentCapePalette = PALETTE_PARTY;
    MPal currentCapeSecPalette = PALETTE_FOREST;
    MCol currentCapeColor = COLOR_BLUE;

    //--------------------------------------------------------------
    // Current Matrix configuration
    CPattern *currentMatrixPattern;
    MPal currentMatrixPalette = PALETTE_PARTY;
    MCol currentMatrixColor = COLOR_BLUE;

    //--------------------------------------------------------------
    // Current Suit configuration
    CPattern *currentSuitPattern;
    MPal currentSuitPalette = PALETTE_PARTY;
    MCol currentSuitColor = COLOR_BLUE;

    GlobalConfig(CPattern *cPatt, CPattern *mPatt, CPattern *sPatt)
    {
        currentCapePattern = cPatt;
        currentMatrixPattern = mPatt;
        currentSuitPattern = sPatt;
    }

    void checkNextPattern()
    {
        //--------------------------------------------------------------
        // If no change happened, return early
        if(!change)
        {
            return;
        }
        //--------------------------------------------------------------
        // Change to full pattern and turn off change
        if (!holdSuitPattern.equals(NO_CHANGE))
        {
            setupSuit(holdSuitPattern);
            holdSuitPattern = NO_CHANGE;
        }
        //--------------------------------------------------------------
        // Change to full pattern and turn off change
        if (!holdMatrixPattern.equals(NO_CHANGE))
        {
            setupMatrix(holdMatrixPattern);
            holdMatrixPattern = NO_CHANGE;
        }
        //--------------------------------------------------------------
        // Change to full pattern and turn off change
        if (!holdCapePattern.equals(NO_CHANGE))
        {
            setupCape(holdCapePattern);
            holdCapePattern = NO_CHANGE;
        }

        if (!holdSuitPal.equals(NO_CHANGE))
        {
            setupPalette(holdSuitPal, BLE_SUIT_TYPE);
            holdSuitPal = NO_CHANGE;
        }
        if (!holdSuitCol.equals(NO_CHANGE))
        {
            setupColor(holdSuitCol, BLE_SUIT_TYPE);
            holdSuitCol = NO_CHANGE;
        }
        if (!holdMatrixPal.equals(NO_CHANGE))
        {
            setupPalette(holdMatrixPal, BLE_MATRIX_TYPE);
            holdMatrixPal = NO_CHANGE;
        }
        if (!holdMatrixCol.equals(NO_CHANGE))
        {
            setupColor(holdMatrixCol, BLE_MATRIX_TYPE);
            holdMatrixCol = NO_CHANGE;
        }
        if (!holdCapePal.equals(NO_CHANGE))
        {
            setupPalette(holdCapePal, BLE_CAPE_TYPE);
            holdCapePal = NO_CHANGE;
        }
        if (!holdCapeSecPal.equals(NO_CHANGE))
        {
            setupPalette(holdCapeSecPal, BLE_CAPE_TYPE_2);
            holdCapeSecPal = NO_CHANGE;
        }
        if (!holdCapeCol.equals(NO_CHANGE))
        {
            setupColor(holdCapeCol, BLE_CAPE_TYPE);
            holdCapeCol = NO_CHANGE;
        }
    }
};

#endif