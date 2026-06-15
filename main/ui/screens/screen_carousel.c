/**
 * @file screen_carousel.c
 * @brief esp32s3GUI – jedyny ekran: karuzela (kopia układu HEOStation select_speaker).
 */

#include "screen_carousel.h"
#include "ui_kit.h"
#include "ui_carousel.h"
#include "layout.h"
#include "fake_speakers.h"
#include "ui_fps.h"
#include "esp_log.h"

static const char *TAG = "screen_carousel";

lv_obj_t *screen_carousel_load(void)
{
    lv_obj_t *scr = ui_root_create();

    lv_obj_t *hint = ui_headline(scr, "Tapnij aby wybrac glosnik");
    lv_obj_set_style_margin_top(hint, LAYOUT_GAP_L, 0);

    uint8_t n = fake_speakers_count();
    int8_t  active = (n > 3) ? 3 : 0;

    if (n > 0) {
        lv_obj_t *car = ui_carousel_create(scr);
        for (uint8_t i = 0; i < n; i++) {
            ui_carousel_add_tile(car, fake_speakers_name(i), i,
                                 (active >= 0 && (uint8_t)active == i));
        }
        ui_carousel_finalize(car, active);
    } else {
        ui_caption(scr, "Brak dostepnych glosnikow");
    }

    ui_fps_attach(scr);
    lv_screen_load(scr);
    ESP_LOGI(TAG, "carousel screen loaded (speakers=%u, active=%d)",
             (unsigned)n, (int)active);
    return scr;
}
