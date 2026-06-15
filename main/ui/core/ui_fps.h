/**
 * @file ui_fps.h
 * @brief esp32s3GUI – licznik FPS (etykieta FLOATING na ekranie).
 */

#ifndef ESP32S3GUI_UI_FPS_H
#define ESP32S3GUI_UI_FPS_H

#include "lvgl.h"

/** @brief Rejestruje licznik klatek (display event + timer 1 s). Wołać raz przy starcie. */
void ui_fps_init(void);

/** @brief Dodaje okienko FPS jako FLOATING dziecko ekranu. Wołać pod LVGL lockiem. */
void ui_fps_attach(lv_obj_t *scr);

#endif /* ESP32S3GUI_UI_FPS_H */
