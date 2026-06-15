/**
 * @file screen_strony.c
 * @brief esp32s3GUI – STRONY: tileview (20 kafli, 4 strony × 6) + paginacja.
 */

#include "screen_strony.h"
#include "ui_kit.h"
#include "ui_theme.h"
#include "ui_fps.h"
#include "layout.h"
#include "esp_log.h"

static const char *TAG = "screen_strony";

#define STRONY_TILE_COUNT      20
#define STRONY_TILES_PER_PAGE  6
#define STRONY_PAGE_COUNT      ((STRONY_TILE_COUNT + STRONY_TILES_PER_PAGE - 1) / STRONY_TILES_PER_PAGE)

#define STRONY_PAGE_DOT_ACTIVE_W   36
#define STRONY_PAGE_DOT_ACTIVE_H   10
#define STRONY_PAGE_DOT_INACTIVE   10
#define STRONY_PAGE_DOT_GAP        10
#define STRONY_PAGE_DOT_RADIUS     5

#define STRONY_PAGINATION_BAR_H  (STRONY_PAGE_DOT_ACTIVE_H + LAYOUT_GAP_M * 2 + LAYOUT_GAP_L)

static lv_obj_t *s_tv         = NULL;
static lv_obj_t *s_pagination = NULL;
static lv_obj_t *s_page_lbl   = NULL;

static const lv_coord_t s_grid_col_dsc[] = {
    LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1),
    LV_GRID_TEMPLATE_LAST
};

static const lv_coord_t s_grid_row_dsc[] = {
    LV_GRID_FR(1), LV_GRID_FR(1),
    LV_GRID_TEMPLATE_LAST
};

static void update_pagination(int page);
static void refresh_chrome(int page);
static void tileview_sync_tile_sizes(lv_obj_t *tv);

static void tileview_size_changed_cb(lv_event_t *e)
{
    tileview_sync_tile_sizes(lv_event_get_target_obj(e));
}

static void refresh_chrome(int page)
{
    if (page < 0) {
        page = 0;
    }
    if (page >= STRONY_PAGE_COUNT) {
        page = STRONY_PAGE_COUNT - 1;
    }
    if (s_page_lbl != NULL) {
        lv_label_set_text_fmt(s_page_lbl, "strona %d / %d", page + 1, STRONY_PAGE_COUNT);
    }

    update_pagination(page);
}

static void update_pagination(int page)
{
    if (s_pagination == NULL) {
        return;
    }

    lv_obj_clean(s_pagination);

    for (int i = 0; i < STRONY_PAGE_COUNT; i++) {
        lv_obj_t *dot = lv_obj_create(s_pagination);
        lv_obj_set_style_border_width(dot, 0, 0);
        lv_obj_set_style_pad_all(dot, 0, 0);
        lv_obj_set_style_bg_opa(dot, LV_OPA_COVER, 0);
        lv_obj_remove_flag(dot, LV_OBJ_FLAG_SCROLLABLE);

        if (i == page) {
            lv_obj_set_size(dot, STRONY_PAGE_DOT_ACTIVE_W, STRONY_PAGE_DOT_ACTIVE_H);
            lv_obj_set_style_bg_color(dot, ui_color_text(), 0);
            lv_obj_set_style_radius(dot, STRONY_PAGE_DOT_RADIUS, 0);
        } else {
            lv_obj_set_size(dot, STRONY_PAGE_DOT_INACTIVE, STRONY_PAGE_DOT_INACTIVE);
            lv_obj_set_style_bg_color(dot, ui_color_text_dim(), 0);
            lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
        }
    }
}

static void tileview_changed_cb(lv_event_t *e)
{
    lv_obj_t *tv = lv_event_get_target_obj(e);
    lv_obj_t *active = lv_tileview_get_tile_active(tv);
    if (active == NULL) {
        return;
    }
    refresh_chrome((int)lv_obj_get_index(active));
}

static void tv_scroll_cb(lv_event_t *e)
{
    (void)e;
    ESP_LOGI(TAG, "tileview scroll event");
}

