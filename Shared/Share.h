

#ifndef SharedValues_H
#define SharedValues_H

//#################################################################
// Pre defined custom color palettes
const TProgmemPalette16 myCCHS1Palette_p PROGMEM =
    {
        // Other purples are:  //CRGB::Indigo, CRGB::Purple, CRGB::DarkMagenta
        CRGB::Purple, CRGB::Gold, CRGB::Purple, CRGB::Gold,
        CRGB::Black, CRGB::Purple, CRGB::Gold, CRGB::Black,
        CRGB::Purple, CRGB::Gold, CRGB::Purple, CRGB::Gold,
        CRGB::Black, CRGB::Purple, CRGB::Gold, CRGB::Black};
const TProgmemPalette16 myCCHS2Palette_p PROGMEM =
    {
        // Other purples are:  //CRGB::Indigo, CRGB::Purple, CRGB::DarkMagenta
        CRGB::Purple, CRGB::Purple, CRGB::Gold, CRGB::Gold,
        CRGB::Purple, CRGB::Purple, CRGB::Gold, CRGB::Gold,
        CRGB::Purple, CRGB::Purple, CRGB::Gold, CRGB::Gold,
        CRGB::Purple, CRGB::Purple, CRGB::Gold, CRGB::Gold};
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
    {
        // Using CRGB::Gray instead of CRGB::White.  Too bright compared to red and blue
        CRGB::Red, CRGB::Gray, CRGB::Blue, CRGB::Black,
        CRGB::Red, CRGB::Gray, CRGB::Blue, CRGB::Black,
        CRGB::Red, CRGB::Gray, CRGB::Blue, CRGB::Black,
        CRGB::Red, CRGB::Gray, CRGB::Blue, CRGB::Black};
const TProgmemPalette16 myBlueGreenPalette_p PROGMEM =
    {
        CRGB::Blue, CRGB::Green, CRGB::Green,
        CRGB::Blue, CRGB::Green, CRGB::Green,
        CRGB::Blue, CRGB::Green, CRGB::Green,
        CRGB::Blue, CRGB::Green, CRGB::Green,
        CRGB::Blue, CRGB::Green, CRGB::Green,
        CRGB::Blue};
const TProgmemPalette16 myChristmasPalette_p PROGMEM =
    {
        CRGB::Gray, CRGB::Red, CRGB::Green, CRGB::SaddleBrown,
        CRGB::Gray, CRGB::Red, CRGB::Green, CRGB::SaddleBrown,
        CRGB::Gray, CRGB::Red, CRGB::Green, CRGB::SaddleBrown,
        CRGB::Gray, CRGB::Red, CRGB::Green, CRGB::SaddleBrown};

//#################################################################
// Shared Values
typedef struct MPal
{
    CRGBPalette16 pal;
    String name;
};
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

typedef struct MCol
{
    CRGB pal;
    String name;
};
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

//#################################################################
// BLE Shared values
#define BLE_SUIT_PATT_CALLBACK 0x00
#define BLE_SUIT_PAL_CALLBACK 0x01
#define BLE_SUIT_COL_CALLBACK 0x02

#define BLE_MATRIX_PATT_CALLBACK 0x03
#define BLE_MATRIX_PAL_CALLBACK 0x04
#define BLE_MATRIX_COL_CALLBACK 0x05

#define BLE_CAPE_PATT_CALLBACK 0x06
#define BLE_CAPE_PAL_CALLBACK 0x07
#define BLE_CAPE_PAL_SEC_CALLBACK 0x08
#define BLE_CAPE_COL_CALLBACK 0x09

#define BLE_MATRIX_SCROLL_CALLBACK 0x0A
#define BLE_MATRIX_TIMER_CALLBACK 0x0B

#define BLE_FULL_CALLBACK 0x10

//#################################################################
// Shared Cape Values
#define CAPE_BACK_VEIN_01 0x01
#define CAPE_BACK_VEIN_02 0x02
#define CAPE_BACK_VEIN_03 0x04
#define CAPE_BACK_VEIN_04 0x08
#define CAPE_BACK_VEIN_05 0x10
#define CAPE_FRONT_VEIN_01 0x20
#define CAPE_FRONT_VEIN_02 0x40
#define CAPE_FRONT_VEIN_03 0x80
#define CAPE_FRONT_VEIN_04 256 //0x0100
#define CAPE_FRONT_VEIN_05 512 //0x0200
uint16_t frontMask[] = {CAPE_FRONT_VEIN_01, CAPE_FRONT_VEIN_02, CAPE_FRONT_VEIN_03, CAPE_FRONT_VEIN_04, CAPE_FRONT_VEIN_05};
uint16_t backMask[] = {CAPE_BACK_VEIN_01, CAPE_BACK_VEIN_02, CAPE_BACK_VEIN_03, CAPE_BACK_VEIN_04, CAPE_BACK_VEIN_05};

