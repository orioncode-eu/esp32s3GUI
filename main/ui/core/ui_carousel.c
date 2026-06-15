/**
 * @file ui_carousel.c
 * @brief esp32s3GUI – karuzela wyboru głośnika (kopia HEOStation_Sonnet).
 *
 * Mechanika 1:1 z HEOStation main/ui/core/ui_carousel.c – bez transform/opacity
 * (wydajność na ONX3248G035 / SPI ST7796U).
 */

#include "ui_carousel.h"
#include "ui_kit.h"
#include "ui_theme.h"
#include "layout.h"

#include <stdint.h>

#define CAROUSEL_TILE_MAIN_PCT   28
#define CAROUSEL_TILE_CROSS_PCT  42
#define CAROUSEL_RADIUS          12
#define CAROUSEL_BORDER_W         3
#define CAROUSEL_SIDE_INSET_W    14
#define CAROUSEL_TILE_GAP         2
#define CAROUSEL_USE_MOMENTUM     0

static inline bool carousel_is_horizontal(lv_obj_t *car)
{
    return (bool)(intptr_t)lv_obj_get_user_data(car);
}

static int32_t obj_center_main(lv_obj_t *obj, bool is_h)
{
    lv_area_t a;
    lv_obj_get_coords(obj, &a);
    return is_h ? (a.x1 + a.x2) / 2 : (a.y1 + a.y2) / 2;
}

static lv_obj_t *tile_label(lv_obj_t *tile)
{
    return (lv_obj_get_child_count(tile) > 0) ? lv_obj_get_child(tile, 0) : NULL;
}

static uint32_t carousel_nearest_index(lv_obj_t *car, bool is_h)
{
    int32_t car_center = obj_center_main(car, is_h);
    uint32_t cnt = lv_obj_get_child_count(car);
    uint32_t best_i = 0;
    int32_t  best_d = INT32_MAX;
    for (uint32_t i = 0; i < cnt; i++) {
        int32_t d = obj_center_main(lv_obj_get_child(car, i), is_h) - car_center;
        if (d < 0) d = -d;
        if (d < best_d) { best_d = d; best_i = i; }
    }
    return best_i;
}

static void carousel_apply(lv_obj_t *car)
{
    uint32_t cnt = lv_obj_get_child_count(car);
    if (cnt == 0) return;

    bool     is_h    = carousel_is_horizontal(car);
    uint32_t nearest = carousel_nearest_index(car, is_h);

    for (uint32_t i = 0; i < cnt; i++) {
        lv_obj_t *t   = lv_obj_get_child(car, i);
        lv_obj_t *lbl = tile_label(t);

        lv_obj_set_style_border_opa(t, LV_OPA_COVER, 0);
        if (i == nearest) {
            lv_obj_set_style_border_color(t, ui_color_accent(), 0);
            lv_obj_set_style_border_width(t, CAROUSEL_BORDER_W, 0);
            if (lbl) lv_obj_set_style_text_color(lbl, ui_color_text(), 0);
        } else {
            lv_obj_set_style_border_color(t, ui_color_bg_top(), 0);
            lv_obj_set_style_border_width(t, CAROUSEL_SIDE_INSET_W, 0);
            if (lbl) lv_obj_set_style_text_color(lbl, ui_color_text_dim(), 0);
        }
    }
}

static void cb_scroll(lv_event_t *e)
{
    carousel_apply(lv_event_get_target_obj(e));
}

static void cb_scroll_end(lv_event_t *e)
{
    carousel_apply(lv_event_get_target_obj(e));
}

