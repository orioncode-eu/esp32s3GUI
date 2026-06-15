/**
 * @file ui_theme.h
 * @brief esp32s3GUI – paleta motywu UI z wymiennymi skórkami.
 *
 * Kolory są opisane w strukturze UiTheme (wartości hex 0xRRGGBB). Cały UI
 * (ui_kit, ui_carousel, ekrany) pobiera kolory wyłącznie przez gettery
 * ui_color_*(), więc podmiana skórki = podmiana jednego wskaźnika. Skórkę
 * wybiera się raz przy boocie (ui_theme_init wg Kconfig), ale ui_theme_set()
 * pozwala też przełączyć ją w locie (np. przyszły benchmark motywów) – po
 * przełączeniu należy przebudować ekran, by nowe kolory zostały zastosowane.
 */

#ifndef ESP32S3GUI_UI_THEME_H
#define ESP32S3GUI_UI_THEME_H

#include "lvgl.h"
#include <stdint.h>

/**
 * @brief Opis skórki: kolory jako hex 0xRRGGBB (gettery konwertują na lv_color_t).
 *        Trzymamy hex zamiast lv_color_t, by skórki mogły być statycznymi const.
 */
typedef struct {
    const char *name;          /**< nazwa skórki (diagnostyka)            */
    uint32_t bg_top;           /**< tło ekranu (główne)                   */
    uint32_t bg_bottom;        /**< tło dolne (gradient/akcent tła)       */
    uint32_t text;             /**< tekst podstawowy                      */
    uint32_t text_dim;         /**< tekst drugorzędny / podpisy           */
    uint32_t accent;           /**< akcent (obwódki, spinner, wyróżnienia)*/
    uint32_t surface;          /**< powierzchnia (listy, przyciski, kafle)*/
    uint32_t surface_active;   /**< powierzchnia wyróżniona (aktywny kafel)*/
} UiTheme;

/** Wbudowane skórki. */
extern const UiTheme ui_theme_dark;
extern const UiTheme ui_theme_light;

/** @brief Ustawia skórkę wg Kconfig (CONFIG_ESP32S3GUI_THEME_*). Wołać raz na starcie. */
void ui_theme_init(void);

/** @brief Podmienia bieżącą skórkę w locie. NULL → ignorowane. */
void ui_theme_set(const UiTheme *theme);

/** @brief Zwraca bieżącą skórkę (nigdy NULL – domyślnie ciemna). */
const UiTheme *ui_theme_current(void);

/* Gettery kolorów (stała sygnatura – reszta UI ich używa). */
lv_color_t ui_color_bg_top(void);
lv_color_t ui_color_bg_bottom(void);
lv_color_t ui_color_text(void);
lv_color_t ui_color_text_dim(void);
lv_color_t ui_color_accent(void);
lv_color_t ui_color_surface(void);
lv_color_t ui_color_surface_active(void);

#endif /* ESP32S3GUI_UI_THEME_H */
