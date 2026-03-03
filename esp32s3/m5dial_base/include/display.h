#ifndef __M5DIAL_DISPLAY_DRV_H_
#define __M5DIAL_DISPLAY_DRV_H_
#include <Arduino.h>
#include <LovyanGFX.hpp>

//LVGL display flush function for now included here
#include <lvgl.h>

#include "common.h"
#include "pin_config.h"

class M5LGFX : public lgfx::LGFX_Device {

  lgfx::Panel_GC9A01  _panel_instance;
  lgfx::Bus_SPI _bus_instance;
  lgfx::Light_PWM _light_instance;

  public:

  M5LGFX(void) {
    {              
      auto cfg = _bus_instance.config();

      cfg.spi_host = SPI3_HOST;
      cfg.spi_mode = 0;                  
      cfg.freq_write = 80000000;
      cfg.freq_read = 16000000;
      cfg.spi_3wire = true;          
      cfg.use_lock = true;          
      cfg.dma_channel = SPI_DMA_CH_AUTO;  

      cfg.pin_sclk = M5DIAL_LCD_SCK_PIN; 
      cfg.pin_mosi = M5DIAL_LCD_MOSI_PIN;
      cfg.pin_dc = M5DIAL_LCD_RS_PIN;

      _bus_instance.config(cfg);      
      _panel_instance.setBus(&_bus_instance); 
    }
    lgfx::gpio_hi(M5DIAL_LCD_RST_PIN);
    lgfx::pinMode(M5DIAL_LCD_RST_PIN, lgfx::pin_mode_t::output);
    lgfx::delay(1);
    {                             
      auto cfg = _panel_instance.config(); 

      cfg.pin_cs = M5DIAL_LCD_CS_PIN;  
      cfg.pin_rst = M5DIAL_LCD_RST_PIN; 
      cfg.pin_busy = -1;

      cfg.panel_width = M5DIAL_LCD_WIDTH;   
      cfg.panel_height = M5DIAL_LCD_HEIGHT;
      cfg.offset_x = 0;       
      cfg.offset_y = 0;  
      cfg.offset_rotation = 0;   
      cfg.dummy_read_pixel = 8;  
      cfg.dummy_read_bits = 1; 
      cfg.readable = false;    
      cfg.invert = true;      
      cfg.rgb_order = false;  

      _panel_instance.config(cfg);
    }
    {
      auto cfg = _light_instance.config();  //https://github.com/m5stack/M5GFX/blob/e1bf27b94593d5e4d09fb352cdacacadab813b7e/src/M5GFX.cpp#L585

      cfg.pin_bl = M5DIAL_LCD_BL_PIN;
      cfg.freq   = M5DIAL_BL_PWM_FRQ;
      cfg.pwm_channel = M5DIAL_BL_PWM_CH;
      cfg.offset = 0;     //reference https://github.com/m5stack/M5GFX/blob/e1bf27b94593d5e4d09fb352cdacacadab813b7e/src/M5GFX.cpp#L1353
      cfg.invert = false;    //bool

      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);
    }
    setPanel(&_panel_instance);
  }
};

// TODO singleton
extern M5LGFX *lcd;

#ifdef __cplusplus
extern "C" {
#endif

void m5dial_display_init(void);
void m5dial_disp_flush(lv_display_t *display, const lv_area_t *area, uint8_t *px_map);

#ifdef __cplusplus
}
#endif

#endif //__M5DIAL_DISPLAY_DRV_H_