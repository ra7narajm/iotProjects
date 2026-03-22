#include <Arduino.h>

#include <Wire.h>
//#include <Adafruit_GFX.h>
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
#define LED3_GPIO 40

//BLE service configuration
/* Service description
   - Navigation Direction Service
     - Navigation Direction Characteristic Rx
   - Date Service
     - Date Characteristic Rx : reading "yyMMddHHmmssZ" (example, 010704120856-0700)
*/
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define ACCELERO_SERVICE_UUID "63f9f310-1d23-412b-b92c-6bf8bb1ee098"
#define ACCELERO_CHAR_X_UUID  "2b5f66d7-9dc0-472d-9fd5-98fc5374dcde"
#define ACCELERO_CHAR_Y_UUID  "1916e0ef-91ee-4039-aff9-89bea63a539a"
#define ACCELERO_CHAR_Z_UUID  "b5cb2604-c5fe-4da4-a928-7362ac23497d"

// reference: htps://github.com/espressif/arduino-esp32/blob/master/libraries/BLE
// examples: https://github.com/nkolban/ESP32_BLE_Arduino/

BLEServer *pServer = NULL;
BLEService *pAcceleroService = NULL;
BLECharacteristic *pAcceleroCharX = NULL;
BLECharacteristic *pAcceleroCharY = NULL;
BLECharacteristic *pAcceleroCharZ = NULL;
// int connectedClients = 0;
bool deviceConnected = false;
uint8_t value = 0;

class AcceleroServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    Serial.println("Device Connected");
    deviceConnected = true;
  }

  void onDisconnect(BLEServer *pServer) {
    Serial.println("Device Disconnected");
    /*if (connectedClients == 0)*/ { // single connection
      deviceConnected = false;
    }
  }
};

class AcceleroCharXCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    //String rxValue(pCharacteristic->getValue().c_str());
    int rxValue = pCharacteristic->getValue().toInt();
    Serial.printf("AcceleX: %d", rxValue);
    if (rxValue != 0) {
      digitalWrite(LED1_GPIO, HIGH);
    }
    
    #if 0
    if (rxValue.length() > 0) {
      digitalWrite(LED2_GPIO, HIGH);
      Serial.print("********** RXValue: ");
      for (int i = 0; i < rxValue.length(); i++) {
        Serial.print(rxValue[i]);
      }
      //Serial.print(rxValue);
    }
    Serial.println();
    #endif
  }
};

void bleRestartAdvertising(void) {
  if (!deviceConnected) {
    pServer->startAdvertising(); // restart advertising
    Serial.println("No clients connected, restarting advertising");
  }
}

void bleSetup(void) {
  // BLE server setup
  if (!BLEDevice::init("RA7NARAJMESP32S3")) {
    Serial.println("BLE initialization failed!");
    return;
  }
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new AcceleroServerCallbacks());

  // https://randomnerdtutorials.com/esp32-web-bluetooth/ BLE2902

  // Navigation Direction service
  pAcceleroService = pServer->createService(ACCELERO_SERVICE_UUID);
  pAcceleroCharX = pAcceleroService->createCharacteristic(ACCELERO_CHAR_X_UUID, BLECharacteristic::PROPERTY_WRITE);
  pAcceleroCharX->setCallbacks(new AcceleroCharXCallbacks());

  pAcceleroService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(ACCELERO_SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  // Set minimum preferred connection interval to 100ms (100 / 1.25 = 80)
  pAdvertising->setMinPreferred(0x80);
  // Set maximum preferred connection interval to 200ms (200 / 1.25 = 160 or 0xA0)
  pAdvertising->setMaxPreferred(0xA0);

  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

#if 0
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
#endif

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
  pinMode(LED3_GPIO, OUTPUT);

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
  //bleNotifyValue();
  bleRestartAdvertising();

  vTaskDelay(1000);
}