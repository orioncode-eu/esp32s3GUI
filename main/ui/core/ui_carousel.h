/**
 * @file ui_carousel.h
 * @brief esp32s3GUI – karuzela wyboru głośnika (snapping wheel, LVGL v9).
 *
 * Kopia mechaniki z HEOStation_Sonnet (ui_carousel.c). Kolory z ui_theme,
 * kierunek z layout_orientation().
 *
 * MUSI być wołane pod LVGL lockiem.
 */

#ifndef ESP32S3GUI_UI_CAROUSEL_H
#define ESP32S3GUI_UI_CAROUSEL_H

#include "lvgl.h"
#include <stdint.h>
#include <stdbool.h>

lv_obj_t *ui_carousel_create(lv_obj_t *parent);

lv_obj_t *ui_carousel_add_tile(lv_obj_t *car, const char *name, uint8_t index,
                               bool is_active);

void ui_carousel_finalize(lv_obj_t *car, int8_t focus_index);

#endif /* ESP32S3GUI_UI_CAROUSEL_H */
