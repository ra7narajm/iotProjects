#ifndef __UI_GUI_H_
#define __UI_GUI_H_

//#define LV_FONT_MONTSERRAT_18 1
//#define LV_FONT_MONTSERRAT_48 1

#include <Arduino.h>
#include <lvgl.h>

#include "common.h"
#include "pin_config.h"

#define LV_VER_RES_MAX  M5DIAL_LCD_WIDTH
#define LV_HOR_RES_MAX  M5DIAL_LCD_HEIGHT

#ifdef __cplusplus
extern "C" {
#endif

extern TaskHandle_t Ui_Task;

// for some reason unable to get EEZ studio ui working

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *clockLabel;
    lv_obj_t *dateLabel;
} objects_t;

extern objects_t objects;

void ui_init(void);
void ui_tick(void);

#ifdef __cplusplus
}
#endif

#endif //__UI_GUI_H_