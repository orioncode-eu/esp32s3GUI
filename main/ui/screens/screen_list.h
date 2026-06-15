/**
 * @file screen_list.h
 * @brief esp32s3GUI – jedyny ekran aplikacji: lista wyboru głośników.
 */

#ifndef ESP32S3GUI_SCREEN_LIST_H
#define ESP32S3GUI_SCREEN_LIST_H

#include "lvgl.h"

/** @brief Buduje listę na korzeniu ekranu. Wołać pod LVGL lockiem. */
void screen_list_build(lv_obj_t *scr);

/** @brief Tworzy i ładuje pełnoekranową listę (translate-on-scroll). */
lv_obj_t *screen_list_load(void);

#endif /* ESP32S3GUI_SCREEN_LIST_H */
