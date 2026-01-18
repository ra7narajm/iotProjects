#ifndef __DISPLAY_SSD1306_H_
#define __DISPLAY_SSD1306_H_

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

void displaySSD1306Setup(void);
void displaySSD1306PrintNew(const String& message);
void displaySSD1306PrintAppend(const String& message);
void displaySSD1306Clear(void);

#endif //__DISPLAY_SSD1306_H_