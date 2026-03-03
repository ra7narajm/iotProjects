#include "display.h"

M5LGFX *lcd;

void m5dial_display_init(void) {
    lcd = new M5LGFX();

    lcd->init();
    lcd->initDMA();
    lcd->setRotation(0);
    lcd->setBrightness(200);
    lcd->setColorDepth(LV_COLOR_DEPTH); // 16
    lcd->fillScreen(TFT_WHITE);

    Serial.println("[m5dial_display_init] finished");
}

void m5dial_disp_flush(lv_display_t *display, const lv_area_t *area, uint8_t *px_map) {
    uint32_t w = lv_area_get_width(area);
    uint32_t h = lv_area_get_height(area);

    // Swap color bytes (RGB565) if necessary for your display controller
    // LVGL native is often swapped compared to LGFX expectations
    lv_draw_sw_rgb565_swap(px_map, w * h);
    // Push the pixel data to the display
    lcd->startWrite();
    lcd->setAddrWindow(area->x1, area->y1, w, h);
    lcd->pushImageDMA<uint16_t>(area->x1, area->y1, w, h, (uint16_t *)px_map);
    // lcd.pushImage(area->x1, area->y1, w, h, (uint16_t *)px_map);
    lcd->endWrite();

    // IMPORTANT: Inform LVGL that flushing is complete
    lv_disp_flush_ready(display);
}