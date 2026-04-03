#include <Arduino.h>
#include <string.h>
#include <Preferences.h>
#include <lvgl.h>

#include <lcd_bl_pwm_bsp.h>

#include "pin_config.h"
#include "lvgl_port.h"
//#include "lcd_bl_pwm_bsp.h"

void setup() {
  Serial.begin(115200);
  Serial.println("Setup started....");
  
  delay(2000);
  lvgl_port_init();
  lcd_bl_pwm_bsp_init(LCD_PWM_MODE_255);
}

void loop() {
  // put your main code here, to run repeatedly:
}