#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

//i2c
#define SDA_GPIO  11 //10
#define SCL_GPIO  12 //8

//#define IMU_INT 13  //mpu6050 int

#define LED1_GPIO 41
#define LED2_GPIO 42


//BLE service configuration
/* Service description
   - Navigation Direction Service
     - Navigation Direction Characteristic Rx
   - Date Service
     - Date Characteristic Rx : reading "yyMMddHHmmssZ" (example, 010704120856-0700)
*/
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

//navigation service and characteristics
#define NAVIGATION_DIRECTION_SERVICE_UUID        "b8a71569-4d81-4146-9580-f3a4f2817cfb"
#define NAVIGATION_DIRECTION_CHARACTERISTIC_UUID "b8a71569-4d81-4146-9580-f3a4f2817ccc"

//date service and characteristics
#define NAVIGATION_DATE_SERVICE_UUID        "6ba7b810-9dad-11d1-80b4-00c04fd430c8"
#define NAVIGATION_DATE_CHARACTERISTIC_UUID "6ba7b811-9dad-11d1-80b4-00c04fd430ff"

#define SERVICE_UUID  "a2e12533-acba-46c7-b6cb-1379addd1fdd"
#define CHAR_UUID     "a2e12533-acba-46c7-b6cb-1379addd1fff"

//#define CHAR_UUID     "c8abbcc7-6d0e-405d-8802-7aff86ea21fe"

// reference: https://github.com/espressif/arduino-esp32/blob/master/libraries/BLE
// examples: https://github.com/nkolban/ESP32_BLE_Arduino/

BLEServer *pServer = NULL;
BLEService *pNavDirService = NULL;
BLECharacteristic *pNavDirCharacteristic = NULL;
// int connectedClients = 0;
bool deviceConnected = false;
uint8_t value = 0;

class NaviDisplayServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    // connectedClients++;
    // Serial.print("Client connected. Total clients: ");
    // Serial.println(connectedClients);
    //  Continue advertising for more connections
    // BLEDevice::startAdvertising();  //disabling for single connection
    Serial.println("Device Connected");
    deviceConnected = true;

    //displaySSD1306PrintNew(F("Device Connected"));
  }

  void onDisconnect(BLEServer *pServer) {
    // connectedClients--;
    // Serial.print("Client disconnected. Total clients: ");
    // Serial.println(connectedClients);
    Serial.println("Device Disconnected");
    /*if (connectedClients == 0)*/ { // single connection
      deviceConnected = false;
    }

    //displaySSD1306PrintNew(F("Device Disconnected"));
  }
};

class NavDirectionServiceCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    String rxValue(pCharacteristic->getValue().c_str());

    if (rxValue.length() > 0) {
      digitalWrite(LED2_GPIO, HIGH);
      Serial.print("********** RXValue: ");
      for (int i = 0; i < rxValue.length(); i++) {
        Serial.print(rxValue[i]);
      }
      //Serial.print(rxValue);

      //displaySSD1306PrintNew(rxValue);
    }
    Serial.println();
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

void bleRestartAdvertising(void) {
  /*
  BLEDevice::deinit(true);
  delay(1000);
  bleSetup();
  */
  if (!deviceConnected) {
    pServer->startAdvertising(); // restart advertising
    Serial.println("No clients connected, restarting advertising");
  }
}

void bleSetup(void) {
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

void bleNotifyValue(void) {
  if (deviceConnected) {
    pNavDirCharacteristic->setValue((uint8_t *)&value, 1);
    pNavDirCharacteristic->notify();
    value++;
    if (!(value % 8)) {
      digitalWrite(LED1_GPIO, LOW);
    } else {
      digitalWrite(LED1_GPIO, HIGH);
    }
    //add delay to avoid stack congestion TODO
  }
}

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()
{
  Serial.begin(115200);
  Wire.begin(SDA_GPIO, SCL_GPIO); // SDA, SCL

  pinMode(LED1_GPIO, OUTPUT);
  pinMode(LED2_GPIO, OUTPUT);

  bleSetup();
  delay(1000);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  #if 0
  // create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
      Task1code, /* Task function. */
      "Task1",   /* name of task. */
      2048,      /* Stack size of task */
      NULL,      /* parameter of the task */
      1,         /* priority of the task */
      &Task1,    /* Task handle to keep track of created task */
      0);        /* pin task to core 0 */

  // create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
      Task2code, /* Task function. */
      "Task2",   /* name of task. */
      2048,      /* Stack size of task */
      NULL,      /* parameter of the task */
      1,         /* priority of the task */
      &Task2,    /* Task handle to keep track of created task */
      1);        /* pin task to core 1 */
  #endif
  delay(500);
}

void loop()
{
  bleNotifyValue();
  bleRestartAdvertising();

  vTaskDelay(1000);
}