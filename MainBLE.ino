/* ################################################################################
 * Bluetooth Low Energy (BLE) methods
 * The following file defines methods used to allow a mobile device to change the
 * LED pattern or to tell the device when a pattern has changed.
 */

#define SERVICE_UUID "a5ff4b63-9be5-49c8-8ac4-3d376ae2cca3"

#define CHARACTERISTIC_GET_FULL_PATT_UUID "9082f7cb-c208-43d6-9848-e0802df34321"

#define CHARACTERISTIC_SET_SUIT_PATT_UUID "9082f7cb-c208-43d6-9848-e0802df31234"
#define CHARACTERISTIC_GET_SUIT_PATT_UUID "9082f7cb-c208-43d6-9848-e0802df301b6"
#define CHARACTERISTIC_GET_SUIT_PAL_UUID "9082f7cb-c208-43d6-9848-e0802df301b7"
#define CHARACTERISTIC_GET_SUIT_COL_UUID "9082f7cb-c208-43d6-9848-e0802df301b8"

#define CHARACTERISTIC_GET_MATRIX_PATT_UUID "9082f7cb-c208-43d6-9848-e0802df301b9"
#define CHARACTERISTIC_GET_MATRIX_PAL_UUID "9082f7cb-c208-43d6-9848-e0802df301ba"
#define CHARACTERISTIC_GET_MATRIX_COL_UUID "9082f7cb-c208-43d6-9848-e0802df301bb"

#define CHARACTERISTIC_GET_CAPE_PATT_UUID "9082f7cb-c208-43d6-9848-e0802df301bc"
#define CHARACTERISTIC_GET_CAPE_PAL_UUID "9082f7cb-c208-43d6-9848-e0802df301bd"
#define CHARACTERISTIC_GET_CAPE_PAL_SEC_UUID "9082f7cb-c208-43d6-9848-e0802df301be"
#define CHARACTERISTIC_GET_CAPE_COL_UUID "9082f7cb-c208-43d6-9848-e0802df301bf"

#define CHARACTERISTIC_GET_MATRIX_SCROLL_UUID "9082f7cb-c208-43d6-9848-e0802df301c0"
#define CHARACTERISTIC_GET_MATRIX_TIMER_UUID "9082f7cb-c208-43d6-9848-e0802df301c1"

#define BLE_SERVER_NAME "PandaLED"

bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

#include "BLE/BLEUtility.h"
#include "BLE/MyServerCallbacks.h"
#include "BLE/CustomBLECharacteristicCallbacks.h"

