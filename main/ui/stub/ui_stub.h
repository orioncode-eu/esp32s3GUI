/**
 * @file ui_stub.h
 * @brief esp32s3GUI – stub wyświetlacza (logi zamiast LVGL).
 */

#ifndef ESP32S3GUI_UI_STUB_H
#define ESP32S3GUI_UI_STUB_H

#include <stdint.h>

void ui_stub_init(void);
void ui_stub_show_page(uint8_t page);

#endif /* ESP32S3GUI_UI_STUB_H */
