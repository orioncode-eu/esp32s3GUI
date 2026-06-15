#ifndef ESP_LCD_ST7796U_H
#define ESP_LCD_ST7796U_H

#include "esp_lcd_panel_vendor.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief LCD panel initialization commands.
 */
typedef struct {
    int         cmd;          /*!< The specific LCD command */
    const void *data;         /*!< Buffer that holds the command specific data */
    size_t      data_bytes;   /*!< Size of `data` in memory, in bytes */
    unsigned int delay_ms;    /*!< Delay in milliseconds after this command */
} st7796u_lcd_init_cmd_t;

/**
 * @brief LCD panel vendor configuration.
 *
 * @note This structure needs to be passed to the `vendor_config` field in
 *       `esp_lcd_panel_dev_config_t`.
 */
typedef struct {
    const st7796u_lcd_init_cmd_t *init_cmds;      /*!< Pointer to init commands array.
                                                        Set to NULL to use defaults. */
    uint16_t init_cmds_size;                       /*!< Number of commands in array */
} st7796u_vendor_config_t;

/**
 * @brief Create LCD panel for model ST7796U.
 *
 * @param[in]  io              LCD panel IO handle
 * @param[in]  panel_dev_config General panel device configuration
 * @param[out] ret_panel       Returned LCD panel handle
 * @return
 *   - ESP_ERR_INVALID_ARG  if parameter is invalid
 *   - ESP_ERR_NO_MEM       if out of memory
 *   - ESP_OK               on success
 */
esp_err_t esp_lcd_new_panel_st7796u(const esp_lcd_panel_io_handle_t io,
                                    const esp_lcd_panel_dev_config_t *panel_dev_config,
                                    esp_lcd_panel_handle_t *ret_panel);

/**
 * @brief SPI bus configuration helper macro.
 *
 * @param sclk        SPI clock pin
 * @param mosi        SPI MOSI pin
 * @param max_trans_sz Maximum transfer size in bytes
 */
#define ST7796U_PANEL_BUS_SPI_CONFIG(sclk, mosi, max_trans_sz) \
    {                                                           \
        .sclk_io_num     = sclk,                               \
        .mosi_io_num     = mosi,                               \
        .miso_io_num     = -1,                                 \
        .quadhd_io_num   = -1,                                 \
        .quadwp_io_num   = -1,                                 \
        .max_transfer_sz = max_trans_sz,                       \
    }

/**
 * @brief SPI panel IO configuration helper macro.
 *
 * @param pclk       Pixel clock (Hz)
 * @param cs         SPI chip select pin
 * @param dc         SPI data/command pin
 * @param callback   Transfer-done callback (NULL if unused)
 * @param callback_ctx Callback context
 */
#define ST7796U_PANEL_IO_SPI_CONFIG(pclk, cs, dc, callback, callback_ctx) \
    {                                                                       \
        .cs_gpio_num         = cs,                                          \
        .dc_gpio_num         = dc,                                          \
        .spi_mode            = 0,                                           \
        .pclk_hz             = pclk,                                        \
        .trans_queue_depth   = 5,                                           \
        .on_color_trans_done = callback,                                    \
        .user_ctx            = callback_ctx,                                \
        .lcd_cmd_bits        = 8,                                           \
        .lcd_param_bits      = 8,                                           \
    }

#ifdef __cplusplus
}
#endif

#endif /* ESP_LCD_ST7796U_H */
