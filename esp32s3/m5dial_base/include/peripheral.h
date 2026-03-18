#ifndef __M5DIAL_PERIPHERAL_H_
#define __M5DIAL_PERIPHERAL_H_
#include <Arduino.h>

#include "common.h"
#include "pin_config.h"
#include <time.h>
//#include "encoder.h"

#ifdef __cplusplus
extern "C" {
#endif

//extern RotaryEncoder rotaryEncoder;

void peripheral_init(void);

//XXX this buzzer adds delay DONT USE
void peripheral_buzzer_tone(uint16_t duration);

//RTC
void peripheral_rtc_get_date_time(struct tm *timeInfo);
void peripheral_rtc_set_date_time(struct tm *timeInfo);
// alarm functions
// timer function

//RotaryEncoder *peripheral_get_rotary_encoder(void);
//void IRAM_ATTR read_encoder_ISR(void);

//RFID
//get
//set??
#ifdef __cplusplus
}
#endif
#endif //M5DIAL_PERIPHERAL_H_