// patterns
#define CAPE_NUM_PATTERNS 7
#define CAPE_NUM_CYCLE_SKIP 1
#define CAPE_PATT_CYCLE "cycle"
#define CAPE_PATT_LIGHTSABER "lightsaber"
#define CAPE_PATT_SPARKLE "sparkle"
#define CAPE_PATT_EXTEND "extend"
#define CAPE_PATT_JUGGLE "juggle"
#define CAPE_PATT_BPM "bpm"
#define CAPE_PATT_FIRE "fire"
String patternCape[] = {CAPE_PATT_CYCLE, CAPE_PATT_LIGHTSABER,
                        CAPE_PATT_SPARKLE, CAPE_PATT_EXTEND,
                        CAPE_PATT_JUGGLE, CAPE_PATT_BPM,
                        CAPE_PATT_FIRE};
String patternCapeCycleSkip[] = {CAPE_PATT_CYCLE};

//--------------------------------------------------------------
// Current Cape configuration
String currentCapePattern = CAPE_PATT_CYCLE;
MPal currentCapePalette = PALETTE_PARTY;
MPal currentCapeSecPalette = PALETTE_FOREST;
MCol currentCapeColor = COLOR_BLUE;

//#################################################################
// Shared Matrix Values
#define CANVAS_WIDTH 16
#define CANVAS_HEIGHT 16
#define CANVAS_NUM_LEDS (CANVAS_WIDTH * CANVAS_HEIGHT)

#define MATRIX_PATT_CYCLE "cycle"
#define MATRIX_PATT_FIRE "PatternFire"
#define MATRIX_PATT_WAVE "PatternWave"
#define MATRIX_PATT_SPIRO "PatternSpiro"
#define MATRIX_PATT_PLASMA "PatternPlasma"
#define MATRIX_PATT_SWIRL "PatternSwirl"
#define MATRIX_PATT_SPIN "PatternSpin"
#define MATRIX_PATT_PULSE "PatternPulse"
#define MATRIX_PATT_DRIFT "PatternDrift"
#define MATRIX_PATT_BTS "PatternBTS"
#define MATRIX_PATT_CUSTOM "PatternCustomBit"
#define MATRIX_PATT_SCROLL "MatrixScroll"
#define MATRIX_PATT_TIMER "MatrixTimer"

#define MATRIX_NUM_PATTERNS 11
String patternMatrix[] = {MATRIX_PATT_CYCLE, MATRIX_PATT_FIRE,
                          MATRIX_PATT_WAVE, MATRIX_PATT_SPIRO,
                          MATRIX_PATT_PLASMA, MATRIX_PATT_SWIRL,
                          MATRIX_PATT_SPIN, MATRIX_PATT_PULSE,
                          MATRIX_PATT_DRIFT, MATRIX_PATT_BTS,
                          MATRIX_PATT_CUSTOM};
#define MATRIX_NUM_CYCLE_SKIP 2
String patternMatrixCycleSkip[] = {MATRIX_PATT_CYCLE, MATRIX_PATT_CUSTOM};

//--------------------------------------------------------------
// Current Matrix configuration
String currentMatrixPattern = MATRIX_PATT_CYCLE;
MPal currentMatrixPalette = PALETTE_PARTY;
MCol currentMatrixColor = COLOR_BLUE;

//#################################################################
// Shared Suit Values
#define SUIT_PATT_CYCLE "cycle"
#define SUIT_PATT_CCHS "paletteCCHS"
#define SUIT_PATT_CCHS2 "paletteCCHS2"
#define SUIT_PATT_CCHSGLIT "palletteCCHSGlitter"
#define SUIT_PATT_SPLIT "twoSplit"
#define SUIT_PATT_JUGGLE "juggle"
#define SUIT_PATT_CONFETTI "confetti"
#define SUIT_PATT_PAL_RWB "paletteRWB"
#define SUIT_PATT_PAL_CLOUD "paletteCloud"
#define SUIT_PATT_PAL_LAVA "paletteLava"
#define SUIT_PATT_PAL_HEAT "paletteHeat"
#define SUIT_PATT_PAL_OCEAN "paletteOcean"
#define SUIT_PATT_PAL_FOREST "paletteForest"
#define SUIT_PATT_PAL_CMAS "paletteChristmas"
#define SUIT_PATT_BPM_PARTY "bpmParty"
#define SUIT_PATT_BPM_RWB "bpmRWB"
#define SUIT_PATT_BPM_CMAS "bpmChristmas"
#define SUIT_PATT_SOLID_BLUE "solidBlue"
#define SUIT_PATT_SOLID_RED "solidRed"
#define SUIT_PATT_SOLID_GREEN "solidGreen"
#define SUIT_PATT_THE_CCHS "theaterCCHS"
#define SUIT_PATT_THE_CMAS "theaterChristmas"
#define SUIT_PATT_MIX_CCHS "mixinsCCHS"
#define SUIT_PATT_MIX_LAVA "mixinsLava"

