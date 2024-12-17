#ifndef SOUND_SENSOR_H
#define SOUND_SENSOR_H

#include <stdbool.h>

void sound_sensor_init(uint sound_sensor_pin);
bool sound_sensor_detected(uint sound_sensor_pin);
bool sound_sensor_is_triggered(uint pin);
#endif
