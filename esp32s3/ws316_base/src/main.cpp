#include <Arduino.h>
#include <vector>
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>
#include <lgfx/v1/platforms/esp32/Light_PWM.hpp>

#include "pin_config.h"

class LGFX : public lgfx::LGFX_Device
{
public:
  lgfx::Bus_RGB _bus_instance;
  lgfx::Panel_RGB _panel_instance;
  lgfx::Light_PWM _light_instance;
  // lgfx::Touch_GT911 _touch_instance;

  lgfx::Panel_ST7701 _panel_instance;

  // TODO reset pin??
  LGFX(void)
  {
    {
      auto cfg = _panel_instance.config();

      cfg.pin_cs = LCD_IO_SPI_CS;
      cfg.pin_rst = LCD_IO_RGB_RESET;

      cfg.memory_width = LCD_H_RES;
      cfg.memory_height = LCD_V_RES;
      cfg.panel_width = LCD_H_RES;
      cfg.panel_height = LCD_V_RES;

      cfg.offset_x = 0;
      cfg.offset_y = 0;

      _panel_instance.config(cfg);
    }

    { // Use PSRAM for frame buffer if available XXX
      auto cfg = _panel_instance.config_detail();
      cfg.pin_cs = LCD_IO_SPI_CS;
      cfg.pin_sclk = LCD_IO_SPI_SCK;
      cfg.pin_mosi = LCD_IO_SPI_SDO;
      cfg.use_psram = 1; // 0=SRAM only / 1=both / 2=PSRAM only
      _panel_instance.config_detail(cfg);
    }

    {
      auto cfg = _bus_instance.config();
      cfg.panel = &_panel_instance;

      // blue
      cfg.pin_d0 = LCD_IO_RGB_B0; // B0
      cfg.pin_d1 = LCD_IO_RGB_B1; // B1
      cfg.pin_d2 = LCD_IO_RGB_B2; // B2
      cfg.pin_d3 = LCD_IO_RGB_B3; // B3
      cfg.pin_d4 = LCD_IO_RGB_B4; // B4

      // green
      cfg.pin_d5 = LCD_IO_RGB_G0;  // G0
      cfg.pin_d6 = LCD_IO_RGB_G1;  // G1
      cfg.pin_d7 = LCD_IO_RGB_G2;  // G2
      cfg.pin_d8 = LCD_IO_RGB_G3;  // G3
      cfg.pin_d9 = LCD_IO_RGB_G4;  // G4
      cfg.pin_d10 = LCD_IO_RGB_G5; // G5

      // red
      cfg.pin_d11 = LCD_IO_RGB_R0; // R0
      cfg.pin_d12 = LCD_IO_RGB_R1; // R1
      cfg.pin_d13 = LCD_IO_RGB_R2; // R2
      cfg.pin_d14 = LCD_IO_RGB_R3; // R3
      cfg.pin_d15 = LCD_IO_RGB_R4; // R4

      cfg.pin_henable = LCD_IO_RGB_DE; //?
      cfg.pin_vsync = LCD_IO_RGB_VSYNC;
      cfg.pin_hsync = LCD_IO_RGB_HSYNC;
      cfg.pin_pclk = LCD_IO_RGB_PCLK;
      cfg.freq_write = 12000000; // 18MHz? kept to 12MHz for stability during CPU load

      /*
      cfg.hsync_polarity = 0;
      cfg.hsync_front_porch = 40;
      cfg.hsync_pulse_width = 48;
      cfg.hsync_back_porch = 40;

      cfg.vsync_polarity = 0;
      cfg.vsync_front_porch = 1;
      cfg.vsync_pulse_width = 31;
      cfg.vsync_back_porch = 13;
      */

      // cfg.pclk_active_neg = 1;  //default
      // cfg.de_idle_high = 0;     //default
      // cfg.pclk_idle_high = 0;   //default

      cfg.hsync_polarity = 0;
      cfg.hsync_front_porch = 30;
      cfg.hsync_pulse_width = 6;
      cfg.hsync_back_porch = 30;

      cfg.vsync_polarity = 0;
      cfg.vsync_front_porch = 20;
      cfg.vsync_pulse_width = 40;
      cfg.vsync_back_porch = 20;
      cfg.pclk_idle_high = 1; // from https://community.home-assistant.io/t/waveshare-esp32-s3-lcd-3-16-sample-config/945239

      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    {
      auto cfg = _light_instance.config();
      cfg.pin_bl = LCD_BACKLIGHT_GPIO;
      _light_instance.config(cfg);
      _panel_instance.light(&_light_instance);
    }

    /*
    {
        auto cfg = _touch_instance.config();
        cfg.x_min      = 0;
        cfg.x_max      = 799;
        cfg.y_min      = 0;
        cfg.y_max      = 479;
        cfg.pin_int    = -1;
        cfg.pin_rst    = -1;
        cfg.bus_shared = true;
        cfg.offset_rotation = 0;
        cfg.i2c_port   = I2C_NUM_1;
        cfg.pin_sda    = GPIO_NUM_19;
        cfg.pin_scl    = GPIO_NUM_20;
        cfg.freq       = 400000;
        cfg.i2c_addr   = 0x14;
        _touch_instance.config(cfg);
        _panel_instance.setTouch(&_touch_instance);
    }
    */
    setPanel(&_panel_instance);
  }
};

static LGFX lcd;

// buffer size (SCREEN_HOR_RES * SCREEN_VER_RES / 10 * (LV_COLOR_DEPTH / 8)) ?
// static lv_color_t* draw_buf1 = (lv_color_t*)heap_caps_aligned_alloc(32, (LCD_WIDTH * LCD_HEIGHT * 2) / 6, MALLOC_CAP_DMA);
// static lv_color_t* draw_buf2 = (lv_color_t*)heap_caps_aligned_alloc(32, (LCD_WIDTH * LCD_HEIGHT * 2) / 6, MALLOC_CAP_DMA);

void setup()
{
  Serial.begin(115200);
  Serial.println("Setup started....");
  lcd.init();

  lcd.setRotation(1);
  lcd.setBrightness(128);

  lcd.setColorDepth(16);

  lcd.drawPixel(0, 0, 0xFFFF);
  lcd.drawFastVLine(4, 0, 100, lcd.color565(0, 255, 0));

  lcd.setColor(0x0000FFU);
  lcd.drawCircle(40, 80, 20);
  lcd.drawEllipse(80, 40, 10, 20);
  lcd.drawArc(80, 80, 20, 10, 0, 90);
  lcd.drawTriangle(60, 80, 80, 80, 80, 60);
  delay(2000);

  lcd.fillScreen(0xFFFFFFu);
  lcd.setColor(0x00FF00u);
  lcd.fillScreen();

  lcd.clear(0xFFFFFFu);
  lcd.setBaseColor(0x000000u);
  lcd.clear();

  Serial.println("Setup done....");
  delay(2000);
}

void loop()
{
}
