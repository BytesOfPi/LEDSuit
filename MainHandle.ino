/* ################################################################################
 * Button Main
 * The following file defines the main setup and loop for LEDs as well as shared
 * variables and methods
 */

//################################################################################
// Custom Button Libraries
#include "HandleControl/HandleControl.h"

//--------------------------------------------------------------
// Create the HandleControl object
HandleControl *handleControl = new HandleControl();

/* controlSetup()
 * Method initializes button pins
 */
void controlSetup()
{
    //--------------------------------------------------------------
    // Setup Handle Control
    handleControl->setup();
}

/* controlLoop()
 * Loops through and determines button presses
 */
void controlLoop()
{
    //--------------------------------------------------------------
    // Check to see if buttons have been pressed
    handleControl->triggerPress();
    handleControl->handle1Press();
}
