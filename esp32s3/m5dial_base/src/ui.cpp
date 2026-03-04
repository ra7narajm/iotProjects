#include "display.h"
#include "ui.h"
#include "peripheral.h"

static lv_display_t *lvDisplay;
//static lv_indev_t *encoderDev;

objects_t objects;
TaskHandle_t Ui_task;

LV_FONT_DECLARE(orbitron_24_4bpp);
LV_FONT_DECLARE(orbitron_14_4bpp);

static void __ui_create_screens(void) {
    //main screen
    objects.main = lv_obj_create(lv_scr_act());
    lv_obj_set_pos(objects.main, 0, 0);
    //lv_obj_align(objects.main, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(objects.main, 240, 240);
    lv_obj_set_style_bg_color(objects.main, lv_color_hex(0xfff8f8f8), LV_PART_MAIN | LV_STATE_DEFAULT);
    //lv_obj_set_style_bg_color(objects.main, lv_color_white(), LV_PART_MAIN);

    //clock
    {
        objects.clockLabel = lv_label_create(objects.main);
        lv_obj_set_size(objects.clockLabel, 180, 70);
        lv_obj_set_pos(objects.clockLabel, 20, 55);
        //lv_obj_align(objects.clockLabel, LV_ALIGN_CENTER, 30, 53);
        lv_obj_set_style_bg_color(objects.clockLabel, lv_color_hex(0xfff8f8f8), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(objects.clockLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(objects.clockLabel, &orbitron_24_4bpp, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(objects.clockLabel, lv_color_black(), LV_PART_MAIN);
        lv_label_set_text(objects.clockLabel, "HH:MM:SS");
    }
    //date
    {
        objects.dateLabel = lv_label_create(objects.main);
        lv_obj_set_size(objects.dateLabel, 180, 35);
        lv_obj_set_pos(objects.dateLabel, 20, 135);
        //lv_obj_align(objects.dateLabel, LV_ALIGN_CENTER, 30, 53);
        lv_obj_set_style_bg_color(objects.dateLabel, lv_color_hex(0xfff8f8f8), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(objects.dateLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(objects.dateLabel, &orbitron_14_4bpp, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(objects.dateLabel, lv_color_black(), LV_PART_MAIN);
        lv_label_set_text(objects.dateLabel, "Day, Dt Mon, YYYY");
    }
}

static constexpr const char* const wday[7] = { "Sun", "Mon", "Tue", "Wed", "Thr", "Fri", "Sat" };
static constexpr const char* const month[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
void update_time_task(lv_timer_t * timer) {
    struct tm dtInfo;
    char labelBuf[24];
    peripheral_rtc_get_date_time(&dtInfo);
    Serial.println(&dtInfo, "%A, %B %d %Y %H:%M:%S");
    {
        //sprintf(labelBuf, "%02d:%02d:%02d", dtInfo.tm_hour, dtInfo.tm_min, dtInfo.tm_sec);
        lv_label_set_text_fmt(objects.clockLabel, "%02d:%02d:%02d", dtInfo.tm_hour, dtInfo.tm_min, dtInfo.tm_sec);
        Serial.printf("updating clockLabel %s\n", lv_label_get_text(objects.clockLabel));
    }
    {
        //TODO need not update every tick
        //sprintf(labelBuf, "%s, %02d %s, %04d", wday[dtInfo.tm_wday], dtInfo.tm_mday, month[dtInfo.tm_mon], dtInfo.tm_year + 1900);
        lv_label_set_text_fmt(objects.dateLabel, "%s, %02d %s, %04d", wday[dtInfo.tm_wday], dtInfo.tm_mday, month[dtInfo.tm_mon], dtInfo.tm_year + 1900);
        Serial.printf("updating dateLabel %s\n", lv_label_get_text(objects.dateLabel));
    }
}

static void __ui_refresh_screens(void) {
    //update_time_task(NULL);
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
}

void ui_tick(void) {
    __ui_refresh_screens();
}

/*
void ui_refresh_task(void *pvParams) {
    for (;;) {
        ui_tick();
        delay(1000);
    }
}
*/