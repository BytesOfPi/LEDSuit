/*
 * BLE Characteristic Callback
 * Here we receive a response from the phone to update the LED State
 */
#ifndef GetMatrixMsgCallbacks_H
#define GetMatrixMsgCallbacks_H

#include "../Shared/Share.h"
#include "../Suit/SuitUtility.h"
#include "../Matrix/MatrixUtility.h"

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

  void setupSuit(String patt, MPal pal = PALETTE_PARTY, MCol col = COLOR_BLUE)
  {
    gConfig.currentSuitPattern->setPattern(patt);
    gConfig.currentSuitPalette = pal;
    gConfig.currentSuitColor = col;
    Serial.print("FULL: Suit[");
    Serial.print(patt);
    Serial.print("][");
    Serial.print(pal.name);
    Serial.print("][");
    Serial.print(col.name);
    Serial.print("]");
  }
  void setupMatrix(String patt, MPal pal = PALETTE_PARTY, MCol col = COLOR_BLUE)
  {
    gConfig.currentMatrixPattern->setPattern(patt);
    gConfig.currentMatrixPalette = pal;
    gConfig.currentMatrixColor = col;
    Serial.print("FULL: Matrix[");
    Serial.print(patt);
    Serial.print("][");
    Serial.print(pal.name);
    Serial.print("][");
    Serial.print(col.name);
    Serial.print("]");
  }
  void setupCape(String patt, MPal pal = PALETTE_PARTY, MPal pal2 = PALETTE_PARTY, MCol col = COLOR_BLUE)
  {
    gConfig.currentCapePattern->setPattern(patt);
    gConfig.currentCapePalette = pal;
    gConfig.currentCapeSecPalette = pal2;
    gConfig.currentCapeColor = col;
    Serial.print("FULL: Cape[");
    Serial.print(patt);
    Serial.print("][");
    Serial.print(pal.name);
    Serial.print("][");
    Serial.print(pal2.name);
    Serial.print("][");
    Serial.print(col.name);
    Serial.print("]");
  }
};

#endif
