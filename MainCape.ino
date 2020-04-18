/* ################################################################################
 * Cape Main
 * The following file defines the main setup and loop for LEDs as well as shared
 * variables and methods
 */

//################################################################################
// Custom Cape Libraries
#include "Cape/Cape.h"

//--------------------------------------------------------------
// Create the Cape object
Cape* cape = new Cape();

/* capeSetup()
 * Method initializes the Fast LED Matrix Canvas
 */
void capeSetup()
{
    //--------------------------------------------------------------
    // Setup Cape LEDs
    cape->setup();
}

/* capeLoop()
 * Loops through as fast as possible to draw the next frame
 */
void capeLoop()
{
    //--------------------------------------------------------------
    // Draw Cape LEDs
    cape->drawFrame();
}
