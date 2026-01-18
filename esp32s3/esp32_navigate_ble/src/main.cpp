#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#include "pinConfig.h"
#include "bleServiceConfig.h"
#include "serialPrint.h"
#include "displaySSD1306.h"

TaskHandle_t Task1;
TaskHandle_t Task2;
TaskHandle_t bleCommTask;

// TaskStatus_t Task1Details;
// TaskStatus_t Task2Details;

// LED pins
const int LED1 = 39;
const int LED2 = 40;
const int LED3 = 41;
const int LED4 = 42;

// Task1code: blinks an LED every 1500 ms
void Task1code(void *pvParameters)
{
  char printBuf[48];
  for (;;)
  {
    // sprintf(printBuf, "Task1 running on core: %d, Tick: %d\n", xPortGetCoreID(), xTaskGetTickCount());
    //__vSafePrint(printBuf);

    // digitalWrite(LED1, HIGH);
    // vTaskDelay(1400);
    // digitalWrite(LED1, LOW);
    vTaskDelay(1400);
  }
}

// Task2code: blinks an LED every 700 ms
void Task2code(void *pvParameters)
{
  char printBuf[32];
  for (;;)
  {
    // sprintf(printBuf, "Task2 running on core: %d, Tick: %d\n", xPortGetCoreID(), xTaskGetTickCount());
    //__vSafePrint(printBuf);

    // digitalWrite(LED2, HIGH);
    // vTaskDelay(600);
    // digitalWrite(LED2, LOW);
    vTaskDelay(600);
  }
}

void setup()
{
  Serial.begin(115200);
  Wire.begin(SDA_GPIO, SCL_GPIO); // SDA, SCL

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  displaySSD1306Setup();

  bleSetup();
  delay(1000);

  __vSafePrintInit();

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
  delay(500);
}

void loop()
{
  /*
  if(deviceConnected) {
    Serial.print("Notifying value: ");
    Serial.print(value);
    //Serial.print(" to ");
    //Serial.print(connectedClients);
    //Serial.println(" client(s)");
    pTxCharacteristic->setValue((uint8_t *)&value, 4);
    pTxCharacteristic->notify();
    value++;
  } else {
    pServer->startAdvertising();  // restart advertising
    Serial.println("No clients connected, restarting advertising");
  }
  */

  Serial.println("in loop...");
  //__vSafePrint("in loop...\n");
  if (deviceConnected)
  {
    // Serial.println("Device is connected");
    //__vSafePrint("Device is connected\n");
    displaySSD1306PrintNew(F("Device Connected"));
  }
  else
  {
    displaySSD1306Clear();
  }
  bleNotifyValue();
  bleRestartAdvertising();

  vTaskDelay(1000);
}