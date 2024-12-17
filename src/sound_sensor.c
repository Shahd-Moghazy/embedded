#include "pico/stdlib.h"
#include "sound_sensor.h"

void sound_sensor_init(uint sound_sensor_pin) {
    gpio_init(sound_sensor_pin);
    gpio_set_dir(sound_sensor_pin, GPIO_IN);
    gpio_pull_down(sound_sensor_pin);
}

bool sound_sensor_detected(uint sound_sensor_pin) {
    return gpio_get(sound_sensor_pin);
}

// Function to check if the sound sensor is triggered
bool sound_sensor_is_triggered(uint pin) {
    return gpio_get(pin); // Read the GPIO pin value
}