#include <Arduino.h>
#include <Wire.h>

#include "bleServiceConfig.h"

BLEServer *pServer = NULL;
BLECharacteristic *pTxCharacteristic = NULL;
//int connectedClients = 0;
bool deviceConnected = false;
uint32_t value = 0;

extern void __vSafePrint(char *out);

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    //connectedClients++;
    //Serial.print("Client connected. Total clients: ");
    //Serial.println(connectedClients);
    // Continue advertising for more connections
    //BLEDevice::startAdvertising();  //disabling for single connection
    Serial.println("Device Connected");
    deviceConnected = true;

    //display.clearDisplay();
    //display.println("Device Connected");
    //display.display();
  };

  void onDisconnect(BLEServer *pServer) {
    //connectedClients--;
    //Serial.print("Client disconnected. Total clients: ");
    //Serial.println(connectedClients);
    Serial.println("Device Disconnected");
    /*if (connectedClients == 0)*/ {  //single connection
      deviceConnected = false;
    }
    
    //display.clearDisplay();
    //display.println("Device Disconnected");
    //display.display();
  }
};

class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    String rxValue(pCharacteristic->getValue().c_str());

    if (rxValue.length() > 0) {
      Serial.println("*********");
      Serial.print("Received Value: ");
      for (int i = 0; i < rxValue.length(); i++) {
        Serial.print(rxValue[i]);
      }

      Serial.println();
      Serial.println("*********");

      //display.clearDisplay();
      //display.println(rxValue);
      //display.display();
    }
  }
};

//"yyMMddHHmmssZ" (example, 010704120856-0700)
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

void bleCommunicationTask( void * pvParameters ){
   char printBuf[48];
   for(;;){
      if (deviceConnected) {
        sprintf(printBuf, "Notify value: %d from core: %d, Tick: %d\n", value, xPortGetCoreID(), xTaskGetTickCount());
        __vSafePrint(printBuf);
        pTxCharacteristic->setValue((uint8_t*)&value, 4);
        pTxCharacteristic->notify();
        value++;
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay a second between sends
      } else {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
      }
   } 
}

void bleSetup() {
  //BLE server setup
  BLEDevice::init("RA7NARAJM ESP32S3");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  //Navigation Direction service
  BLEService *pNavDirService = pServer->createService(NAVIGATION_DIRECTION_SERVICE_UUID);
  BLECharacteristic *pNavDirCharacteristic = pNavDirService->createCharacteristic(NAVIGATION_DIRECTION_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_WRITE);
  pNavDirCharacteristic->setCallbacks(new MyCallbacks());

  //Date update service
  BLEService *pNavDateService = pServer->createService(NAVIGATION_DATE_SERVICE_UUID);
  BLECharacteristic *pNavDateCharacteristic = pNavDateService->createCharacteristic(NAVIGATION_DATE_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_WRITE);
  pNavDateCharacteristic->setCallbacks(new NavDateServiceCallbacks());
  
  pNavDirService->start();
  pNavDateService->start();
  
  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(NAVIGATION_DIRECTION_SERVICE_UUID);
  pAdvertising->addServiceUUID(NAVIGATION_DATE_SERVICE_UUID);
  //pAdvertising->setScanResponse(true);
  //pAdvertising->setMinPreferred(0x00);  // functions that help with iPhone connections issue
  //pAdvertising->setMaxPreferred(0x1F);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}