#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "orbitron_font_12.h"
#include "displaySSD1306.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void displaySSD1306Setup(void)
{
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ; // Don't proceed, loop forever
    }
    Serial.println(F("SSD1306 initialized"));

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
    Serial.println(F("Hello World"));

    display.setTextSize(1); // Normal 1:1 pixel scale
    display.setRotation(2);
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.setFont(&Orbitron_Medium_12);
    display.setCursor(0, 0); // Start at top-left corner
    display.println(F("Hello, world!"));

    display.display();
    Serial.println(F("Did it work?"));
    delay(2000);
    display.clearDisplay();
}

void displaySSD1306PrintNew(const String &message)
{
    display.clearDisplay();
    display.setCursor(0, 0); // Start at top-left corner
    display.println(message);
    display.display();
}

void displaySSD1306PrintAppend(const String &message)
{
    display.clearDisplay();
    display.println(message);
    display.display();
}

void displaySSD1306Clear(void)
{
    display.clearDisplay();
    display.display();
}