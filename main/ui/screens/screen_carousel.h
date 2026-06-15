/**
 * @file screen_carousel.h
 * @brief esp32s3GUI – jedyny ekran aplikacji: karuzela wyboru głośników.
 */

#ifndef ESP32S3GUI_SCREEN_CAROUSEL_H
#define ESP32S3GUI_SCREEN_CAROUSEL_H

#include "lvgl.h"

/** @brief Tworzy i ładuje pełnoekranową karuzelę (fake_speakers + FPS). */
lv_obj_t *screen_carousel_load(void);

#endif /* ESP32S3GUI_SCREEN_CAROUSEL_H */
