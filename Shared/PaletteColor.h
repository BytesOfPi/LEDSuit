/*
 * PaletteColor.h: https://github.com/BytesOfPi/LEDSuit
 * Copyright (c) 2020 Nathan DeGroff
 *
 * Overview:
 * This file defines the structs and values for palettes and colors used by the
 * LED outfit's components.
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

#ifndef PaletteColor_H
#define PaletteColor_H

//#################################################################
// Custom TypeDefs
typedef struct MPal
{
    CRGBPalette16 pal;
    String name;
};

typedef struct MCol
{
    CRGB pal;
    String name;
};

//#################################################################
// Shared Values
MPal PALETTE_PARTY = {(CRGBPalette16)PartyColors_p, "Party"};
MPal PALETTE_CLOUD = {(CRGBPalette16)CloudColors_p, "Cloud"};
MPal PALETTE_LAVA = {(CRGBPalette16)LavaColors_p, "Lava"};
MPal PALETTE_OCEAN = {(CRGBPalette16)OceanColors_p, "Ocean"};
MPal PALETTE_FOREST = {(CRGBPalette16)ForestColors_p, "Forest"};
MPal PALETTE_RAINBOW = {(CRGBPalette16)RainbowStripeColors_p, "Rainbow"};
MPal PALETTE_HEAT = {(CRGBPalette16)HeatColors_p, "Heat"};
MPal PALETTE_CCHS = {(CRGBPalette16)myCCHS1Palette_p, "CCHS"};
MPal PALETTE_RWB = {(CRGBPalette16)myRedWhiteBluePalette_p, "RWB"};
MPal PALETTE_CMAS = {(CRGBPalette16)myChristmasPalette_p, "Christmas"};

#define PALETTE_NUM 10
MPal arrMPal[] = {PALETTE_PARTY, PALETTE_CLOUD, PALETTE_LAVA, PALETTE_OCEAN,
                  PALETTE_FOREST, PALETTE_RAINBOW, PALETTE_HEAT, PALETTE_CCHS,
                  PALETTE_RWB, PALETTE_CMAS};

// http://fastled.io/docs/3.1/pixeltypes_8h_source.html#l00090
MCol COLOR_RED = {CRGB::Red, "Red"};
MCol COLOR_ORANGE = {CRGB::Orange, "Orange"};
MCol COLOR_YELLOW = {CRGB::Yellow, "Yellow"};
MCol COLOR_GREEN = {CRGB::Green, "Green"};
MCol COLOR_BLUE = {CRGB::Blue, "Blue"};
MCol COLOR_VIOLET = {CRGB::Violet, "Violet"};
MCol COLOR_IVORY = {CRGB::Ivory, "Ivory"};

#define COLOR_NUM 7
MCol arrMCol[] = {COLOR_RED, COLOR_ORANGE, COLOR_YELLOW, COLOR_GREEN,
                  COLOR_BLUE, COLOR_VIOLET, COLOR_IVORY};

#endif