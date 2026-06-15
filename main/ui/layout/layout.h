/**
 * @file layout.h
 * @brief esp32s3GUI – warstwa abstrakcji layoutu ekranu.
 */

#ifndef ESP32S3GUI_LAYOUT_H
#define ESP32S3GUI_LAYOUT_H

#include "sdkconfig.h"
#include "lvgl.h"

#define LAYOUT_W  CONFIG_ESP32S3GUI_DISPLAY_WIDTH
#define LAYOUT_H  CONFIG_ESP32S3GUI_DISPLAY_HEIGHT

#define LAYOUT_MARGIN       8
#define LAYOUT_PADDING      6
#define LAYOUT_STATUS_BAR_H 24
#define LAYOUT_CTRL_BTN_SIZE  40
#define LAYOUT_CTRL_BTN_Y     (LAYOUT_H - LAYOUT_CTRL_BTN_SIZE - LAYOUT_MARGIN)

/** Wysokość wiersza listy w stylu „kafel”. */
int32_t layout_tile_list_row_px(void);
#define LAYOUT_VOL_ARC_SIZE   80
#define LAYOUT_VOL_ARC_X      (LAYOUT_W - LAYOUT_VOL_ARC_SIZE - LAYOUT_MARGIN)
#define LAYOUT_VOL_ARC_Y      (LAYOUT_STATUS_BAR_H + LAYOUT_MARGIN)

#define LAYOUT_PAD_SCREEN   12
#define LAYOUT_GAP_S         4
#define LAYOUT_GAP_M         8
#define LAYOUT_GAP_L        16

typedef enum {
    UI_ORIENT_LANDSCAPE = 0,
    UI_ORIENT_PORTRAIT,
} UiOrient;

typedef enum {
    LAYOUT_FONT_TITLE = 0,
    LAYOUT_FONT_HEADLINE,
    LAYOUT_FONT_BODY,
    LAYOUT_FONT_CAPTION,
} LayoutFontRole;

UiOrient layout_orientation(void);
const lv_font_t *layout_font(LayoutFontRole role);

#endif /* ESP32S3GUI_LAYOUT_H */