lv_obj_t *ui_carousel_create(lv_obj_t *parent)
{
    bool is_h = (layout_orientation() == UI_ORIENT_LANDSCAPE);

    lv_obj_t *car = lv_obj_create(parent);
    lv_obj_remove_style_all(car);
    lv_obj_set_width(car, lv_pct(100));
    lv_obj_set_flex_grow(car, 1);
    lv_obj_set_style_bg_opa(car, LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_all(car, 0, 0);
    lv_obj_set_scrollbar_mode(car, LV_SCROLLBAR_MODE_OFF);

    if (is_h) {
        lv_obj_set_flex_flow(car, LV_FLEX_FLOW_ROW);
        lv_obj_set_scroll_dir(car, LV_DIR_HOR);
        lv_obj_set_scroll_snap_x(car, LV_SCROLL_SNAP_CENTER);
        lv_obj_set_style_pad_column(car, CAROUSEL_TILE_GAP, 0);
    } else {
        lv_obj_set_flex_flow(car, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_scroll_dir(car, LV_DIR_VER);
        lv_obj_set_scroll_snap_y(car, LV_SCROLL_SNAP_CENTER);
        lv_obj_set_style_pad_row(car, CAROUSEL_TILE_GAP, 0);
    }
    lv_obj_set_flex_align(car, LV_FLEX_ALIGN_START,
                               LV_FLEX_ALIGN_CENTER,
                               LV_FLEX_ALIGN_CENTER);

#if !CAROUSEL_USE_MOMENTUM
    lv_obj_remove_flag(car, LV_OBJ_FLAG_SCROLL_MOMENTUM);
#endif
    lv_obj_add_flag(car, LV_OBJ_FLAG_SCROLL_ONE);

    lv_obj_set_user_data(car, (void *)(intptr_t)is_h);

    lv_obj_add_event_cb(car, cb_scroll,     LV_EVENT_SCROLL,     NULL);
    lv_obj_add_event_cb(car, cb_scroll_end, LV_EVENT_SCROLL_END, NULL);
    return car;
}

lv_obj_t *ui_carousel_add_tile(lv_obj_t *car, const char *name, uint8_t index,
                               bool is_active)
{
    bool is_h = carousel_is_horizontal(car);

    lv_obj_t *tile = lv_button_create(car);
    lv_obj_remove_style_all(tile);
    lv_obj_remove_flag(tile, LV_OBJ_FLAG_SCROLLABLE);

    if (is_h) {
        lv_obj_set_width(tile,  lv_pct(CAROUSEL_TILE_MAIN_PCT));
        lv_obj_set_height(tile, lv_pct(CAROUSEL_TILE_CROSS_PCT));
    } else {
        lv_obj_set_height(tile, lv_pct(CAROUSEL_TILE_MAIN_PCT));
        lv_obj_set_width(tile,  lv_pct(CAROUSEL_TILE_CROSS_PCT));
    }
    lv_obj_set_style_flex_grow(tile, 0, 0);

    lv_obj_set_style_radius(tile, CAROUSEL_RADIUS, 0);
    lv_obj_set_style_bg_opa(tile, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(tile,
        is_active ? ui_color_surface_active() : ui_color_surface(), 0);
    lv_obj_set_style_pad_all(tile, LAYOUT_GAP_M, 0);

    lv_obj_t *lbl = lv_label_create(tile);
    lv_label_set_text(lbl, name ? name : "?");
    lv_obj_set_width(lbl, lv_pct(100));
    lv_label_set_long_mode(lbl, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(lbl, ui_color_text(), 0);
    lv_obj_set_style_text_font(lbl, layout_font(LAYOUT_FONT_BODY), 0);
    lv_obj_center(lbl);

    ui_kit_attach_action(tile, UI_ACTION_SELECT_SPEAKER, (int8_t)index);
    return tile;
}

void ui_carousel_finalize(lv_obj_t *car, int8_t focus_index)
{
    if (car == NULL) return;
    uint32_t cnt = lv_obj_get_child_count(car);
    if (cnt == 0) return;

    bool is_h = carousel_is_horizontal(car);

    lv_obj_t *parent = lv_obj_get_parent(car);
    if (parent != NULL && is_h) {
        lv_obj_update_layout(parent);
        lv_obj_set_width(car, lv_obj_get_width(parent));
    }

    lv_obj_update_layout(car);

    int32_t main_content  = is_h ? lv_obj_get_content_width(car)
                                 : lv_obj_get_content_height(car);
    int32_t cross_content = is_h ? lv_obj_get_content_height(car)
                                 : lv_obj_get_content_width(car);
    int32_t tile_main  = main_content  * CAROUSEL_TILE_MAIN_PCT  / 100;
    int32_t tile_cross = cross_content * CAROUSEL_TILE_CROSS_PCT / 100;
    if (tile_main  < 1) tile_main  = 1;
    if (tile_cross < 1) tile_cross = 1;

    for (uint32_t i = 0; i < cnt; i++) {
        lv_obj_t *t = lv_obj_get_child(car, i);
        if (is_h) { lv_obj_set_width(t, tile_main);  lv_obj_set_height(t, tile_cross); }
        else      { lv_obj_set_height(t, tile_main); lv_obj_set_width(t, tile_cross); }
    }

    int32_t pad = (main_content - tile_main) / 2;
    if (pad < 0) pad = 0;
    if (is_h) {
        lv_obj_set_style_pad_left(car, pad, 0);
        lv_obj_set_style_pad_right(car, pad, 0);
    } else {
        lv_obj_set_style_pad_top(car, pad, 0);
        lv_obj_set_style_pad_bottom(car, pad, 0);
    }
    lv_obj_update_layout(car);

    if (focus_index < 0 || (uint32_t)focus_index >= cnt) focus_index = 0;
    lv_obj_t *ft = lv_obj_get_child(car, (uint32_t)focus_index);
    int32_t diff = obj_center_main(ft, is_h) - obj_center_main(car, is_h);
    if (is_h) lv_obj_scroll_by(car, -diff, 0, LV_ANIM_OFF);
    else      lv_obj_scroll_by(car, 0, -diff, LV_ANIM_OFF);

    carousel_apply(car);
}
