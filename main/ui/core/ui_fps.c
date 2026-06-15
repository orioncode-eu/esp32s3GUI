/**
 * @file ui_fps.c
 * @brief esp32s3GUI – licznik FPS.
 *
 * Licznik klatek rejestruje się raz (ui_fps_init); etykietę tworzy ui_fps_attach
 * jako FLOATING dziecko bieżącego lv_screen (nie lv_layer_top()).
 */

#include "ui_fps.h"
#include "ui_theme.h"
#include "layout.h"

static uint32_t    s_frame_cnt = 0;
static lv_obj_t   *s_lbl       = NULL;
static lv_timer_t *s_timer     = NULL;

static void disp_refr_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_REFR_READY) {
        s_frame_cnt++;
    }
}

static void fps_timer_cb(lv_timer_t *timer)
{
    (void)timer;
    if (s_lbl == NULL) {
        return;
    }

    char buf[16];
    lv_snprintf(buf, sizeof(buf), "%u FPS", (unsigned)s_frame_cnt);
    lv_label_set_text(s_lbl, buf);
    s_frame_cnt = 0;
}

void ui_fps_init(void)
{
    if (s_timer != NULL) {
        return;
    }

    lv_display_t *disp = lv_display_get_default();
    if (disp != NULL) {
        lv_display_add_event_cb(disp, disp_refr_cb, LV_EVENT_REFR_READY, NULL);
    }

    s_timer = lv_timer_create(fps_timer_cb, 1000, NULL);
}

void ui_fps_attach(lv_obj_t *scr)
{
    if (scr == NULL) {
        return;
    }

    lv_obj_t *box = lv_obj_create(scr);
    lv_obj_add_flag(box, LV_OBJ_FLAG_FLOATING);
    lv_obj_set_size(box, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_align(box, LV_ALIGN_TOP_RIGHT, -LAYOUT_PAD_SCREEN, LAYOUT_PAD_SCREEN);
    lv_obj_set_style_bg_color(box, ui_color_surface(), 0);
    lv_obj_set_style_bg_opa(box, LV_OPA_80, 0);
    lv_obj_set_style_border_width(box, 0, 0);
    lv_obj_set_style_radius(box, 6, 0);
    lv_obj_set_style_pad_hor(box, LAYOUT_GAP_M, 0);
    lv_obj_set_style_pad_ver(box, LAYOUT_GAP_S, 0);
    lv_obj_remove_flag(box, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_remove_flag(box, LV_OBJ_FLAG_CLICKABLE);

    s_lbl = lv_label_create(box);
    lv_label_set_text(s_lbl, "-- FPS");
    lv_obj_set_style_text_color(s_lbl, ui_color_text(), 0);
    lv_obj_set_style_text_font(s_lbl, layout_font(LAYOUT_FONT_CAPTION), 0);
    lv_obj_move_foreground(box);
}
