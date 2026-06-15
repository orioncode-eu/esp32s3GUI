/**
 * @file layout.c
 * @brief esp32s3GUI – orientacja runtime + mapowanie czcionek.
 */

#include "layout.h"

UiOrient layout_orientation(void)
{
    lv_display_t *disp = lv_display_get_default();
    if (disp == NULL) {
        return (LAYOUT_W >= LAYOUT_H) ? UI_ORIENT_LANDSCAPE : UI_ORIENT_PORTRAIT;
    }

    int32_t hor = lv_display_get_horizontal_resolution(disp);
    int32_t ver = lv_display_get_vertical_resolution(disp);
    return (hor >= ver) ? UI_ORIENT_LANDSCAPE : UI_ORIENT_PORTRAIT;
}

const lv_font_t *layout_font(LayoutFontRole role)
{
    switch (role) {
        case LAYOUT_FONT_TITLE:    return &lv_font_montserrat_24;
        case LAYOUT_FONT_HEADLINE: return &lv_font_montserrat_20;
        case LAYOUT_FONT_BODY:     return &lv_font_montserrat_16;
        case LAYOUT_FONT_CAPTION: return &lv_font_montserrat_14;
        default:                  return &lv_font_montserrat_16;
    }
}

int32_t layout_tile_list_row_px(void)
{
    lv_display_t *disp = lv_display_get_default();
    int32_t hor = LAYOUT_W;
    int32_t ver = LAYOUT_H;
    if (disp != NULL) {
        hor = lv_display_get_horizontal_resolution(disp);
        ver = lv_display_get_vertical_resolution(disp);
    }

    int32_t cross = (layout_orientation() == UI_ORIENT_LANDSCAPE) ? ver : hor;
    /* ~20% wysokości ekranu – czytelny wiersz listy. */
    int32_t px = (cross * 20) / 100;
    if (px < 48) {
        px = 48;
    }
    if (px > 72) {
        px = 72;
    }
    return px;
}
