/**
 * @file screen_strony.h
 * @brief esp32s3GUI – jedyny ekran: siatka kafli w tileview + paginacja (STRONY).
 */

#ifndef ESP32S3GUI_SCREEN_STRONY_H
#define ESP32S3GUI_SCREEN_STRONY_H

#include "lvgl.h"

/** @brief Tworzy i ładuje ekran STRONY (tileview 4×6 kafli + paginacja + FPS). */
lv_obj_t *screen_strony_load(void);

#endif /* ESP32S3GUI_SCREEN_STRONY_H */
