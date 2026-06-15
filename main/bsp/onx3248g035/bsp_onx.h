/**
 * @file bsp_onx.h
 * @brief esp32s3GUI – publiczny interfejs BSP dla ONX3248G035.
 *
 * Wywołać RAZ z ui_task (przez display_driver_init() w hal/). Po powrocie:
 *  - Magistrala I2C i SPI są zainicjalizowane.
 *  - Panel LCD ST7796U jest włączony (`disp_on=true`).
 *  - Podświetlenie (LEDC PWM) gra na ~100%.
 *  - Touch CST826 jest gotowy.
 *  - LVGL port jest zainicjalizowany, panel + touch są dodane jako
 *    display/indev. esp_lvgl_port tworzy własny task LVGL – ui_task NIE
 *    woła `lv_timer_handler()` ręcznie.
 */

#ifndef ESP32S3GUI_BSP_ONX_H
#define ESP32S3GUI_BSP_ONX_H

#include "esp_err.h"

esp_err_t bsp_onx_init(void);

#endif /* ESP32S3GUI_BSP_ONX_H */
