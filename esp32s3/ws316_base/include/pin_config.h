#ifndef __PIN_CONFIG_H__
#define __PIN_CONFIG_H__
//pin configuration
//i2c
#define I2C_SDA_GPIO  15
#define I2C_SCL_GPIO  7
//i2c device QMI8658
#define QMI_DEV_ADDR  0x6B          //TODO or 0x6A
#define QMI_INT_GPIO  0             //TODO
#define QMI_SDA_GPIO  I2C_SDA_GPIO
#define QMI_SCL_GPIO  I2C_SCL_GPIO

/*
reference: 
    - https://community.home-assistant.io/t/waveshare-esp32-s3-lcd-3-16-sample-config/945239
    - https://files.waveshare.com/wiki/ESP32-S3-LCD-3.16/ESP32-S3-LCD-3.16-Schematic.pdf
    - https://www.haraldkreuzer.net/en/news/getting-started-sunton-esp32-s3-7-inch-display-lovyangfx-and-lvgl
*/

//uart
#define UART_TX_GPIO  43
#define UART_RX_GPIO  44

//lcd configuration
#define LCD_H_RES       320
#define LCD_V_RES       820

#define LCD_IO_SPI_CS      42   //  reference: ESP32-S3-LCD-3.16-Schematic.pdf
#define LCD_IO_SPI_SCK     2    //     CLK 2
#define LCD_IO_SPI_SDO     1    //     MOSI 1

#define LCD_IO_RGB_HSYNC   38
#define LCD_IO_RGB_VSYNC   39
#define LCD_IO_RGB_DE      40
#define LCD_IO_RGB_PCLK    41

//#define LCD_IO_RGB_DISP    37       //?
#define LCD_IO_RGB_RESET   16

#define LCD_IO_RGB_R0      17
#define LCD_IO_RGB_R1      46
#define LCD_IO_RGB_R2      3
#define LCD_IO_RGB_R3      8
#define LCD_IO_RGB_R4      18

#define LCD_IO_RGB_G0      14
#define LCD_IO_RGB_G1      13
#define LCD_IO_RGB_G2      12
#define LCD_IO_RGB_G3      11
#define LCD_IO_RGB_G4      10
#define LCD_IO_RGB_G5      9

#define LCD_IO_RGB_B0      21
#define LCD_IO_RGB_B1      5
#define LCD_IO_RGB_B2      45
#define LCD_IO_RGB_B3      48
#define LCD_IO_RGB_B4      47

#define LCD_BACKLIGHT_GPIO  6

#endif // __PIN_CONFIG_H__