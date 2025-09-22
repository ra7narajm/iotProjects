#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "orbitron_font_12.h"
#include "pinConfig.h"
#include "bleServiceConfig.h"

TaskHandle_t Task1;
TaskHandle_t Task2;
TaskHandle_t bleCommTask;

//TaskStatus_t Task1Details;
//TaskStatus_t Task2Details;

// LED pins
const int LED1 = 39;
const int LED2 = 40;
const int LED3 = 41;
const int LED4 = 42;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

SemaphoreHandle_t __print_mutex;

void __vSafePrint(char *out) {
	xSemaphoreTake(__print_mutex, portMAX_DELAY);
	puts(out);
	xSemaphoreGive(__print_mutex);
}

//Task1code: blinks an LED every 1500 ms
void Task1code( void * pvParameters ){
   char printBuf[48];
   for(;;){
      sprintf(printBuf, "Task1 running on core: %d, Tick: %d\n", xPortGetCoreID(), xTaskGetTickCount());
      __vSafePrint(printBuf);

      digitalWrite(LED1, HIGH);
      vTaskDelay(1400);
      digitalWrite(LED1, LOW);
      vTaskDelay(1400);
   } 
}

//Task2code: blinks an LED every 700 ms
void Task2code( void * pvParameters ){
   char printBuf[32];
   for(;;){
      sprintf(printBuf, "Task2 running on core: %d, Tick: %d\n", xPortGetCoreID(), xTaskGetTickCount());
      __vSafePrint(printBuf);

      digitalWrite(LED2, HIGH);
      vTaskDelay(600);
      digitalWrite(LED2, LOW);
      vTaskDelay(600);
   }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_GPIO, SCL_GPIO); // SDA, SCL

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  __print_mutex = xSemaphoreCreateMutex();

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(10, 10, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);
  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...
  Serial.println("Hello World");

  display.setTextSize(1); // Normal 1:1 pixel scale
  display.setRotation(2);
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setFont(&Orbitron_Medium_12);
  display.setCursor(0,0); // Start at top-left corner
  display.println(F("Hello, world!"));

  display.display();
  Serial.println("Did it work?");
  delay(2000);
  display.clearDisplay();
  delay(2000);

  bleSetup();

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    2048,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    2048,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
    delay(500);

}

void loop() {
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

  vTaskDelay(2000);
}