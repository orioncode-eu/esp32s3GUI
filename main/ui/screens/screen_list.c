/**
 * @file screen_list.c
 * @brief esp32s3GUI – ekran 2: lista wyboru (klasyczna lub styl kafli).
 */

#include "screen_list.h"
#include "ui_kit.h"
#include "layout.h"
#include "fake_speakers.h"
#include "ui_fps.h"
#include "esp_log.h"

static const char *TAG = "screen_list";

void screen_list_build(lv_obj_t *scr)
{
    if (scr == NULL) {
        return;
    }

    lv_obj_t *hint = ui_headline(scr, "Tapnij aby wybrac glosnik");
    lv_obj_set_style_margin_top(hint, LAYOUT_GAP_L, 0);
    lv_obj_set_style_margin_bottom(hint, LAYOUT_GAP_L, 0);

    lv_obj_t *list = ui_list_create_ex(scr, UI_LIST_STYLE_TILE_SCROLL);

    uint8_t n = fake_speakers_count();
    int8_t  active = (n > 3) ? 3 : 0;

    for (uint8_t i = 0; i < n; i++) {
        ui_list_add_speaker_ex(list, fake_speakers_name(i), i, (i == (uint8_t)active));
    }
    lv_obj_send_event(list, LV_EVENT_SCROLL, NULL);
}

lv_obj_t *screen_list_load(void)
{
    lv_obj_t *scr = ui_root_create();
    lv_obj_set_flex_align(scr, LV_FLEX_ALIGN_START,
                               LV_FLEX_ALIGN_CENTER,
                               LV_FLEX_ALIGN_CENTER);
    lv_obj_set_size(scr, lv_pct(100), lv_pct(100));
    screen_list_build(scr);
    ui_fps_attach(scr);
    lv_screen_load(scr);
    ESP_LOGI(TAG, "list screen loaded (translate-on-scroll)");
    return scr;
}
