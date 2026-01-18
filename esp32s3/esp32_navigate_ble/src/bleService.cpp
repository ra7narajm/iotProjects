#include <Arduino.h>
#include <Wire.h>

#include "serialPrint.h"
#include "bleServiceConfig.h"
#include "displaySSD1306.h"

// reference: https://github.com/espressif/arduino-esp32/blob/master/libraries/BLE
// examples: https://github.com/nkolban/ESP32_BLE_Arduino/

BLEServer *pServer = NULL;
BLEService *pNavDirService = NULL;
BLECharacteristic *pNavDirCharacteristic = NULL;
// int connectedClients = 0;
bool deviceConnected = false;
uint8_t value = 0;

// extern void __vSafePrint(char *out);

class NaviDisplayServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    // connectedClients++;
    // Serial.print("Client connected. Total clients: ");
    // Serial.println(connectedClients);
    //  Continue advertising for more connections
    // BLEDevice::startAdvertising();  //disabling for single connection
    // Serial.println("Device Connected");
    __vSafeSerialPrintln("Device Connected");
    deviceConnected = true;

    displaySSD1306PrintNew(F("Device Connected"));
  }

  void onDisconnect(BLEServer *pServer)
  {
    // connectedClients--;
    // Serial.print("Client disconnected. Total clients: ");
    // Serial.println(connectedClients);
    // Serial.println("Device Disconnected");
    __vSafeSerialPrintln("Device Disconnected");
    /*if (connectedClients == 0)*/ { // single connection
      deviceConnected = false;
    }

    displaySSD1306PrintNew(F("Device Disconnected"));
  }
};

class NavDirectionServiceCallbacks : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    String rxValue(pCharacteristic->getValue().c_str());

    if (rxValue.length() > 0)
    {
      __vSafeSerialPrint(F("*********"));
      __vSafeSerialPrintln(rxValue);

      displaySSD1306PrintNew(rxValue);
    }
  }
};

//"yyMMddHHmmssZ" (example, 010704120856-0700)
/*
class NavDateServiceCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    String rxValue(pCharacteristic->getValue().c_str());

    if (rxValue.length() > 0) {
      Serial.println("*********");
      Serial.print("Received Date Value: ");
      for (int i = 0; i < rxValue.length(); i++) {
        Serial.print(rxValue[i]);
      }

      Serial.println();
      Serial.println("*********");

      //display.clearDisplay();
      //display.println(rxValue);
      //display.display();
    }
    BT_IN = pCharacteristic->getValue();
    if (BT_IN.length() == 15) {
    pCharacteristic->setValue("");
    inString[0]= BT_IN[0];  //Y
    inString[1]= BT_IN[1];  //Y
    inString[2]= BT_IN[2];  //M
    inString[3]= BT_IN[3];  //M
    inString[4]= BT_IN[4];  //D
    inString[5]= BT_IN[5];  //D
    inString[6]= BT_IN[6];  //w     XXX Day of week not used
    inString[7]= BT_IN[7];  //H
    inString[8]= BT_IN[8];  //H
    inString[9]= BT_IN[9];  //M
    inString[10]= BT_IN[10];//M
    inString[11]= BT_IN[11];//S
    inString[12]= BT_IN[12];//S

    Year = (((inString[0] -48)*10)+(inString[1] -48));
    // now month
    Month = (((inString[2] -48)*10)+(inString[3] -48));
    // now date
    Date = (((inString[4] -48)*10)+(inString[5] -48));
    // now Day of Week
    Dow = (inString[6] -48);
    // now hour
    Hour = (((inString[7] -48)*10)+(inString[8] -48));
    // now minute
    Minute = (((inString[9] -48)*10)+(inString[10] -48));
    // now second
    Second = (((inString[11] -48)*10)+(inString[12] -48));

    myRTC.setClockMode(false);// set to 24h
    myRTC.setYear(Year);
    myRTC.setMonth(Month);
    myRTC.setDate(Date);
    myRTC.setDoW(Dow);
    myRTC.setHour(Hour);
    myRTC.setMinute(Minute);
    myRTC.setSecond(Second);
    myRTC.turnOffAlarm(1);
    myRTC.turnOffAlarm(2);
    Serial.println("The Time Has Been Set");
    delay(600);
    BLEDevice:: deinit(true);
    Serial.println("Bluetooth has been turned off");
    delay(600);             // Allow RTC registers to set before any read occurs

    counter = 0;
    }
  }
};
*/

void bleRestartAdvertising(void)
{
  /*
  BLEDevice::deinit(true);
  delay(1000);
  bleSetup();
  */
  if (!deviceConnected)
  {
    pServer->startAdvertising(); // restart advertising
    __vSafeSerialPrintln(F("No clients connected, restarting advertising"));
    // Serial.println("No clients connected, restarting advertising");
  }
}

void bleSetup(void)
{
  // BLE server setup
  BLEDevice::init("RA7NARAJM ESP32S3");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new NaviDisplayServerCallbacks());

  // https://randomnerdtutorials.com/esp32-web-bluetooth/ BLE2902

  // Navigation Direction service
  pNavDirService = pServer->createService(NAVIGATION_DIRECTION_SERVICE_UUID);
  pNavDirCharacteristic = pNavDirService->createCharacteristic(NAVIGATION_DIRECTION_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_WRITE|BLECharacteristic::PROPERTY_NOTIFY);
  pNavDirCharacteristic->setCallbacks(new NavDirectionServiceCallbacks());
  pNavDirCharacteristic->addDescriptor(new BLE2902());

  // Date update service
  // BLEService *pNavDateService = pServer->createService(NAVIGATION_DATE_SERVICE_UUID);
  // BLECharacteristic *pNavDateCharacteristic = pNavDateService->createCharacteristic(NAVIGATION_DATE_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_WRITE);
  // pNavDateCharacteristic->setCallbacks(new NavDateServiceCallbacks());

  pNavDirService->start();
  // pNavDateService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(NAVIGATION_DIRECTION_SERVICE_UUID);
  // pAdvertising->addServiceUUID(NAVIGATION_DATE_SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  // Set minimum preferred connection interval to 100ms (100 / 1.25 = 80)
  pAdvertising->setMinPreferred(0x80);
  // Set maximum preferred connection interval to 200ms (200 / 1.25 = 160 or 0xA0)
  pAdvertising->setMaxPreferred(0xA0);

  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void bleNotifyValue(void)
{
  if (deviceConnected)
  {
    pNavDirCharacteristic->setValue((uint8_t *)&value, 1);
    pNavDirCharacteristic->notify();
    value++;
    //add delay to avoid stack congestion TODO
  }
}