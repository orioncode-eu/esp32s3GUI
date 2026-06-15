/**
 * @file bsp_onx_lvgl.h
 * @brief esp32s3GUI – wpięcie panelu+touch ONX3248G035 w esp_lvgl_port.
 *
 * Po sukcesie:
 *  - lvgl_port_init() jest wywołany (esp_lvgl_port tworzy własny task LVGL).
 *  - panel jest dodany jako display (`lvgl_port_add_disp`).
 *  - touch jest dodany jako indev (`lvgl_port_add_touch`).
 *
 * Wymaga gotowych: bsp_onx_panel_init() + bsp_onx_touch_init().
 */

#ifndef ESP32S3GUI_BSP_ONX_LVGL_H
#define ESP32S3GUI_BSP_ONX_LVGL_H

#include "esp_err.h"

esp_err_t bsp_onx_lvgl_register(void);

#endif /* ESP32S3GUI_BSP_ONX_LVGL_H */