#define SUIT_PATT_MIX2_LAVA "mixSpecLava"
#define SUIT_PATT_MIX2_OCEAN "mixSpecOcean"
#define SUIT_PATT_MIX2_FOREST "mixSpecForest"

#define SUIT_NUM_PATTERNS 27
String patternSuit[] = {SUIT_PATT_CYCLE, SUIT_PATT_CCHS, SUIT_PATT_CCHS2, SUIT_PATT_CCHSGLIT,
                        SUIT_PATT_SPLIT, SUIT_PATT_JUGGLE, SUIT_PATT_CONFETTI, SUIT_PATT_PAL_RWB,
                        SUIT_PATT_PAL_CLOUD, SUIT_PATT_PAL_LAVA, SUIT_PATT_PAL_HEAT, SUIT_PATT_PAL_OCEAN,
                        SUIT_PATT_PAL_FOREST, SUIT_PATT_PAL_CMAS, SUIT_PATT_BPM_PARTY, SUIT_PATT_BPM_RWB,
                        SUIT_PATT_BPM_CMAS, SUIT_PATT_SOLID_BLUE, SUIT_PATT_SOLID_RED, SUIT_PATT_SOLID_GREEN,
                        SUIT_PATT_THE_CCHS, SUIT_PATT_THE_CMAS, SUIT_PATT_MIX_CCHS, SUIT_PATT_MIX_LAVA,
                        SUIT_PATT_MIX2_LAVA, SUIT_PATT_MIX2_OCEAN, SUIT_PATT_MIX2_FOREST};

#define SUIT_NUM_CYCLE_SKIP 1
String patternSuitCycleSkip[] = {SUIT_PATT_CYCLE};

//--------------------------------------------------------------
// Current Suit configuration
String currentSuitPattern = SUIT_PATT_CYCLE;
MPal currentSuitPalette = PALETTE_PARTY;
MCol currentSuitColor = COLOR_BLUE;

//#################################################################
// Full Outfit Combo
#define FULL_PATT_CYCLE "cycle"
#define FULL_PATT_BPM "bpm"
#define FULL_PATT_CCHS "CCHS"

//#################################################################
// Determine if string is in list
bool inList(String val, String list[], uint16_t listSize)
{
    for (int i = 0; i <= listSize; i++)
    {
        // if we find the selected pattern, set the index
        if (val.equalsIgnoreCase(list[i]))
        {
            return true;
        }
    }
    return false;
}

/*
 * blePalette()
 * This method is called from Bluetooth and directs what the 
 * current palette should be
 */
void blePalette(String val, uint8_t callback)
{
    for (int i = 0; i < PALETTE_NUM; i++)
    {
        if (arrMPal[i].name.equals(val))
        {
            switch (callback)
            {
            case BLE_SUIT_PAL_CALLBACK:
                currentSuitPalette = arrMPal[i];
                return;
            case BLE_MATRIX_PAL_CALLBACK:
                currentMatrixPalette = arrMPal[i];
                return;
            case BLE_CAPE_PAL_CALLBACK:
                currentCapePalette = arrMPal[i];
                return;
            case BLE_CAPE_PAL_SEC_CALLBACK:
                currentCapeSecPalette = arrMPal[i];
                return;
            }
        }
    }
}

/*
 * bleColor()
 * This method is called from Bluetooth and directs what the 
 * current color should be
 */
void bleColor(String val, uint8_t callback)
{
    for (int i = 0; i < COLOR_NUM; i++)
    {
        if (arrMCol[i].name.equals(val))
        {
            switch (callback)
            {
            case BLE_SUIT_COL_CALLBACK:
                currentSuitColor = arrMCol[i];
                return;
            case BLE_MATRIX_COL_CALLBACK:
                currentMatrixColor = arrMCol[i];
                return;
            case BLE_CAPE_COL_CALLBACK:
                currentCapeColor = arrMCol[i];
                return;
            }
        }
    }
}


#endif