void bleSetup()
{
  Serial.begin(115200);

  Serial.println("Initializing Bluetooth...");
  // Initialize device and create a server
  BLEDevice::init(BLE_SERVER_NAME);
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  //--------------------------------------------------------------
  // Step 1: Create the service and set it up with the writable characteristic
  BLEService *pService = pServer->createService(SERVICE_UUID);

  //--------------------------------------------------------------
  // Step 2: Setup the Notify Pattern Message Characteristic
  // pCharSendStrandMsg defined in BLEUtility.h
  pCharSendStrandMsg = pService->createCharacteristic(
      CHARACTERISTIC_SET_SUIT_PATT_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE |
          BLECharacteristic::PROPERTY_NOTIFY |
          BLECharacteristic::PROPERTY_INDICATE);
  // Create a BLE Descriptor
  pCharSendStrandMsg->addDescriptor(new BLE2902());

  //--------------------------------------------------------------
  // Step 3: Setup the FULL Outfit Pattern Message Characteristics
  BLECharacteristic *pCharGetMsg = pService->createCharacteristic(
      CHARACTERISTIC_GET_FULL_PATT_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE);
  pCharGetMsg->setCallbacks(new CustomBLECharacteristicCallbacks(BLE_FULL_CALLBACK));
  pCharGetMsg->setValue("Control full outfit");

  //--------------------------------------------------------------
  // Step 4: Setup the Get PATTERN Message Characteristics
  pCharGetMsg = pService->createCharacteristic(
      CHARACTERISTIC_GET_SUIT_PATT_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE);
  pCharGetMsg->setCallbacks(new CustomBLECharacteristicCallbacks(BLE_SUIT_PATT_CALLBACK));
  pCharGetMsg->setValue("Control suit pattern by writing here");
  pCharGetMsg = pService->createCharacteristic(
      CHARACTERISTIC_GET_MATRIX_PATT_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE);
  pCharGetMsg->setCallbacks(new CustomBLECharacteristicCallbacks(BLE_MATRIX_PATT_CALLBACK));
  pCharGetMsg->setValue("Control matrix pattern by writing here");
  pCharGetMsg = pService->createCharacteristic(
      CHARACTERISTIC_GET_CAPE_PATT_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE);
  pCharGetMsg->setCallbacks(new CustomBLECharacteristicCallbacks(BLE_CAPE_PATT_CALLBACK));
  pCharGetMsg->setValue("Control cape pattern by writing here");

  //--------------------------------------------------------------
  // Step 5: Setup the Get PALETTE Message Characteristics
  pCharGetMsg = pService->createCharacteristic(
      CHARACTERISTIC_GET_SUIT_PAL_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE);
  pCharGetMsg->setCallbacks(new CustomBLECharacteristicCallbacks(BLE_SUIT_PAL_CALLBACK));
  pCharGetMsg->setValue("Control suit palette by writing here");
  pCharGetMsg = pService->createCharacteristic(
      CHARACTERISTIC_GET_MATRIX_PAL_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE);
  pCharGetMsg->setCallbacks(new CustomBLECharacteristicCallbacks(BLE_MATRIX_PAL_CALLBACK));
  pCharGetMsg->setValue("Control matrix palette by writing here");
  pCharGetMsg = pService->createCharacteristic(
      CHARACTERISTIC_GET_CAPE_PAL_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE);
  pCharGetMsg->setCallbacks(new CustomBLECharacteristicCallbacks(BLE_CAPE_PAL_CALLBACK));
  pCharGetMsg->setValue("Control cape palette by writing here");
  pCharGetMsg = pService->createCharacteristic(
      CHARACTERISTIC_GET_CAPE_PAL_SEC_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE);
  pCharGetMsg->setCallbacks(new CustomBLECharacteristicCallbacks(BLE_CAPE_PAL_SEC_CALLBACK));
  pCharGetMsg->setValue("Control cape 2 palette by writing here");

  //--------------------------------------------------------------
  // Step 6: Setup the Get COLOR Message Characteristics
  pCharGetMsg = pService->createCharacteristic(
      CHARACTERISTIC_GET_SUIT_COL_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE);
  pCharGetMsg->setCallbacks(new CustomBLECharacteristicCallbacks(BLE_SUIT_COL_CALLBACK));
  pCharGetMsg->setValue("Control suit color by writing here");
  pCharGetMsg = pService->createCharacteristic(
      CHARACTERISTIC_GET_MATRIX_COL_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE);
  pCharGetMsg->setCallbacks(new CustomBLECharacteristicCallbacks(BLE_MATRIX_COL_CALLBACK));
  pCharGetMsg->setValue("Control matrix color by writing here");
  pCharGetMsg = pService->createCharacteristic(
      CHARACTERISTIC_GET_CAPE_COL_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE);
  pCharGetMsg->setCallbacks(new CustomBLECharacteristicCallbacks(BLE_CAPE_COL_CALLBACK));
  pCharGetMsg->setValue("Control cape color by writing here");

  //--------------------------------------------------------------
  // Step 7: Setup the Get LED Matrix SCROLL Message Characteristic
  pCharGetMsg = pService->createCharacteristic(
      CHARACTERISTIC_GET_MATRIX_SCROLL_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE);
  pCharGetMsg->setCallbacks(new CustomBLECharacteristicCallbacks(BLE_MATRIX_SCROLL_CALLBACK));
  pCharGetMsg->setValue("Matrix Scroll");

  //--------------------------------------------------------------
  // Step 8: Setup the Get LED Matrix TIMER Message Characteristic
  pCharGetMsg = pService->createCharacteristic(
      CHARACTERISTIC_GET_MATRIX_TIMER_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE);
  pCharGetMsg->setCallbacks(new CustomBLECharacteristicCallbacks(BLE_MATRIX_TIMER_CALLBACK));
  pCharGetMsg->setValue("Matrix Timer");

  //--------------------------------------------------------------
  // Step 9: Start Service
  pService->start();

  //--------------------------------------------------------------
  // Step 10: Start Advertising
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);
  pAdvertising->start();
  Serial.println("Bluetooth init complete.");
}

/*
 * BLE loop logic
 * Here we specify what we detect from the keypad and act upon it
 */
void bleLoop()
{
  // disconnecting
  if (!deviceConnected && oldDeviceConnected)
  {
    delay(500);                  // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising(); // restart advertising
    Serial.println("start advertising");
    oldDeviceConnected = deviceConnected;
  }
  // connecting
  if (deviceConnected && !oldDeviceConnected)
  {
    // do stuff here on connecting
    oldDeviceConnected = deviceConnected;
  }
}
