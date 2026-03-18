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
extern void ui_refresh_task(void *pvParams);

// for some reason unable to get EEZ studio ui working
// private --------------
enum screenEnum {
    screenMain = 0,
    _screenMax = 1,
};

typedef struct _objects0_t {
    lv_obj_t *main;         //screen
    lv_obj_t *clockLabel;
    lv_obj_t *dateLabel;
} objects0_t;

extern objects0_t object0;  //1st screen objects

/*
typedef struct _objects1_t {
    lv_obj_t *main;         //screen
} objects1_t;

extern objects1_t object1;  //2nd screen objects
*/

//list of screen, usage screen[screenID]
extern lv_obj_t *screens[];
// private --------------

void ui_init(void);
void ui_tick(void);

#ifdef __cplusplus
}
#endif

#endif //__UI_GUI_H_