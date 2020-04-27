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

class GlobalConfig
{
public:
    //--------------------------------------------------------------
    // Current Cape configuration
    CPattern *currentCapePattern;
    MPal currentCapePalette = PALETTE_PARTY;
    MPal currentCapeSecPalette = PALETTE_PARTY;
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
};

#endif