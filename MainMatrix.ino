/* ################################################################################
 * LED Main
 * The following file defines the main setup and loop for LEDs as well as shared
 * variables and methods
 */

//################################################################################
// Custom Cape Libraries
#include "Matrix/Matrix.h"

//--------------------------------------------------------------
// Create the Cape object
Matrix* matrix = new Matrix();

/* matrixSetup()
 * Method initializes the Fast LED Matrix Canvas
 */
void matrixSetup()
{
    //--------------------------------------------------------------
    // Setup Matrix LEDs and class
    matrix->setup();
}

/* matrixLoop()
 * Loops through as fast as possible to draw the next frame
 */
// uint8_t matrixWhichOne = 0;
void matrixLoop()
{
    // //--------------------------------------------------------------
    // // If user elected to show timer, then only show timer
    // if(timer.isShow())
    // {
    //     timer.drawFrame(canvas);
    //     return;
    // }

    //--------------------------------------------------------------
    // Setup Matrix LEDs and class
    matrix->drawFrame();

    // //--------------------------------------------------------------
    // // Draw scrolling frame
    // scroll.drawFrame(canvas);
}
