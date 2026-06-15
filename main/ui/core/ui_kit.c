/**
 * @file ui_kit.c
 * @brief esp32s3GUI – implementacja buildera UI.
 */

#include "ui_kit.h"
#include "ui_theme.h"
#include "layout.h"
#include "esp_log.h"

#include <stdint.h>

static const char *TAG = "ui_kit";

static UiActionHandler s_action_handler = NULL;

void ui_kit_set_action_handler(UiActionHandler handler)
{
    s_action_handler = handler;
}

/* Wspólny callback akcji: dekoduje (action, arg) z user_data i wywołuje
 * zarejestrowany handler. Pakowanie: (action << 8) | (uint8_t)arg. */
static void action_event_cb(lv_event_t *e)
{
    uintptr_t packed = (uintptr_t)lv_event_get_user_data(e);
    UiActionId action = (UiActionId)(packed >> 8);
    int8_t     arg    = (int8_t)(uint8_t)(packed & 0xFFu);

    ESP_LOGI(TAG, "action %d arg %d", (int)action, (int)arg);
    if (s_action_handler != NULL) {
        s_action_handler(action, arg);
    }
}

void ui_kit_attach_action(lv_obj_t *obj, UiActionId action, int8_t arg)
{
    uintptr_t packed = ((uintptr_t)action << 8) | (uint8_t)arg;
    lv_obj_add_event_cb(obj, action_event_cb, LV_EVENT_CLICKED, (void *)packed);
}

void ui_kit_emit_action(UiActionId action, int8_t arg)
{
    ESP_LOGI(TAG, "action %d arg %d", (int)action, (int)arg);
    if (s_action_handler != NULL) {
        s_action_handler(action, arg);
    }
}

/* ── Korzeń ekranu ─────────────────────────────────────────────────────── */
lv_obj_t *ui_root_create(void)
{
    lv_obj_t *scr = lv_obj_create(NULL);

    lv_obj_set_style_bg_color(scr, ui_color_bg_top(), 0);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(scr, 0, 0);
    lv_obj_set_style_radius(scr, 0, 0);
    lv_obj_set_style_pad_all(scr, LAYOUT_PAD_SCREEN, 0);
    lv_obj_set_style_pad_row(scr, LAYOUT_GAP_M, 0);
    lv_obj_set_style_pad_column(scr, LAYOUT_GAP_M, 0);
    lv_obj_set_scrollbar_mode(scr, LV_SCROLLBAR_MODE_OFF);
    lv_obj_remove_flag(scr, LV_OBJ_FLAG_SCROLLABLE);

    /* Jedyne miejsce decyzji zależnej od orientacji – obecnie stos pionowy. */
    (void)layout_orientation();
    lv_obj_set_flex_flow(scr, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(scr, LV_FLEX_ALIGN_CENTER,
                               LV_FLEX_ALIGN_CENTER,
                               LV_FLEX_ALIGN_CENTER);
    return scr;
}

lv_obj_t *ui_row_create(lv_obj_t *parent)
{
    lv_obj_t *row = lv_obj_create(parent);
    lv_obj_remove_style_all(row);
    lv_obj_set_width(row, lv_pct(100));
    lv_obj_set_height(row, LV_SIZE_CONTENT);
    lv_obj_set_style_pad_column(row, LAYOUT_GAP_M, 0);
    lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row, LV_FLEX_ALIGN_SPACE_BETWEEN,
                               LV_FLEX_ALIGN_CENTER,
                               LV_FLEX_ALIGN_CENTER);
    lv_obj_remove_flag(row, LV_OBJ_FLAG_SCROLLABLE);
    return row;
}

/* ── Etykiety ─────────────────────────────────────────────────────────── */
static lv_obj_t *make_label(lv_obj_t *parent, const char *text,
                            LayoutFontRole role, lv_color_t color)
{
    lv_obj_t *lbl = lv_label_create(parent);
    lv_label_set_text(lbl, text ? text : "");
    lv_obj_set_style_text_font(lbl, layout_font(role), 0);
    lv_obj_set_style_text_color(lbl, color, 0);
    return lbl;
}

lv_obj_t *ui_title(lv_obj_t *parent, const char *text)
{
    return make_label(parent, text, LAYOUT_FONT_TITLE, ui_color_text());
}

lv_obj_t *ui_headline(lv_obj_t *parent, const char *text)
{
    return make_label(parent, text, LAYOUT_FONT_HEADLINE, ui_color_text_dim());
}

lv_obj_t *ui_subtitle(lv_obj_t *parent, const char *text)
{
    return make_label(parent, text, LAYOUT_FONT_BODY, ui_color_text_dim());
}

lv_obj_t *ui_caption(lv_obj_t *parent, const char *text)
{
    return make_label(parent, text, LAYOUT_FONT_CAPTION, ui_color_text_dim());
}

