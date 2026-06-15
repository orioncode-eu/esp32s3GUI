/**
 * @file ui_theme.c
 * @brief esp32s3GUI – implementacja skórek i getterów kolorów.
 *
 * Dodanie nowej skórki: zdefiniuj kolejny const UiTheme, wystaw go w nagłówku
 * i (opcjonalnie) dodaj wybór w Kconfig + gałąź w ui_theme_init(). Reszta UI
 * nie wymaga zmian – pobiera kolory przez gettery.
 */

#include "ui_theme.h"
#include "sdkconfig.h"

const UiTheme ui_theme_dark = {
    .name           = "dark",
    .bg_top         = 0x101826,   /* granat                 */
    .bg_bottom      = 0x05070C,   /* prawie czarny          */
    .text           = 0xFFFFFF,
    .text_dim       = 0x9AA4B2,
    .accent         = 0x2D9CDB,   /* niebieski              */
    .surface        = 0x1B2433,
    .surface_active = 0x1E3A52,
};

const UiTheme ui_theme_light = {
    .name           = "light",
    .bg_top         = 0xF4F6F8,
    .bg_bottom      = 0xE8ECF0,
    .text           = 0x1A1F26,
    .text_dim       = 0x5C6773,
    .accent         = 0x1565C0,
    .surface        = 0xFFFFFF,
    .surface_active = 0xD6E8F7,
};

/* Domyślnie ciemna, by gettery działały nawet przed ui_theme_init(). */
static const UiTheme *s_theme = &ui_theme_dark;

void ui_theme_init(void)
{
#if defined(CONFIG_ESP32S3GUI_THEME_LIGHT)
    s_theme = &ui_theme_light;
#else
    s_theme = &ui_theme_dark;
#endif
}

void ui_theme_set(const UiTheme *theme)
{
    if (theme != NULL) {
        s_theme = theme;
    }
}

const UiTheme *ui_theme_current(void)
{
    return s_theme;
}

lv_color_t ui_color_bg_top(void)         { return lv_color_hex(s_theme->bg_top); }
lv_color_t ui_color_bg_bottom(void)      { return lv_color_hex(s_theme->bg_bottom); }
lv_color_t ui_color_text(void)           { return lv_color_hex(s_theme->text); }
lv_color_t ui_color_text_dim(void)       { return lv_color_hex(s_theme->text_dim); }
lv_color_t ui_color_accent(void)         { return lv_color_hex(s_theme->accent); }
lv_color_t ui_color_surface(void)        { return lv_color_hex(s_theme->surface); }
lv_color_t ui_color_surface_active(void) { return lv_color_hex(s_theme->surface_active); }