static void build_tile_page(lv_obj_t *tv, int page)
{
    lv_obj_t *tile = lv_tileview_add_tile(tv, (uint8_t)page, 0, LV_DIR_HOR);
    lv_obj_set_size(tile, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_opa(tile, LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_all(tile, LAYOUT_GAP_M, 0);
    lv_obj_set_layout(tile, LV_LAYOUT_GRID);
    lv_obj_set_grid_dsc_array(tile, s_grid_col_dsc, s_grid_row_dsc);
    lv_obj_set_style_pad_column(tile, LAYOUT_GAP_M, 0);
    lv_obj_set_style_pad_row(tile, LAYOUT_GAP_M, 0);
    lv_obj_remove_flag(tile, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scroll_dir(tile, LV_DIR_NONE);

    for (int t = 0; t < STRONY_TILES_PER_PAGE; t++) {
        const int idx = page * STRONY_TILES_PER_PAGE + t;
        if (idx >= STRONY_TILE_COUNT) {
            break;
        }

        const int col = t % 3;
        const int row = t / 3;

        lv_obj_t *card = lv_button_create(tile);
        lv_obj_remove_style_all(card);
        lv_obj_remove_flag(card, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_grid_cell(card,
                             LV_GRID_ALIGN_STRETCH, col, 1,
                             LV_GRID_ALIGN_STRETCH, row, 1);
        lv_obj_set_style_bg_color(card, ui_color_surface(), 0);
        lv_obj_set_style_bg_opa(card, LV_OPA_COVER, 0);
        lv_obj_set_style_border_color(card, ui_color_accent(), 0);
        lv_obj_set_style_border_width(card, 2, 0);
        lv_obj_set_style_radius(card, 10, 0);
        lv_obj_set_style_pad_all(card, LAYOUT_GAP_S, 0);
        lv_obj_set_flex_flow(card, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_align(card, LV_FLEX_ALIGN_CENTER,
                                   LV_FLEX_ALIGN_CENTER,
                                   LV_FLEX_ALIGN_CENTER);

        lv_obj_t *num = lv_label_create(card);
        lv_label_set_text_fmt(num, "%d", idx + 1);
        lv_obj_set_style_text_color(num, ui_color_accent(), 0);
        lv_obj_set_style_text_font(num, layout_font(LAYOUT_FONT_TITLE), 0);

        lv_obj_t *cap = lv_label_create(card);
        lv_label_set_text(cap, "kafel");
        lv_obj_set_style_text_color(cap, ui_color_text(), 0);
        lv_obj_set_style_text_font(cap, layout_font(LAYOUT_FONT_CAPTION), 0);

        ui_kit_attach_action(card, UI_ACTION_SELECT_SPEAKER, (int8_t)idx);
    }
}

static void tileview_sync_tile_sizes(lv_obj_t *tv)
{
    lv_obj_update_layout(tv);
    const int32_t w = lv_obj_get_content_width(tv);
    const int32_t h = lv_obj_get_content_height(tv);
    const uint32_t n = lv_obj_get_child_count(tv);
    for (uint32_t i = 0; i < n; i++) {
        lv_obj_t *tile = lv_obj_get_child(tv, i);
        lv_obj_set_size(tile, w, h);
    }
    lv_obj_update_layout(tv);
}

lv_obj_t *screen_strony_load(void)
{
    s_tv = NULL;
    s_pagination = NULL;
    s_page_lbl = NULL;

    lv_obj_t *scr = ui_root_create();
    lv_obj_set_flex_align(scr, LV_FLEX_ALIGN_START,
                               LV_FLEX_ALIGN_CENTER,
                               LV_FLEX_ALIGN_CENTER);
    lv_obj_set_size(scr, lv_pct(100), lv_pct(100));
    /* Ekran ma stały layout – bez scrolla pionowego rodzica tileview. */
    lv_obj_remove_flag(scr, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *header = ui_row_create(scr);
    lv_obj_set_style_pad_hor(header, LAYOUT_PAD_SCREEN, 0);

    s_page_lbl = lv_label_create(header);
    lv_label_set_text(s_page_lbl, "strona 1 / 4");
    lv_obj_set_style_text_color(s_page_lbl, ui_color_text(), 0);
    lv_obj_set_style_text_font(s_page_lbl, layout_font(LAYOUT_FONT_BODY), 0);
    lv_obj_set_width(s_page_lbl, lv_pct(100));
    lv_obj_set_style_text_align(s_page_lbl, LV_TEXT_ALIGN_CENTER, 0);

    s_tv = lv_tileview_create(scr);
    lv_obj_set_width(s_tv, lv_pct(100));
    lv_obj_set_flex_grow(s_tv, 1);
    lv_obj_set_style_bg_opa(s_tv, LV_OPA_TRANSP, 0);
    /* Tylko poziomo – flex_grow inaczej daje też scroll pionowy (gest ginie na kafelkach). */
    lv_obj_set_scroll_dir(s_tv, LV_DIR_HOR);
    lv_obj_set_scrollbar_mode(s_tv, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_event_cb(s_tv, tileview_changed_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(s_tv, tileview_size_changed_cb, LV_EVENT_SIZE_CHANGED, NULL);
    lv_obj_add_event_cb(s_tv, tv_scroll_cb, LV_EVENT_SCROLL, NULL);
    lv_obj_add_event_cb(s_tv, tv_scroll_cb, LV_EVENT_SCROLL_BEGIN, NULL);

    for (int p = 0; p < STRONY_PAGE_COUNT; p++) {
        build_tile_page(s_tv, p);
    }

    s_pagination = lv_obj_create(scr);
    lv_obj_set_width(s_pagination, lv_pct(100));
    lv_obj_set_height(s_pagination, STRONY_PAGINATION_BAR_H);
    lv_obj_set_style_bg_opa(s_pagination, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(s_pagination, 0, 0);
    lv_obj_set_style_pad_all(s_pagination, LAYOUT_GAP_M, 0);
    lv_obj_set_style_pad_bottom(s_pagination, LAYOUT_GAP_L, 0);
    lv_obj_set_flex_flow(s_pagination, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(s_pagination,
                               LV_FLEX_ALIGN_CENTER,
                               LV_FLEX_ALIGN_CENTER,
                               LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(s_pagination, STRONY_PAGE_DOT_GAP, 0);
    lv_obj_remove_flag(s_pagination, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_remove_flag(s_pagination, LV_OBJ_FLAG_CLICKABLE);

    refresh_chrome(0);
    lv_obj_update_layout(scr);
    tileview_sync_tile_sizes(s_tv);

    ui_fps_attach(scr);
    lv_screen_load(scr);
    ESP_LOGI(TAG, "STRONY screen loaded (%d tiles, %d pages)",
             STRONY_TILE_COUNT, STRONY_PAGE_COUNT);
    return scr;
}