/* ── Lista ────────────────────────────────────────────────────────────── */
#define UI_TILE_RADIUS    12
#define UI_TILE_BORDER_W   3
#define UI_TILE_ROW_GAP    2
#define UI_LIST_SCROLLBAR_W  8
#define UI_LIST_SCROLLBAR_PAD  4

static lv_obj_t *tile_list_find_label(lv_obj_t *btn)
{
    uint32_t n = lv_obj_get_child_count(btn);
    for (uint32_t i = 0; i < n; i++) {
        lv_obj_t *c = lv_obj_get_child(btn, i);
        if (lv_obj_check_type(c, &lv_label_class)) {
            return c;
        }
    }
    return NULL;
}

static void style_tile_list_row(lv_obj_t *btn, bool is_active)
{
    lv_obj_set_width(btn, lv_pct(100));
    lv_obj_set_height(btn, layout_tile_list_row_px());
    lv_obj_set_style_radius(btn, UI_TILE_RADIUS, 0);
    lv_obj_set_style_bg_opa(btn, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(btn,
        is_active ? ui_color_surface_active() : ui_color_surface(), 0);
    lv_obj_set_style_border_opa(btn, LV_OPA_COVER, 0);
    if (is_active) {
        lv_obj_set_style_border_width(btn, UI_TILE_BORDER_W, 0);
        lv_obj_set_style_border_color(btn, ui_color_accent(), 0);
    } else {
        lv_obj_set_style_border_width(btn, 0, 0);
    }
    lv_obj_set_style_pad_all(btn, LAYOUT_GAP_M, 0);
    lv_obj_set_style_shadow_width(btn, 0, 0);
    lv_obj_set_style_bg_color(btn, ui_color_surface_active(), LV_STATE_PRESSED);

    lv_obj_set_flex_flow(btn, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(btn, LV_FLEX_ALIGN_CENTER,
                               LV_FLEX_ALIGN_CENTER,
                               LV_FLEX_ALIGN_CENTER);

    lv_obj_t *lbl = tile_list_find_label(btn);
    if (lbl != NULL) {
        lv_obj_set_width(lbl, lv_pct(100));
        lv_label_set_long_mode(lbl, LV_LABEL_LONG_WRAP);
        lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_font(lbl, layout_font(LAYOUT_FONT_BODY), 0);
        lv_obj_set_style_text_color(lbl,
            is_active ? ui_color_text() : ui_color_text_dim(), 0);
    }
}

static void style_tile_list_scrollbar(lv_obj_t *list)
{
    const int32_t pad_r = UI_LIST_SCROLLBAR_W + UI_LIST_SCROLLBAR_PAD;

    lv_obj_set_scrollbar_mode(list, LV_SCROLLBAR_MODE_ON);
    /* Treść (kafle) kończy się przed paskiem – scrollbar ma własny pas z prawej. */
    lv_obj_set_style_pad_right(list, pad_r, 0);
    lv_obj_set_style_width(list, UI_LIST_SCROLLBAR_W, LV_PART_SCROLLBAR);
    lv_obj_set_style_bg_color(list, ui_color_surface(), LV_PART_SCROLLBAR);
    lv_obj_set_style_bg_opa(list, LV_OPA_COVER, LV_PART_SCROLLBAR);
    lv_obj_set_style_radius(list, 4, LV_PART_SCROLLBAR);
    lv_obj_set_style_pad_all(list, 2, LV_PART_SCROLLBAR);
}

static bool ui_list_is_tile_style(UiListStyle style)
{
    return style == UI_LIST_STYLE_TILE || style == UI_LIST_STYLE_TILE_SCROLL;
}

static void tile_scroll_translate_cb(lv_event_t *e)
{
    lv_obj_t *cont = lv_event_get_target_obj(e);
    UiListStyle style = (UiListStyle)(intptr_t)lv_obj_get_user_data(cont);
    if (style != UI_LIST_STYLE_TILE_SCROLL) {
        return;
    }

    lv_area_t cont_a;
    lv_obj_get_coords(cont, &cont_a);
    int32_t cont_y_center = cont_a.y1 + lv_area_get_height(&cont_a) / 2;
    int32_t r = lv_obj_get_height(cont) * 7 / 10;
    int32_t child_cnt = (int32_t)lv_obj_get_child_count(cont);

    for (int32_t i = 0; i < child_cnt; i++) {
        lv_obj_t *child = lv_obj_get_child(cont, i);
        lv_area_t child_a;
        lv_obj_get_coords(child, &child_a);
        int32_t child_y_center = child_a.y1 + lv_area_get_height(&child_a) / 2;
        int32_t diff_y = LV_ABS(child_y_center - cont_y_center);
        int32_t x;
        if (diff_y >= r) {
            x = r;
        } else {
            uint32_t x_sqr = (uint32_t)(r * r - diff_y * diff_y);
            lv_sqrt_res_t res;
            lv_sqrt(x_sqr, &res, 0x8000);
            x = r - res.i;
        }
        lv_obj_set_style_translate_x(child, x, 0);
    }
}

static void ui_list_attach_tile_scroll(lv_obj_t *list)
{
    lv_obj_add_event_cb(list, tile_scroll_translate_cb, LV_EVENT_SCROLL, NULL);
    lv_obj_add_event_cb(list, tile_scroll_translate_cb, LV_EVENT_CHILD_CHANGED, NULL);
    lv_obj_set_scroll_dir(list, LV_DIR_VER);
}

lv_obj_t *ui_list_create(lv_obj_t *parent)
{
    return ui_list_create_ex(parent, UI_LIST_STYLE_CLASSIC);
}

lv_obj_t *ui_list_create_ex(lv_obj_t *parent, UiListStyle style)
{
    lv_obj_t *list = lv_list_create(parent);
    lv_obj_set_width(list, lv_pct(100));
    lv_obj_set_flex_grow(list, 1);

    if (style == UI_LIST_STYLE_TILE) {
        lv_obj_set_style_bg_opa(list, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_width(list, 0, 0);
        lv_obj_set_style_pad_left(list, 0, 0);
        lv_obj_set_style_pad_top(list, 0, 0);
        lv_obj_set_style_pad_bottom(list, 0, 0);
        lv_obj_set_style_pad_row(list, UI_TILE_ROW_GAP, 0);
        style_tile_list_scrollbar(list);
    } else if (style == UI_LIST_STYLE_TILE_SCROLL) {
        lv_obj_set_style_bg_opa(list, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_width(list, 0, 0);
        lv_obj_set_style_pad_left(list, 0, 0);
        lv_obj_set_style_pad_top(list, 0, 0);
        lv_obj_set_style_pad_bottom(list, 0, 0);
        lv_obj_set_style_pad_row(list, UI_TILE_ROW_GAP, 0);
        lv_obj_set_scrollbar_mode(list, LV_SCROLLBAR_MODE_OFF);
        ui_list_attach_tile_scroll(list);
    } else {
        lv_obj_set_scrollbar_mode(list, LV_SCROLLBAR_MODE_OFF);
        lv_obj_set_style_bg_color(list, ui_color_surface(), 0);
        lv_obj_set_style_bg_opa(list, LV_OPA_COVER, 0);
        lv_obj_set_style_border_width(list, 0, 0);
        lv_obj_set_style_radius(list, LAYOUT_GAP_M, 0);
        lv_obj_set_style_pad_all(list, LAYOUT_GAP_S, 0);
    }
    lv_obj_set_user_data(list, (void *)(intptr_t)style);
    return list;
}

lv_obj_t *ui_list_add_speaker(lv_obj_t *list, const char *text, uint8_t index)
{
    return ui_list_add_speaker_ex(list, text, index, false);
}

lv_obj_t *ui_list_add_speaker_ex(lv_obj_t *list, const char *text, uint8_t index,
                                 bool is_active)
{
    UiListStyle style = (UiListStyle)(intptr_t)lv_obj_get_user_data(list);
    const void *icon = ui_list_is_tile_style(style) ? NULL : LV_SYMBOL_AUDIO;
    lv_obj_t *btn = lv_list_add_button(list, icon, text ? text : "?");
    if (ui_list_is_tile_style(style)) {
        style_tile_list_row(btn, is_active);
    } else {
        lv_obj_set_style_bg_opa(btn, LV_OPA_TRANSP, 0);
        lv_obj_set_style_text_color(btn, ui_color_text(), 0);
        lv_obj_set_style_text_font(btn, layout_font(LAYOUT_FONT_BODY), 0);
    }

    ui_kit_attach_action(btn, UI_ACTION_SELECT_SPEAKER, (int8_t)index);
    return btn;
}

/* ── Przyciski ────────────────────────────────────────────────────────── */
lv_obj_t *ui_icon_button_cb(lv_obj_t *parent, const char *symbol,
                            lv_event_cb_t cb, void *user_data)
{
    lv_obj_t *btn = lv_button_create(parent);
    lv_obj_set_style_bg_color(btn, ui_color_surface(), 0);
    lv_obj_set_style_bg_opa(btn, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(btn, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_pad_all(btn, LAYOUT_GAP_M, 0);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t *lbl = lv_label_create(btn);
    lv_label_set_text(lbl, symbol ? symbol : LV_SYMBOL_CLOSE);
    lv_obj_set_style_text_color(lbl, ui_color_text(), 0);
    lv_obj_set_style_text_font(lbl, lv_font_get_default(), 0);
    lv_obj_remove_flag(lbl, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_center(lbl);

    if (cb != NULL) {
        lv_obj_add_event_cb(btn, cb, LV_EVENT_CLICKED, user_data);
    }
    return btn;
}
