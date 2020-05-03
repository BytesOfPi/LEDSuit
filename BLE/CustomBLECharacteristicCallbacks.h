/*
 * CustomBLECharacteristicCallbacks.h: https://github.com/BytesOfPi/LEDSuit
 * Copyright (c) 2020 Nathan DeGroff
 *
 * Overview:
 * Here we receive a response from the phone to update the LED State
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef GetMatrixMsgCallbacks_H
#define GetMatrixMsgCallbacks_H

#include "../Shared/Share.h"
#include "../Matrix/MatrixUtility.h"

void fullPattern(String val)
{
  if (val.equals(FULL_PATT_CYCLE))
  {
    gConfig.holdSuitPattern = SUIT_PATT_CYCLE;
    gConfig.holdMatrixPattern = MATRIX_PATT_CYCLE;
    gConfig.holdCapePattern = CAPE_PATT_CYCLE;
    return;
  }
  if (val.equals(FULL_PATT_BPM))
  {
    gConfig.holdSuitPattern = SUIT_PATT_BPM_PARTY;
    gConfig.holdMatrixPattern = MATRIX_PATT_WAVE;
    gConfig.holdCapePattern = CAPE_PATT_BPM;
    return;
  }
  if (val.equals(FULL_PATT_CCHS))
  {
    gConfig.holdSuitPattern = SUIT_PATT_CCHS;
    gConfig.holdMatrixPattern = MATRIX_PATT_PULSE;
    gConfig.holdCapePattern = CAPE_PATT_JUGGLE;
    return;
  }
  if (val.equals(FULL_PATT_JUGGLE))
  {
    gConfig.holdSuitPattern = SUIT_PATT_JUGGLE;
    gConfig.holdMatrixPattern = MATRIX_PATT_FIRE;
    gConfig.holdCapePattern = CAPE_PATT_JUGGLE;
    return;
  }
  if (val.equals(FULL_PATT_BTS))
  {
    gConfig.holdSuitPattern = SUIT_PATT_CONFETTI;
    gConfig.holdMatrixPattern = MATRIX_PATT_BTS;
    gConfig.holdCapePattern = CAPE_PATT_SPARKLE;
    return;
  }
}

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
    String val = String(value.c_str());

    // bleNotifyStrandMsg(val);

    // Route the string to the appropriate BLE handler
    switch (type)
    {
    // Set component pattern
    case BLE_FULL_CALLBACK:
      // gConfig.holdFull = val;
      fullPattern(val);
      break;
    // Set component pattern
    case BLE_SUIT_PATT_CALLBACK:
      gConfig.holdSuitPattern = val;
      break;
    case BLE_MATRIX_PATT_CALLBACK:
      gConfig.holdMatrixPattern = val;
      break;
    case BLE_CAPE_PATT_CALLBACK:
      //blePattern(val, type);
      gConfig.holdCapePattern = val;
      break;

    // Check Prefix and strip off prefix before routing
    case BLE_PAL_COL_CALLBACK:
      if (val.startsWith(BLE_PREFIX_SUIT_PAL))
        gConfig.holdSuitPal = val.substring(1);
      else if (val.startsWith(BLE_PREFIX_SUIT_COL))
        gConfig.holdSuitCol = val.substring(1);
      else if (val.startsWith(BLE_PREFIX_MATRIX_PAL))
        gConfig.holdMatrixPal = val.substring(1);
      else if (val.startsWith(BLE_PREFIX_MATRIX_COL))
        gConfig.holdMatrixCol = val.substring(1);
      else if (val.startsWith(BLE_PREFIX_CAPE_PAL))
        gConfig.holdCapePal = val.substring(1);
      else if (val.startsWith(BLE_PREFIX_CAPE_PAL_SEC))
        gConfig.holdCapeSecPal = val.substring(1);
      else if (val.startsWith(BLE_PREFIX_CAPE_COL))
        gConfig.holdCapeCol = val.substring(1);
      break;
    }
    
    //--------------------------------------------------------------
    // Signal to main program that a change has been made
    gConfig.change = true;
  }
};

#endif
