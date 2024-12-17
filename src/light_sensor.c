#include "light_sensor.h"
#include "pico/stdlib.h"

// Initialize the TSL2561 sensor
void tsl2561_init(i2c_inst_t *i2c) {
    uint8_t data[2];
    data[0] = TSL2561_CMD | TSL2561_CONTROL_REG;
    data[1] = TSL2561_POWER_ON; // Power on the sensor
    i2c_write_blocking(i2c, TSL2561_ADDR, data, 2, false);
}

// Read the light intensity from TSL2561
uint16_t tsl2561_read_light(i2c_inst_t *i2c) {
    uint8_t reg = TSL2561_CMD | TSL2561_DATA0_LOW;
    uint8_t data[2];

    // Request DATA0 (visible + infrared light)
    i2c_write_blocking(i2c, TSL2561_ADDR, &reg, 1, true);
    i2c_read_blocking(i2c, TSL2561_ADDR, data, 2, false);

    // Combine the two bytes into a single 16-bit value
    uint16_t light = data[1] << 8 | data[0];
    return light;
}