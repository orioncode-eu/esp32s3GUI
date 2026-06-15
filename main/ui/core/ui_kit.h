/**
 * @file ui_kit.h
 * @brief esp32s3GUI – deklaratywny builder UI.
 *
 * Konstruktory tworzące widgety LVGL z parametrami semantycznymi (rola, tekst,
 * akcja). Kolory pobierane z ui_theme (skórki), wymiary względne (flex/lv_pct).
 */

#ifndef ESP32S3GUI_UI_KIT_H
#define ESP32S3GUI_UI_KIT_H

#include "lvgl.h"
#include <stdint.h>

/* ── Akcje UI ──────────────────────────────────────────────────────────────
 * Minimalny zestaw na potrzeby benchmarku (bez kolejek/tasków jak w HEOStation).
 * Kliknięcie kafla karuzeli / pozycji listy wywołuje zarejestrowany handler. */
typedef enum {
    UI_ACTION_NONE = 0,
    UI_ACTION_SELECT_SPEAKER,
} UiActionId;

/** Sygnatura handlera akcji UI. arg = np. indeks głośnika. */
typedef void (*UiActionHandler)(UiActionId action, int8_t arg);

/** @brief Rejestruje globalny handler akcji (NULL → tylko log). */
void ui_kit_set_action_handler(UiActionHandler handler);

/**
 * @brief Podpina pod obiekt callback kliknięcia wywołujący zarejestrowany
 *        handler akcji. Pakowanie (action << 8) | (uint8_t)arg – jedno źródło
 *        konwencji dla ui_kit.
 */
void ui_kit_attach_action(lv_obj_t *obj, UiActionId action, int8_t arg);

/** @brief Wywołuje zarejestrowany handler akcji (używane m.in. przez karuzelę). */
void ui_kit_emit_action(UiActionId action, int8_t arg);

/** @brief Korzeń ekranu: pełnoekranowy kontener flex-column z tłem skórki. */
lv_obj_t *ui_root_create(void);

/** @brief Poziomy pasek (flex-row, 100% szerokości). */
lv_obj_t *ui_row_create(lv_obj_t *parent);

/* Etykiety wg roli czcionki (kolor ze skórki). */
lv_obj_t *ui_title(lv_obj_t *parent, const char *text);
lv_obj_t *ui_headline(lv_obj_t *parent, const char *text);
lv_obj_t *ui_subtitle(lv_obj_t *parent, const char *text);
lv_obj_t *ui_caption(lv_obj_t *parent, const char *text);

/** Styl listy głośników. */
typedef enum {
    UI_LIST_STYLE_CLASSIC = 0,  /**< lv_list domyślny (ikona + tekst). */
    UI_LIST_STYLE_TILE,           /**< wiersze jak kafle karuzeli (pionowo). */
    UI_LIST_STYLE_TILE_SCROLL,    /**< kafle + translate-on-scroll (LVGL Pro). */
} UiListStyle;

/** @brief Lista przewijalna – styl klasyczny. */
lv_obj_t *ui_list_create(lv_obj_t *parent);

/** @brief Lista z wybranym stylem (flex_grow=1). */
lv_obj_t *ui_list_create_ex(lv_obj_t *parent, UiListStyle style);

/** @brief Pozycja głośnika. Klik → UI_ACTION_SELECT_SPEAKER(index). */
lv_obj_t *ui_list_add_speaker(lv_obj_t *list, const char *text, uint8_t index);

/** @brief Pozycja z opcjonalnym wyróżnieniem aktywnego (TILE / TILE_SCROLL). */
lv_obj_t *ui_list_add_speaker_ex(lv_obj_t *list, const char *text, uint8_t index,
                                 bool is_active);

/** @brief Przycisk-ikona z własnym callbackiem (np. zamknięcie/sterowanie). */
lv_obj_t *ui_icon_button_cb(lv_obj_t *parent, const char *symbol,
                            lv_event_cb_t cb, void *user_data);

#endif /* ESP32S3GUI_UI_KIT_H */
