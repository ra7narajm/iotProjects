#ifndef __PIN_CONFIG_H_
#define __PIN_CONFIG_H_

#define M5DIAL_LCD_HEIGHT 240
#define M5DIAL_LCD_WIDTH  240

//i2c
#define M5DIAL_SCL_PIN 12
#define M5DIAL_SDA_PIN 11

//rtc
#define M5DIAL_RTC_SCL_PIN M5DIAL_SCL_PIN
#define M5DIAL_RTC_SDA_PIN M5DIAL_SDA_PIN
#define M5DIAL_RTC_I2C_ADDR 0x51    //RTC8563

//rfid scl/sda as per i2c
#define M5DIAL_RFID_SCL_PIN M5DIAL_SCL_PIN
#define M5DIAL_RFID_SDA_PIN M5DIAL_SDA_PIN
#define M5DIAL_RFID_RST_PIN 8
#define M5DIAL_RFID_IRQ_PIN 10
#define M5DIAL_RFID_I2C_ADDR 0x28   //ws1850s

//buzzer
#define M5DIAL_BUZZER_PIN 3

//rotary encoder
#define M5DIAL_ROTARY_APIN 41
#define M5DIAL_ROTARY_BPIN 40
#define M5DIAL_ROTARY_VCC_PIN -1  //connected to vcc
#define M5DIAL_ROTARY_BUTTON_PIN 42 //https://github.com/m5stack/M5Unified/blob/bf322a062657aae49b2a93e2ca87351769f7b440/src/M5Unified.cpp#L1232

//GC9101
#define M5DIAL_LCD_RS_PIN   4       //register select or Data/Command
#define M5DIAL_LCD_MOSI_PIN 5       //SDA
#define M5DIAL_LCD_SCK_PIN  6
#define M5DIAL_LCD_CS_PIN   7
#define M5DIAL_LCD_RST_PIN  8
#define M5DIAL_LCD_BL_PIN   9

#define M5DIAL_BL_PWM_CH 7
#define M5DIAL_BL_PWM_FRQ 44100

//FT3267 touch sensor (i2c) #TODO
#define M5DIAL_TOUCH_SDA_PIN M5DIAL_SCL_PIN
#define M5DIAL_TOUCH_SCL_PIN M5DIAL_SDA_PIN
#define M5DIAL_TOUCH_INT_PIN 14
#define M5DIAL_TOUCH_I2C_ADDR 0x38

//power management ?? #TODO AXP2101 pmic (single cell lipo)
#define M5DIAL_POWER_HOLD_PIN 46

#define M5DIAL_BTN_A_PIN    0  //TODO

#endif //__PIN_CONFIG_H_