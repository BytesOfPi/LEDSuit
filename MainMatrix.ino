/* ################################################################################
 * LED Main
 * The following file defines the main setup and loop for LEDs as well as shared
 * variables and methods
 */
#define MATRIX_DATA_PIN 5
// #define MATRIX_DATA_PIN 18
#define MATRIX_CHIPSET WS2812B
#define MATRIX_COLOR_ORDER GRB
#define MATRIX_BRIGHTNESS 20

#define CANVAS_WIDTH 16
#define CANVAS_HEIGHT 16
#define CANVAS_NUM_LEDS (CANVAS_WIDTH * CANVAS_HEIGHT)

//################################################################################
// Custom Matrix Libraries
#include "Matrix/PatternSpiro.h"
#include "Matrix/PatternPlasma.h"
#include "Matrix/PatternSwirl.h"
#include "Matrix/PatternSpin.h"
#include "Matrix/PatternPulse.h"
#include "Matrix/PatternIncrementalDrift.h"
#include "Matrix/PatternWave.h"
#include "Matrix/PatternFire.h"

#include "Matrix/PatternBTS.h"
#include "Matrix/PatternCustomBit.h"

//--------------------------------------------------------------
// Create the new FastLED GFX Canvas
GFXcanvas canvas(CANVAS_WIDTH, CANVAS_HEIGHT);

//--------------------------------------------------------------
// Create new pattern classes
#define MATRIX_NUM_PATTERNS 9
Drawable *matrixDraw[] = { new PatternFire(CANVAS_WIDTH, CANVAS_HEIGHT),
                          new PatternWave(CANVAS_WIDTH, CANVAS_HEIGHT),
                          new PatternSpiro(CANVAS_WIDTH, CANVAS_HEIGHT),
                          new PatternPlasma(CANVAS_WIDTH, CANVAS_HEIGHT),
                          new PatternSwirl(CANVAS_WIDTH, CANVAS_HEIGHT),
                          new PatternSpin(CANVAS_WIDTH, CANVAS_HEIGHT),
                          new PatternPulse(CANVAS_WIDTH, CANVAS_HEIGHT),
                          new PatternIncrementalDrift(CANVAS_WIDTH, CANVAS_HEIGHT),
                          new PatternBTS(CANVAS_WIDTH, CANVAS_HEIGHT),
                          new PatternCustomBit(CANVAS_WIDTH, CANVAS_HEIGHT)};

//--------------------------------------------------------------
// Setup Palettes
#define MATRIX_NUM_PALETTES 7
CRGBPalette16 matrixPalettes[] = {(CRGBPalette16)CloudColors_p, (CRGBPalette16)LavaColors_p, (CRGBPalette16)OceanColors_p,
                                  (CRGBPalette16)ForestColors_p, (CRGBPalette16)RainbowColors_p, (CRGBPalette16)RainbowStripeColors_p,
                                  (CRGBPalette16)HeatColors_p};

/* matrixSetup()
 * Method initializes the Fast LED Matrix Canvas
 */
void matrixSetup()
{
    FastLED.addLeds<MATRIX_CHIPSET, MATRIX_DATA_PIN, MATRIX_COLOR_ORDER>(canvas.getBuffer(), CANVAS_NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(MATRIX_BRIGHTNESS);

    canvas.setTextWrap(false);
    canvas.setTextSize(1);
}

/* matrixLoop()
 * Loops through as fast as possible to draw the next frame
 */
uint8_t matrixWhichOne = 0;
void matrixLoop()
{
    //--------------------------------------------------------------
    // If user elected to show timer, then only show timer
    if(timer.isShow())
    {
        timer.drawFrame(canvas);
        return;
    }
    //--------------------------------------------------------------
    // Determine which pattern to show
    matrixDraw[matrixWhichOne]->drawFrame(canvas);

    //--------------------------------------------------------------
    // Hold off on drawing till Matrix and LED loop have drawn next
    // frame
    //FastLED.show();

    //--------------------------------------------------------------
    // Switch pattern every 20 seconds
    EVERY_N_SECONDS(20)
    {
        canvas.fillScreen(CRGB::Black);
        matrixDraw[matrixWhichOne]->stop(canvas);
        matrixWhichOne = (matrixWhichOne == MATRIX_NUM_PATTERNS - 1) ? 0 : matrixWhichOne + 1;
        matrixDraw[matrixWhichOne]->start(canvas);
        matrixDraw[matrixWhichOne]->setPalette(matrixPalettes[random8(MATRIX_NUM_PALETTES)]);
    }
    
    //--------------------------------------------------------------
    // Draw scrolling frame
    scroll.drawFrame(canvas);
}
