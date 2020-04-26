
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