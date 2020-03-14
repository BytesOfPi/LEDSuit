/*
 * BLE Server Callback
 * These functions are called when a device connects and disconnects
 */
#ifndef MyServerCallbacks_H
#define MyServerCallbacks_H

class MyServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer *pServer)
  {
    deviceConnected = false;
  }
};

#endif
