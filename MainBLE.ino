/* ################################################################################
 * Bluetooth Low Energy (BLE) methods
 * The following file defines methods used to allow a mobile device to change the
 * LED pattern or to tell the device when a pattern has changed.
 */

#define SERVICE_UUID "a5ff4b63-9be5-49c8-8ac4-3d376ae2cca3"

#define CHARACTERISTIC_GET_FULL_PATT_UUID "9082f7cb-c208-43d6-9848-e0802df34321"

#define CHARACTERISTIC_SET_SUIT_PATT_UUID "ecd0fe34-fa4c-4cd0-9a4e-991d57ef5008"
#define CHARACTERISTIC_GET_SUIT_PATT_UUID "2539da47-36d5-4e26-8789-621e683aa294"
#define CHARACTERISTIC_GET_MATRIX_PATT_UUID "1c29d445-46e7-40c1-ad45-e87d10f4ba79"
#define CHARACTERISTIC_GET_CAPE_PATT_UUID "e6f0ebf4-6b06-4d0a-a3a0-d1c91e7b5919"

#define CHARACTERISTIC_GET_PAL_COL_UUID "66efc725-2d5b-4e88-a381-cd6cb4d78cfb"

#define CHARACTERISTIC_GET_MATRIX_SCROLL_UUID "799b6016-8120-4b10-b6c3-0c58178ebd00"
#define CHARACTERISTIC_GET_MATRIX_TIMER_UUID "a8ea0d63-4c4d-4535-a420-fb97053d32b8"

#define BLE_SERVER_NAME "PandaLED"

bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

#include "BLE/BLEUtility.h"
#include "BLE/MyServerCallbacks.h"
#include "BLE/CustomBLECharacteristicCallbacks.h"

void createBLECharacteristic(BLEService *pService, const char *uuid, uint8_t bleID, const char *msg)
{
    BLECharacteristic *pCharGetMsg = pService->createCharacteristic(
        uuid, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    pCharGetMsg->setCallbacks(new CustomBLECharacteristicCallbacks(bleID));
    pCharGetMsg->setValue(msg);
}

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
    // pCharSendStrandMsg = pService->createCharacteristic(
    //     CHARACTERISTIC_SET_SUIT_PATT_UUID,
    //     BLECharacteristic::PROPERTY_READ |
    //         BLECharacteristic::PROPERTY_WRITE |
    //         BLECharacteristic::PROPERTY_NOTIFY |
    //         BLECharacteristic::PROPERTY_INDICATE);
    // // Create a BLE Descriptor
    // pCharSendStrandMsg->addDescriptor(new BLE2902());

    //--------------------------------------------------------------
    // Step 3: Setup the FULL Outfit Pattern Message Characteristics
    createBLECharacteristic(pService,
                            CHARACTERISTIC_GET_FULL_PATT_UUID,
                            BLE_FULL_CALLBACK,
                            "Full outfit");

    //--------------------------------------------------------------
    // Step 4: Setup the Get PATTERN Message Characteristics
    createBLECharacteristic(pService,
                            CHARACTERISTIC_GET_SUIT_PATT_UUID,
                            BLE_SUIT_PATT_CALLBACK,
                            "Suit Pattern");
    createBLECharacteristic(pService,
                            CHARACTERISTIC_GET_MATRIX_PATT_UUID,
                            BLE_MATRIX_PATT_CALLBACK,
                            "Matrix Pattern");
    createBLECharacteristic(pService,
                            CHARACTERISTIC_GET_CAPE_PATT_UUID,
                            BLE_CAPE_PATT_CALLBACK,
                            "Cape Pattern");

    //--------------------------------------------------------------
    // Step 5: Setup the Get PALETTE Message Characteristics
    createBLECharacteristic(pService,
                            CHARACTERISTIC_GET_PAL_COL_UUID,
                            BLE_PAL_COL_CALLBACK,
                            "SuitPal");
    /*
    createBLECharacteristic(pService,
                            CHARACTERISTIC_GET_MATRIX_PAL_UUID,
                            BLE_MATRIX_PAL_CALLBACK,
                            "MatrixPal");
    createBLECharacteristic(pService,
                            CHARACTERISTIC_GET_CAPE_PAL_UUID,
                            BLE_CAPE_PAL_CALLBACK,
                            "CapePal1");
    createBLECharacteristic(pService,
                            CHARACTERISTIC_GET_CAPE_PAL_SEC_UUID,
                            BLE_CAPE_PAL_SEC_CALLBACK,
                            "CapePal2");

    //--------------------------------------------------------------
    // Step 6: Setup the Get COLOR Message Characteristics
    createBLECharacteristic(pService,
                            CHARACTERISTIC_GET_SUIT_COL_UUID,
                            BLE_SUIT_COL_CALLBACK,
                            "Suit Color");
    createBLECharacteristic(pService,
                            CHARACTERISTIC_GET_MATRIX_COL_UUID,
                            BLE_MATRIX_COL_CALLBACK,
                            "Matrix Color");
    createBLECharacteristic(pService,
                            CHARACTERISTIC_GET_CAPE_COL_UUID,
                            BLE_CAPE_COL_CALLBACK,
                            "Cape Color");

    //--------------------------------------------------------------
    // Step 7: Setup the Get LED Matrix SCROLL Message Characteristic
    createBLECharacteristic(pService,
                            CHARACTERISTIC_GET_MATRIX_SCROLL_UUID,
                            BLE_MATRIX_SCROLL_CALLBACK,
                            "Matrix Scroll");
    // pCharGetMsg = pService->createCharacteristic(
    //     CHARACTERISTIC_GET_MATRIX_SCROLL_UUID,
    //     BLECharacteristic::PROPERTY_READ |
    //         BLECharacteristic::PROPERTY_WRITE);
    // pCharGetMsg->setCallbacks(new CustomBLECharacteristicCallbacks(BLE_MATRIX_SCROLL_CALLBACK));
    // pCharGetMsg->setValue("Matrix Scroll");

    /*
    //--------------------------------------------------------------
    // Step 8: Setup the Get LED Matrix TIMER Message Characteristic
    pCharGetMsg = pService->createCharacteristic(
        CHARACTERISTIC_GET_MATRIX_TIMER_UUID,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE);
    pCharGetMsg->setCallbacks(new CustomBLECharacteristicCallbacks(BLE_MATRIX_TIMER_CALLBACK));
    pCharGetMsg->setValue("Matrix Timer");
    */
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
