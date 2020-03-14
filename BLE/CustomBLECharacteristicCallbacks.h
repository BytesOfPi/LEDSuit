/*
 * BLE Characteristic Callback
 * Here we receive a response from the phone to update the LED State
 */
#ifndef GetMatrixMsgCallbacks_H
#define GetMatrixMsgCallbacks_H

#include "../Matrix/MatrixUtility.h"
#include "../Strand/StrandUtility.h"

#define BLE_STRAND_CALLBACK 0x00
#define BLE_MATRIX_SCROLL_CALLBACK 0x01
#define BLE_MATRIX_TIMER_CALLBACK 0x02
#define BLE_MATRIX_PATTERN_CALLBACK 0x03

class CustomBLECharacteristicCallbacks : public BLECharacteristicCallbacks
{
private:
  uint8_t type;

public:
  //--------------------------------------------------------------
  // Constructor
  CustomBLECharacteristicCallbacks(uint8_t val) : BLECharacteristicCallbacks()
  {
    type = val;
  }

  void onWrite(BLECharacteristic *pCharacteristic)
  {
    // Get value from characteristic
    std::string value = pCharacteristic->getValue();

    // If nothing was sent, return early
    if (value.length() <= 0)
    {
      return;
    }
    // Convert value to a String
    String val = value.c_str();

    // Route the string to the appropriate BLE handler
    switch (type)
    {
    case BLE_MATRIX_PATTERN_CALLBACK:
      bleMatrixPattern(val);
      break;
    case BLE_MATRIX_SCROLL_CALLBACK:
      bleMatrixScroll(val);
      break;
    case BLE_MATRIX_TIMER_CALLBACK:
      bleMatrixTimer(val);
      break;
    case BLE_STRAND_CALLBACK:
      bleStrand(val);
      break;
    }
  }
};

#endif
