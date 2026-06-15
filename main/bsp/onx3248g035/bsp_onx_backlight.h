/**
 * @file bsp_onx_backlight.h
 * @brief esp32s3GUI – podświetlenie LCD przez LEDC PWM (ONX3248G035).
 *
 * 10-bit rozdzielczość PWM, 10 kHz. Ustawienie na ~100% w bsp_onx_backlight_init();
 * w przyszłości (auto-dim, sleep) można wywoływać bsp_onx_backlight_set_pct().
 */

#ifndef ESP32S3GUI_BSP_ONX_BACKLIGHT_H
#define ESP32S3GUI_BSP_ONX_BACKLIGHT_H

#include "esp_err.h"
#include <stdint.h>

esp_err_t bsp_onx_backlight_init(void);

/** @brief Ustaw poziom podświetlenia (0..100 %). */
esp_err_t bsp_onx_backlight_set_pct(uint8_t pct);

#endif /* ESP32S3GUI_BSP_ONX_BACKLIGHT_H */
