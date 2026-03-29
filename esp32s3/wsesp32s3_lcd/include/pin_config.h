#ifndef __PIN_CONFIG_H_
#define __PIN_CONFIG_H_

//ESP32-S3-LCD-3.16 pin configuration
//i2c
#define WSL_I2C_SDA_GPIO  15
#define WSL_I2C_SCL_GPIO  7
//i2c device QMI8658
#define WSL_QMI_DEV_ADDR  0x6B          //TODO or 0x6A
#define WSL_QMI_INT_GPIO  0             //TODO
#define WSL_QMI_SDA_GPIO  WSL_I2C_SDA_GPIO
#define WSL_QMI_SCL_GPIO  WSL_I2C_SCL_GPIO

#define Backlight_Testing
#define EXAMPLE_Rotate_90

/*
reference: 
    - https://community.home-assistant.io/t/waveshare-esp32-s3-lcd-3-16-sample-config/945239
    - https://files.waveshare.com/wiki/ESP32-S3-LCD-3.16/ESP32-S3-LCD-3.16-Schematic.pdf
    - https://www.haraldkreuzer.net/en/news/getting-started-sunton-esp32-s3-7-inch-display-lovyangfx-and-lvgl
*/

//uart
#define WSL_UART_TX_GPIO  43
#define WSL_UART_RX_GPIO  44

//lcd configuration
#define WSL_LCD_H_RES       320
#define WSL_LCD_V_RES       820

#define WSL_LCD_IO_SPI_CS      0   //  reference: ESP32-S3-LCD-3.16-Schematic.pdf
#define WSL_LCD_IO_SPI_SCK     2    //     CLK 2
#define WSL_LCD_IO_SPI_SDO     1    //     MOSI 1

#define WSL_LCD_IO_RGB_HSYNC   38
#define WSL_LCD_IO_RGB_VSYNC   39
#define WSL_LCD_IO_RGB_DE      40
#define WSL_LCD_IO_RGB_PCLK    41

#define WSL_LCD_IO_RGB_DISP    //37       //?
#define WSL_LCD_IO_RGB_RESET   16

#define WSL_LCD_IO_RGB_R0      17
#define WSL_LCD_IO_RGB_R1      46
#define WSL_LCD_IO_RGB_R2      3
#define WSL_LCD_IO_RGB_R3      8
#define WSL_LCD_IO_RGB_R4      18

#define WSL_LCD_IO_RGB_G0      14
#define WSL_LCD_IO_RGB_G1      13
#define WSL_LCD_IO_RGB_G2      12
#define WSL_LCD_IO_RGB_G3      11
#define WSL_LCD_IO_RGB_G4      10
#define WSL_LCD_IO_RGB_G5      9

#define WSL_LCD_IO_RGB_B0      21
#define WSL_LCD_IO_RGB_B1      5
#define WSL_LCD_IO_RGB_B2      45
#define WSL_LCD_IO_RGB_B3      48
#define WSL_LCD_IO_RGB_B4      47

#define WSL_LCD_BACKLIGHT_GPIO  6  
//#define EXAMPLE_PIN_NUM_BK_LIGHT   6

#define WSL_LCD_BIT_PER_PIXEL 16

#define EXAMPLE_LVGL_TICK_PERIOD_MS    2
#define EXAMPLE_LVGL_TASK_MAX_DELAY_MS 500
#define EXAMPLE_LVGL_TASK_MIN_DELAY_MS 1
#define EXAMPLE_LVGL_TASK_STACK_SIZE   (8 * 1024)
#define EXAMPLE_LVGL_TASK_PRIORITY     5

#endif //__PIN_CONFIG_H_