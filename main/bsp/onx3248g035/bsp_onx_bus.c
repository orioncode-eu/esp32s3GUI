/**
 * @file bsp_onx_bus.c
 * @brief esp32s3GUI – I2C master bus dla ONX3248G035.
 */

#include "bsp_onx_bus.h"
#include "bsp_onx_pinmap.h"
#include "esp_check.h"
#include "esp_log.h"

static const char *TAG = "bsp_onx_bus";

static i2c_master_bus_handle_t s_i2c_bus = NULL;

esp_err_t bsp_onx_bus_init(void)
{
    if (s_i2c_bus != NULL) {
        return ESP_OK;   /* już zainicjalizowane */
    }

    const i2c_master_bus_config_t cfg = {
        .clk_source                   = I2C_CLK_SRC_DEFAULT,
        .i2c_port                     = BSP_ONX_I2C_PORT,
        .scl_io_num                   = BSP_ONX_I2C_SCL,
        .sda_io_num                   = BSP_ONX_I2C_SDA,
        .glitch_ignore_cnt            = 7,
        .flags.enable_internal_pullup = true,
    };
    ESP_RETURN_ON_ERROR(i2c_new_master_bus(&cfg, &s_i2c_bus),
                        TAG, "I2C bus init failed");
    ESP_LOGI(TAG, "I2C bus initialized (port=%d, SCL=%d, SDA=%d)",
             BSP_ONX_I2C_PORT, BSP_ONX_I2C_SCL, BSP_ONX_I2C_SDA);
    return ESP_OK;
}

i2c_master_bus_handle_t bsp_onx_bus_i2c_handle(void)
{
    return s_i2c_bus;
}
