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

void setupSuit(String val, MPal pal = PALETTE_PARTY, MCol col = COLOR_BLUE)
{
  gConfig.currentSuitPattern->setPattern(val);
  gConfig.currentSuitPalette = pal;
  gConfig.currentSuitColor = col;
  Serial.print("FULL: Suit[");
  Serial.print(val);
  Serial.print("][");
  Serial.print(pal.name);
  Serial.print("][");
  Serial.print(col.name);
  Serial.print("]");
}
void setupMatrix(String val, MPal pal = PALETTE_PARTY, MCol col = COLOR_BLUE)
{
  gConfig.currentMatrixPattern->setPattern(val);
  gConfig.currentMatrixPalette = pal;
  gConfig.currentMatrixColor = col;
  Serial.print("FULL: Matrix[");
  Serial.print(val);
  Serial.print("][");
  Serial.print(pal.name);
  Serial.print("][");
  Serial.print(col.name);
  Serial.print("]");
}
void setupCape(String val, MPal pal = PALETTE_PARTY, MPal pal2 = PALETTE_PARTY, MCol col = COLOR_BLUE)
{
  gConfig.currentCapePattern->setPattern(val);
  gConfig.currentCapePalette = pal;
  gConfig.currentCapeSecPalette = pal2;
  gConfig.currentCapeColor = col;
  Serial.print("FULL: Cape[");
  Serial.print(val);
  Serial.print("][");
  Serial.print(pal.name);
  Serial.print("][");
  Serial.print(pal2.name);
  Serial.print("][");
  Serial.print(col.name);
  Serial.print("]");
}

void fullPattern(String val)
{
  if (val.equals(FULL_PATT_CYCLE))
  {
    setupSuit(SUIT_PATT_CYCLE);
    setupMatrix(MATRIX_PATT_CYCLE);
    setupCape(CAPE_PATT_CYCLE);
    return;
  }
  if (val.equals(FULL_PATT_BPM))
  {
    setupSuit(SUIT_PATT_BPM_PARTY, PALETTE_PARTY);
    setupMatrix(MATRIX_PATT_WAVE, PALETTE_PARTY);
    setupCape(CAPE_PATT_BPM, PALETTE_PARTY);
    return;
  }
  if (val.equals(FULL_PATT_CCHS))
  {
    setupSuit(SUIT_PATT_CCHS, PALETTE_CCHS);
    setupMatrix(MATRIX_PATT_PULSE, PALETTE_CCHS);
    setupCape(CAPE_PATT_JUGGLE, PALETTE_CCHS, PALETTE_CCHS);
    return;
  }
  if (val.equals(FULL_PATT_JUGGLE))
  {
    setupSuit(SUIT_PATT_JUGGLE, PALETTE_PARTY);
    setupMatrix(MATRIX_PATT_FIRE, PALETTE_HEAT);
    setupCape(CAPE_PATT_JUGGLE, PALETTE_PARTY, PALETTE_PARTY);
    return;
  }
  if (val.equals(FULL_PATT_BTS))
  {
    setupSuit(SUIT_PATT_CONFETTI, PALETTE_PARTY);
    setupMatrix(MATRIX_PATT_BTS, PALETTE_HEAT);
    setupCape(CAPE_PATT_SPARKLE, PALETTE_PARTY, PALETTE_PARTY);
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
      fullPattern(val);
      break;
    // Set component pattern
    case BLE_SUIT_PATT_CALLBACK:
    case BLE_MATRIX_PATT_CALLBACK:
    case BLE_CAPE_PATT_CALLBACK:
      blePattern(val, type);
      break;

    // Set component palette
    case BLE_SUIT_PAL_CALLBACK:
    case BLE_MATRIX_PAL_CALLBACK:
    case BLE_CAPE_PAL_CALLBACK:
    case BLE_CAPE_PAL_SEC_CALLBACK:
      blePalette(val, type);
      break;

    // Set component color
    case BLE_SUIT_COL_CALLBACK:
    case BLE_MATRIX_COL_CALLBACK:
    case BLE_CAPE_COL_CALLBACK:
      bleColor(val, type);
      break;
    }
  }
};

#endif
