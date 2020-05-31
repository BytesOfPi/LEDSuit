/* ################################################################################
 * Main Method
 * The following file runs the main methods.  It contains all library include files
 * as well as defines the main setup() and loop() method. 
 */
//################################################################################
// Keypad Libraries
#include <Key.h>
#include <Keypad.h>

//################################################################################
// Bluetooth Low Energy (BLE) Libraries
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

//################################################################################
// FastLED Libraries
#include <FastLED.h>

FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif
//################################################################################
// Modified Adafruit GFX Libraries for Fast LED
#include <FastLED_GFX.h>

//################################################################################
// Shared variable libraries
#include "Shared/Share.h"

/*
 * setup() - Primary method used to initialize device and 
 * peripherals
 */
void setup()
{
  // Start serial connection
  Serial.begin(115200);
  Serial.println(F("Starting connection"));

  // 3 second delay before BLE setup
  delay(3000); 

  // Run Setup specific to BLE logic
  bleSetup();

  // 3 second delay to allow capacitor to fill up and LEDs to juice up.
  delay(3000); 

  // Run Setup specific to suit
  suitSetup();

  // LED Matrix setup
  matrixSetup();

  // Cape setup
  capeSetup();

  Serial.println(F("Setup complete!"));
}

/*
 * loop() - Run continuously until Arduino stops or is rebooted  
 */
void loop()
{
  // Check to see if we're changing any patterns from BLE Commands
  gConfig.checkNextPattern();

  // Run loop specific to Keyboard logic
  keyboardLoop();
  // keyboardCycleLoop();

  // Run loop specific to BLE logic
  bleLoop();

  // Run loop specific to LED logic
  suitLoop();

  // Run loop specific to matrix
  matrixLoop();
  
  // Run loop specific to cape
  capeLoop();

  // Send FastLED info out once rather than in ledLoop and matrixLoop
  FastLED.show();
}
