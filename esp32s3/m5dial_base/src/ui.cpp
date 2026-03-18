#include "display.h"
#include "ui.h"
#include "peripheral.h"

TaskHandle_t Ui_Task;

static lv_display_t *lvDisplay;
//static lv_indev_t *encoderDev;

objects0_t object0;     //1st screen object
screenEnum currentScreenId = screenMain;    //main
lv_obj_t *screens[_screenMax];

LV_FONT_DECLARE(orbitron_24_4bpp);
LV_FONT_DECLARE(orbitron_14_4bpp);

static void __ui_create_main_screen(void) {
    //main screen
    object0.main = lv_obj_create(NULL);     // NULL creates new screen
    screens[screenMain] = object0.main;
    lv_obj_set_pos(object0.main, 0, 0);
    //lv_obj_align(object0.main, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(object0.main, 240, 240);
    lv_obj_set_style_bg_color(object0.main, lv_color_hex(0xfff8f8f8), LV_PART_MAIN | LV_STATE_DEFAULT);
    //lv_obj_set_style_bg_color(object0.main, lv_color_white(), LV_PART_MAIN);

    //clock
    {
        object0.clockLabel = lv_label_create(object0.main);
        lv_obj_set_size(object0.clockLabel, 180, 70);
        lv_obj_set_pos(object0.clockLabel, 25, 55);
        //lv_obj_align(object0.clockLabel, LV_ALIGN_CENTER, 30, 53);
        lv_obj_set_style_bg_color(object0.clockLabel, lv_color_hex(0xfff8f8f8), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(object0.clockLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(object0.clockLabel, &orbitron_24_4bpp, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(object0.clockLabel, lv_color_black(), LV_PART_MAIN);
        lv_label_set_text(object0.clockLabel, "HH:MM:SS");
    }
    //date
    {
        object0.dateLabel = lv_label_create(object0.main);
        lv_obj_set_size(object0.dateLabel, 180, 35);
        lv_obj_set_pos(object0.dateLabel, 25, 135);
        //lv_obj_align(object0.dateLabel, LV_ALIGN_CENTER, 30, 53);
        lv_obj_set_style_bg_color(object0.dateLabel, lv_color_hex(0xfff8f8f8), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(object0.dateLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(object0.dateLabel, &orbitron_14_4bpp, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(object0.dateLabel, lv_color_black(), LV_PART_MAIN);
        lv_label_set_text(object0.dateLabel, "Day, Dt Mon, YYYY");
    }
}

static constexpr const char* const wday[7] = { "Sun", "Mon", "Tue", "Wed", "Thr", "Fri", "Sat" };
static constexpr const char* const month[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
void update_time_task(lv_timer_t * timer) {
    struct tm dtInfo;
    //char labelBuf[24];
    peripheral_rtc_get_date_time(&dtInfo);
    //Serial.println(&dtInfo, "%A, %B %d %Y %H:%M:%S");
    {
        //sprintf(labelBuf, "%02d:%02d:%02d", dtInfo.tm_hour, dtInfo.tm_min, dtInfo.tm_sec);
        lv_label_set_text_fmt(object0.clockLabel, "%02d:%02d:%02d", dtInfo.tm_hour, dtInfo.tm_min, dtInfo.tm_sec);
        //Serial.printf("updating clockLabel %s\n", lv_label_get_text(object0.clockLabel));
    }
    {
        //TODO need not update every tick
        //sprintf(labelBuf, "%s, %02d %s, %04d", wday[dtInfo.tm_wday], dtInfo.tm_mday, month[dtInfo.tm_mon], dtInfo.tm_year + 1900);
        lv_label_set_text_fmt(object0.dateLabel, "%s, %02d %s, %04d", wday[dtInfo.tm_wday], dtInfo.tm_mday, month[dtInfo.tm_mon], dtInfo.tm_year + 1900);
        //Serial.printf("updating dateLabel %s\n", lv_label_get_text(object0.dateLabel));
    }
    //periheral_buzzer_tone(1000);  //adding too much delay
}

void __ui_refresh_main(void) {
    //refresh main screen
}

// --------------------------------------------------------------------------
static void __ui_create_next0_screen(void) {
}

void __ui_refresh_next0(void) {
    //refresh main screen
}

typedef void (*__ui_refresh_screen_func_t)(void);
__ui_refresh_screen_func_t __refresh_screen_funcs[] = {
    __ui_refresh_main,
    //__ui_refresh_next0,
};

static void __ui_refresh_screens(void) {
    __refresh_screen_funcs[currentScreenId]();
}

static void __ui_create_screens(void) {
    __ui_create_main_screen();
}

static void __ui_load_screen(screenEnum sId) {
    lv_scr_load_anim(screens[sId], LV_SCR_LOAD_ANIM_FADE_IN, 200, 0, false);
}

// buffer size (SCREEN_HOR_RES * SCREEN_VER_RES / 10 * (LV_COLOR_DEPTH / 8)) ?
static lv_color_t *draw_buf1 = (lv_color_t *)heap_caps_aligned_alloc(32, (M5DIAL_LCD_WIDTH * M5DIAL_LCD_HEIGHT * 2) / 6, MALLOC_CAP_DMA);
static lv_color_t *draw_buf2 = (lv_color_t *)heap_caps_aligned_alloc(32, (M5DIAL_LCD_WIDTH * M5DIAL_LCD_HEIGHT * 2) / 6, MALLOC_CAP_DMA);

void ui_init(void) {  
    lv_init();

    lvDisplay = lv_display_create(M5DIAL_LCD_WIDTH, M5DIAL_LCD_HEIGHT); // TODO null check??
    lv_display_set_color_format(lvDisplay, LV_COLOR_FORMAT_RGB565);
    lv_display_set_flush_cb(lvDisplay, m5dial_disp_flush);
    lv_display_set_buffers(lvDisplay, draw_buf1, draw_buf2, ((M5DIAL_LCD_WIDTH * M5DIAL_LCD_HEIGHT * 2) / 6), LV_DISPLAY_RENDER_MODE_PARTIAL);

    // register the encoder input device
    // encoderDev = lv_indev_create();
    // lv_indev_set_type(encoderDev, LV_INDEV_TYPE_ENCODER);
    // lv_indev_set_read_cb(encoderDev, encoder_read_cb);
    // create group and add objects to it

    __ui_create_screens();

    lv_timer_create(update_time_task, 1000, NULL);

    __ui_load_screen(screenMain);
}

void ui_tick(void) {
    __ui_refresh_screens();
}

//instead of running in loop function run all lvgl in task pinned one core
void ui_refresh_task(void *pvParams) {
    for (;;) {
        lv_timer_handler();   //older version lv_task_handler() ref: https://docs.lvgl.io/8/porting/timer-handler.html
        ui_tick();
        vTaskDelay(5);   //5ms recommended
        lv_tick_inc(5);
    }
}