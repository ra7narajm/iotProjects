#include "peripheral.h"

#include <Wire.h>
#include <SensorPCF8563.hpp>
#include <time.h>

static SensorPCF8563 rtc;

static void __peripheral_rtc_init(void) {
    //RTC PCF8563
    if (!rtc.begin(Wire, M5DIAL_SDA_PIN, M5DIAL_SCL_PIN)) {
        Serial.println("Failed to find PCF85063");
        while(1) {
            delay(1000);
        }
    }
    //year, month, day, hour, minute, second
    rtc.setDateTime(2026, 3, 3, 10, 10, 10);    //default time
    if (!rtc.isClockIntegrityGuaranteed()) {
        Serial.println("[ERROR]:Clock integrity is not guaranteed; oscillator has stopped or has been interrupted");
    }
    Serial.println("__peripheral_rtc_init: RTC initialised");
}

void peripheral_rtc_get_date_time(struct tm *timeInfo) {
    if (!timeInfo) return;
    RTC_DateTime dtInfo = rtc.getDateTime();
    *timeInfo = dtInfo.toUnixTime();
}

void peripheral_rtc_set_date_time(struct tm *timeInfo) {
    if (!timeInfo) return;
    rtc.setDateTime(*timeInfo);
}

void peripheral_init(void) {
    Wire.begin(M5DIAL_SDA_PIN, M5DIAL_SCL_PIN, 400000);     //freq: 400000, as per M5 lib, TODO try leaving it 0

    __peripheral_rtc_init();
}

