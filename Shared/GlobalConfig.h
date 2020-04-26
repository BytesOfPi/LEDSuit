
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