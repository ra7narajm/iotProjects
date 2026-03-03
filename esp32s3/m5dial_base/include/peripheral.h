#ifndef __M5DIAL_PERIPHERAL_H_
#define __M5DIAL_PERIPHERAL_H_
#include <Arduino.h>

#include "common.h"
#include "pin_config.h"
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

void peripheral_init(void);

//RTC
void peripheral_rtc_get_date_time(struct tm *timeInfo);
void peripheral_rtc_set_date_time(struct tm *timeInfo);
// alarm functions
// timer function

//RFID
//get
//set??
#ifdef __cplusplus
}
#endif
#endif //M5DIAL_PERIPHERAL_H_