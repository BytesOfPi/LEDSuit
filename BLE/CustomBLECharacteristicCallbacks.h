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
      bleSuitPattern(val);
      break;
    case BLE_MATRIX_PATT_CALLBACK:
      currentMatrixPattern = val;
      break;
    case BLE_CAPE_PATT_CALLBACK:
      currentCapePattern = val;
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
      setupMatrix(SUIT_PATT_BPM_PARTY, PALETTE_PARTY);
      setupCape(MATRIX_PATT_WAVE, PALETTE_PARTY);
      return;
    }
    if (val.equals(FULL_PATT_CCHS))
    {
      setupSuit(SUIT_PATT_BPM_PARTY, PALETTE_CCHS);
      setupMatrix(SUIT_PATT_BPM_PARTY, PALETTE_CCHS);
      setupCape(MATRIX_PATT_WAVE, PALETTE_CCHS, PALETTE_CCHS);
      return;
    }
  }

  void setupSuit(String patt, MPal pal = PALETTE_PARTY, MCol col = COLOR_BLUE)
  {
    bleSuitPattern(patt);
    currentSuitPalette = pal;
    currentSuitColor = col;
  }
  void setupMatrix(String patt, MPal pal = PALETTE_PARTY, MCol col = COLOR_BLUE)
  {
    currentMatrixPattern = patt;
    currentMatrixPalette = pal;
    currentMatrixColor = col;
  }
  void setupCape(String patt, MPal pal = PALETTE_PARTY, MPal pal2 = PALETTE_PARTY, MCol col = COLOR_BLUE)
  {
    currentCapePattern = patt;
    currentCapePalette = pal;
    currentCapeSecPalette = pal2;
    currentCapeColor = col;
  }
};

#endif
