# esp32s3GUI — architektura

Projekt benchmarkowy GUI dla ESP32-S3. Wzorzec warstw wyświetlacza pochodzi z [HEOStation_Sonnet](../HEOStation_Sonnet) (tylko odczyt — nie modyfikujemy tamtego repo).

## Warstwy

```
Kconfig (sdkconfig)
    → display_driver (main/hal)
    → BSP per-board (main/bsp/<board>/)
    → esp_lvgl_port (task LVGL + mutex)
    → ui_task (main/tasks)
    → ekrany (main/ui/screens)
    → ui_kit + ui_theme + layout
```

## Wyświetlacz (compile-time)

Menu **esp32s3GUI Display Configuration** w `idf.py menuconfig`:

| Opcja | Proporcje | BSP |
|-------|-----------|-----|
| STUB | brak | `bsp/stub` |
| ILI9341 320×240 | prostokąt | placeholder → STUB |
| ST7789 240×240 | kwadrat | placeholder → STUB |
| GC9A01 240×240 | okrąg | placeholder → STUB |
| ST7796 480×320 (ONX3248G035) | prostokąt | `bsp/onx3248g035` |

`CONFIG_ESP32S3GUI_DISPLAY_WIDTH/HEIGHT` ustawiane automatycznie.

## Orientacja

- Domyślnie landscape (480×320) dla ONX3248G035.
- **Force portrait** (`CONFIG_ESP32S3GUI_FORCE_PORTRAIT`) — stałe w `bsp_onx_pinmap.h`; touch i panel używają tych samych flag.
- UI czyta orientację w runtime przez `layout_orientation()` (rozdzielczość LVGL).

## Motywy (skórki)

Skórka to struktura `UiTheme` (`main/ui/core/ui_theme.h`) z kolorami hex; wbudowane `ui_theme_dark` i `ui_theme_light`. Cały UI pobiera kolory wyłącznie przez gettery `ui_color_*()`, więc podmiana skórki = podmiana jednego wskaźnika:

- `ui_theme_init()` — wybiera skórkę wg Kconfig (menu **esp32s3GUI Theme**) raz przy boocie (wołane w `ui_task`).
- `ui_theme_set(const UiTheme *)` — podmiana w locie (np. przyszły benchmark motywów); po zmianie należy przebudować ekran.

Nowa skórka: kolejny `const UiTheme` w `ui_theme.c` + (opcjonalnie) opcja w `Kconfig.projbuild` i gałąź w `ui_theme_init()`.

## FPS

`main/ui/core/ui_fps.c` — etykieta FPS jako **FLOATING dziecko bieżącego lv_screen** (nie `lv_layer_top()`), tworzona przy każdym ekranie przez `ui_fps_attach(scr)`. Licznik klatek rejestruje się raz w `ui_fps_init()` (display event + timer co 1 s).

## Rdzenie CPU

Dual-core (`CONFIG_FREERTOS_UNICORE=n`). Stałe w `main/core_pins.h`:

| Rdzeń | Zawartość |
|-------|-----------|
| **0** (`GUI_CORE_UI`) | `ui_task` (`xTaskCreatePinnedToCore`, prio 5) |
| **1** (`GUI_CORE_APP`) | wolny pod przyszłą logikę benchmarku — **bez LVGL API** |

Task LVGL (`esp_lvgl_port`) **NIE jest jawnie przypięty** — odtworzone 1:1 z działającego HEOStation (`task_affinity = -1`). Eksperyment „oba taski graficzne na rdzeniu 0” (potencjalny trop diagnostyki dotyku) jest udokumentowany jednolinijkowo w `bsp_onx_lvgl.c` i celowo wyłączony domyślnie. Nowe taski aplikacyjne tworzyć wyłącznie na `GUI_CORE_APP`.

## Szkielet UI

**Nawigator** (`screen_navigator.c`) — każdy z 5 ekranów to **osobny pełny lv_screen** (wzorzec jak HEOStation `screen_select_speaker_load`): nowy korzeń → treść bezpośrednio na `scr` → pasek strzałek (`ui_benchmark_chrome.c`) → FPS → `lv_screen_load` → usunięcie poprzedniego ekranu. **Bez** powłoki ze slotem, **bez** `lv_obj_clean` w wspólnym kontenerze, **bez** `lv_layer_top()` dla nawigacji/FPS.

| Ekran | Moduł | Treść |
|-------|-------|-------|
| 1 | `screen_carousel.c` + `ui_carousel.c` | karuzela wyboru głośnika (kopia 1:1 z HEOStation) |
| 2 | `screen_list.c` | klasyczna lista LVGL (`lv_list`) tych samych głośników |
| 3-5 | `screen_placeholder.c` | napis „To jest ekran nr X” |

Dane głośników: `main/ui/data/fake_speakers.c` (zmyślone nazwy, jak `HEOSTATION_FAKE_SPEAKERS`). Kliknięcie kafla/pozycji wywołuje handler akcji z `ui_kit` (bez kolejek — benchmark jednozadaniowy).

## Jak dodać nowy BSP

1. Katalog `main/bsp/<nazwa>/` (pinmap, bus, panel, touch, lvgl).
2. Publiczne API: `bsp_<nazwa>_init()`.
3. Wpis w `display_driver.c` i warunkowy `SRCS` w `main/CMakeLists.txt`.
4. Opcja w `Kconfig.projbuild`.

## Jak dodać ekran benchmarku

1. Nowy moduł `main/ui/screens/screen_<nazwa>.c` z funkcją `screen_<nazwa>_build(lv_obj_t *scr)`.
2. Wpięcie w `show_screen()` (`screen_navigator.c`) i ewentualne zwiększenie `SCREEN_COUNT`.
3. Dopisanie pliku do `UI_SOURCES` w `main/CMakeLists.txt`.
4. Kolory wyłącznie przez `ui_theme.h`, layout relatywny (`lv_pct`, flex).

## Build (użytkownik)

**Pierwszy build** (lub po zmianie targetu):

```bash
idf.py set-target esp32s3
idf.py fullclean
idf.py build
idf.py -p COM3 flash monitor
```

Jeśli flash zgłasza `This chip is ESP32-S3, not ESP32` — projekt był zbudowany dla złego targetu; wykonaj `set-target esp32s3` i przebuduj od zera.

Domyślnie: ONX3248G035, motyw dark, **tryb diagnostyczny: sama lista** (`lv_list`). Przełącznik w menu **esp32s3GUI Startup (diagnostics)**.
