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
    gConfig.holdSuitPattern = SUIT_PATT_BPM;
    gConfig.holdSuitPal = PALETTE_PARTY.name;
    gConfig.holdMatrixPattern = MATRIX_PATT_WAVE;
    gConfig.holdCapePattern = CAPE_PATT_BPM;
    return;
  }
  if (val.equals(FULL_PATT_CCHS))
  {
    gConfig.holdSuitPattern = SUIT_PATT_PALETTE;
    gConfig.holdSuitPal = PALETTE_CCHS.name;
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
public:
  //--------------------------------------------------------------
  // Constructor
  CustomBLECharacteristicCallbacks() : BLECharacteristicCallbacks()
  {
  }

  void onWrite(BLECharacteristic *pCharacteristic)
  {
    // Get value from characteristic
    std::string value = pCharacteristic->getValue();
    String thisCharUUID = String(pCharacteristic->getUUID().toString().c_str());

    // If nothing was sent, return early
    if (value.length() <= 0)
    {
      return;
    }
    // Convert value to a String
    String val = String(value.c_str());

    // Consolidating Characteristics into one with a prefix
    // Evaluate prefix before routing to next path
    if (String(CHARACTERISTIC_GET_FULL_PATT_UUID).equals(thisCharUUID))
    {
      String trimVal = val.substring(1);
      switch (val.charAt(0))
      {
      // '0' = Full outfit pattern
      case '0':
        fullPattern(trimVal);
        break;
      // '1' = Body suit pattern
      case '1':
        gConfig.holdSuitPattern = trimVal;
        break;
      // '2' = Matrix pattern
      case '2':
        gConfig.holdMatrixPattern = trimVal;
        break;
      // '3' = Cape pattern
      case '3':
        gConfig.holdCapePattern = trimVal;
        break;
      // '4' = Palette/Color pattern
      case '4':
        char prefix = trimVal.charAt(0);
        trimVal = trimVal.substring(1);
        Serial.print("PREFIX: ");
        Serial.print(String(prefix));
        switch (prefix)
        {
        case BLE_PREFIX_SUIT_PAL:
          gConfig.holdSuitPal = trimVal;
          break;
        case BLE_PREFIX_SUIT_COL:
          gConfig.holdSuitCol = trimVal;
          break;
        case BLE_PREFIX_SUIT_COL_2:
          gConfig.holdSuitCol2 = trimVal;
          break;
        case BLE_PREFIX_MATRIX_PAL:
          gConfig.holdMatrixPal = trimVal;
          break;
        case BLE_PREFIX_MATRIX_COL:
          gConfig.holdMatrixCol = trimVal;
          break;
        case BLE_PREFIX_CAPE_PAL:
          gConfig.holdCapePal = trimVal;
          break;
        case BLE_PREFIX_CAPE_PAL_2:
          gConfig.holdCapeSecPal = trimVal;
          break;
        case BLE_PREFIX_CAPE_COL:
          gConfig.holdCapeCol = trimVal;
          break;
        }
      }
    }
    // Scroll Command
    else if (String(CHARACTERISTIC_GET_MATRIX_SCROLL_UUID).equals(thisCharUUID))
    {
      bleMatrixScroll(val);
    }
    //--------------------------------------------------------------
    // Signal to main program that a change has been made
    gConfig.change = true;

  }
};

#endif
