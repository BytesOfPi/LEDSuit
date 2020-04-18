/* ################################################################################
 * Suit Main
 * The following file defines the main setup and loop for LEDs as well as shared
 * variables and methods
 */
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

#define DATA_PIN_001 4
#define DATA_PIN_002 16
#define DATA_PIN_003 17

#define NUM_LEDS_001 75
#define NUM_LEDS_002 150
#define NUM_LEDS_003 75

#define LED_INDEX_001 NUM_LEDS_001
#define LED_INDEX_002 NUM_LEDS_001 + NUM_LEDS_002
#define LED_INDEX_003 LED_INDEX_002 + NUM_LEDS_003
#define NUM_LEDS 300

#define BRIGHTNESS 96
#define FRAMES_PER_SECOND 120

#define TOT_SCROLL 30

//#################################################################
// Include Suit Methods
#include "Suit/Suit.h"

//--------------------------------------------------------------
// Create the Suit Object
Suit* suit = new Suit();

//#################################################################
// setup() and loop()
/*
 * ledSetup()
 * This method is invoked at the start to set up the LEDs and necessary values
 */
void ledSetup()
{
    //--------------------------------------------------------------
    // Setup Suit LEDs
    suit->setup();
}

/*
 * ledLoop()
 * This method is invoked each time to draw LEDs
 */
void ledLoop()
{
    //--------------------------------------------------------------
    // Draw Suit LEDs
    suit->drawFrame();
}
