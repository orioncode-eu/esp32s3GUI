#include "exio_pcf8574.h"
#include "esp_log.h"

static const char *TAG = "pcf8574";

/**
 * Shadow register: tracks current output state of all 8 I/O pins.
 * Initialized to 0xFF (all high = inputs with pull-up, PCF8574 default).
 */
static volatile uint8_t s_io_state = 0xFF;

/** I2C device handle for PCF8574 */
static i2c_master_dev_handle_t s_dev_handle = NULL;

esp_err_t exio_pcf8574_init(void)
{
    i2c_master_bus_handle_t bus_handle;

    /* Retrieve bus handle for I2C_NUM_0 (must be already initialized) */
    ESP_RETURN_ON_ERROR(
        i2c_master_get_bus_handle(0, &bus_handle),
        TAG, "Failed to get I2C bus 0 handle – call i2c_new_master_bus() first");

    if (bus_handle == NULL) {
        ESP_LOGE(TAG, "I2C bus handle is NULL");
        return ESP_FAIL;
    }

    const i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .scl_speed_hz    = PCF8574_IIC_SPEED,
        .device_address  = PCF8574_ADDR,
    };
    ESP_RETURN_ON_ERROR(
        i2c_master_bus_add_device(bus_handle, &dev_cfg, &s_dev_handle),
        TAG, "Failed to add PCF8574 to I2C bus");

    if (s_dev_handle == NULL) {
        ESP_LOGE(TAG, "PCF8574 device handle is NULL");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "PCF8574 initialized at 0x%02X", PCF8574_ADDR);
    return ESP_OK;
}

esp_err_t exio_pcf8574_pin_read(exio_pcf8574_pin pin, exio_pcf8574_level *level)
{
    uint8_t read_state = 0;
    uint8_t bit_mask   = (uint8_t)(0x01 << (pin - 1));

    esp_err_t ret = i2c_master_receive(s_dev_handle, &read_state, 1, PCF8574_RW_MAX_TIMEOUT);
    if (ret == ESP_OK) {
        *level = (read_state & bit_mask) ? EXIO_PCF8574_LEVEL_H : EXIO_PCF8574_LEVEL_L;
    }
    return ret;
}

esp_err_t exio_pcf8574_pin_write(exio_pcf8574_pin pin, exio_pcf8574_level level)
{
    uint8_t bit_mask = (uint8_t)(0x01 << (pin - 1));

    if (level == EXIO_PCF8574_LEVEL_H) {
        s_io_state |=  bit_mask;
    } else {
        s_io_state &= ~bit_mask;
    }

    uint8_t cmd[1] = {s_io_state};
    return i2c_master_transmit(s_dev_handle, cmd, 1, PCF8574_RW_MAX_TIMEOUT);
}
