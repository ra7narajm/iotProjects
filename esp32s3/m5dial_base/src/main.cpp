#include <Arduino.h>
#include <string.h>
#include <Preferences.h>
#include <LovyanGFX.hpp>
#include <lvgl.h>
#include <WiFi.h>

#include "time.h"
#include "esp_sntp.h"

#include "common.h"
#include "pin_config.h"
#include "peripheral.h"
#include "display.h"
#include "ui.h"

/*
static void encoder_read_cb(lv_indev_t *indev, lv_indev_data_t *data) {
    // Replace with your actual encoder hardware reading logic
    // data->enc_diff = num_clicks; // + for CW, - for CCW
    // data->state = is_pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
}
*/

void buzzerTone(uint16_t pinNum, uint16_t duration) {
  digitalWrite(pinNum, HIGH);
  delay(duration);
  digitalWrite(pinNum, LOW);
}

//wifi - save/read from preferences
const char* ssid = "darks7ar";
const char* password = "S0nuM0nuMr1dul";
static void __setup_wifi(void) {
  Serial.printf("Connecting to SSID: %s", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }
  Serial.printf("Connected to SSID: %s", ssid);
}

#define NTP_SERVER1  "0.pool.ntp.org"
#define NTP_SERVER2  "1.pool.ntp.org"
#define NTP_SERVER3  "time.nist.gov"
#define NTP_TIMEZONE "IST-5:30"   //Asia/kolkata https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv

// Callback function (called when time adjusts via NTP)
static void __timeavailable(struct timeval* t) {
  struct tm dtInfo;
  Serial.println("Got time adjustment from NTP!");
  //update RTC 8563
  if (!getLocalTime(&dtInfo)) {
    Serial.println("No time available...");
  }
  peripheral_rtc_set_date_time(&dtInfo);
  
  Serial.println(&dtInfo, "%A, %B %d %Y %H:%M:%S");
}

static void __setup_sntp(void) {
  // set notification call-back function
  sntp_set_time_sync_notification_cb(__timeavailable);
  sntp_servermode_dhcp(1);  // (optional)
  configTzTime(NTP_TIMEZONE, NTP_SERVER1, NTP_SERVER2, NTP_SERVER3);
  delay(1000);
  while (sntp_get_sync_status() != SNTP_SYNC_STATUS_COMPLETED) {
    Serial.println(".");
    delay(500);
  }
  Serial.printf("Connected to SNTP");
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting ....");

  peripheral_init();  //TODO init all peripherals

  __setup_wifi();
  __setup_sntp();

  m5dial_display_init();
  delay(1000);

  Serial.println("UI init next ...");
  ui_init();
  Serial.println("UI init complete.....");
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  lv_timer_handler();   //older version lv_task_handler() ref: https://docs.lvgl.io/8/porting/timer-handler.html
  ui_tick();
  vTaskDelay(5);   //5ms recommended
  lv_tick_inc(5);
}