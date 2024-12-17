#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

#include "hardware/i2c.h"

#define TSL2561_ADDR 0x39 // Default I2C address for TSL2561
#define TSL2561_CMD  0x80
#define TSL2561_POWER_ON 0x03
#define TSL2561_POWER_OFF 0x00
#define TSL2561_CONTROL_REG 0x00
#define TSL2561_DATA0_LOW 0x0C
#define TSL2561_DATA1_LOW 0x0E

void tsl2561_init(i2c_inst_t *i2c);
uint16_t tsl2561_read_light(i2c_inst_t *i2c);

#endif