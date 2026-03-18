#include "peripheral.h"

#include <Wire.h>
#include <SensorPCF8563.hpp>
#include <time.h>

//#include "encoder.h"

static SensorPCF8563 rtc;
static uint8_t buzzerPin = -1;
//RotaryEncoder rotaryEncoder = RotaryEncoder(M5DIAL_ROTARY_APIN, M5DIAL_ROTARY_BPIN, read_encoder_ISR);

static void __peripheral_buzzer_init(uint8_t pinNum) {
    buzzerPin = pinNum;
    pinMode(pinNum, OUTPUT);
}

void peripheral_buzzer_tone(uint16_t duration) {
    if (buzzerPin < 0) return;
    digitalWrite(buzzerPin, HIGH);
    delay(duration);
    digitalWrite(buzzerPin, LOW);
}

static void __peripheral_rtc_init(void) {
    //RTC PCF8563, old: rtc.begin(Wire, M5DIAL_SDA_PIN, M5DIAL_SCL_PIN)
    if (!rtc.begin(Wire)) {
        Serial.println("Failed to find PCF85063");
        //rtcEnabled = false;
        return;
        //while(1) {
        //    delay(1000);
        //}
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

#if 0
void IRAM_ATTR read_encoder_ISR(void) {
	rotaryEncoder.readEncoderISR();
}

RotaryEncoder *peripheral_get_rotary_encoder(void) {
    return &rotaryEncoder;
}
#endif

void peripheral_init(void) {
    Wire.begin(M5DIAL_SDA_PIN, M5DIAL_SCL_PIN, 400000);     //freq: 400000, as per M5 lib, TODO try leaving it 0
    //TODO: 

    __peripheral_buzzer_init(M5DIAL_BUZZER_PIN);
    __peripheral_rtc_init();

    //Wire.setPins(M5DIAL_SDA_PIN, M5DIAL_SCL_PIN);   //Warn: `setPins(): bus already initialized. change pins only when not.`
    //Wire.setClock(400000);
}