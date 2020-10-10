/* ################################################################################
 * Suit Main
 * The following file defines the main setup and loop for LEDs as well as shared
 * variables and methods
 */

//#################################################################
// Include Suit Methods
#include "Suit/Suit.h"

//--------------------------------------------------------------
// Create the Suit Object
Suit* suit = new Suit();

//#################################################################
// setup() and loop()
/*
 * suitSetup()
 * This method is invoked at the start to set up the LEDs and necessary values
 */
void suitSetup()
{
    //--------------------------------------------------------------
    // Setup Suit LEDs
    suit->setup();
}

/*
 * suitLoop()
 * This method is invoked each time to draw LEDs
 */
void suitLoop()
{
    //--------------------------------------------------------------
    // Check for global config change
    suit->checkGlobal();

    //--------------------------------------------------------------
    // Draw Suit LEDs
    suit->drawFrame();
}
