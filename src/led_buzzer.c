#include "pico/stdlib.h"
#include "led_buzzer.h"

void led_buzzer_init(uint led_pin, uint buzzer_pin) {
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);
    gpio_put(led_pin, 0);

    gpio_init(buzzer_pin);
    gpio_set_dir(buzzer_pin, GPIO_OUT);
    gpio_put(buzzer_pin, 0);
}

void led_buzzer_on(uint led_pin, uint buzzer_pin) {
    gpio_put(led_pin, 1);
    gpio_put(buzzer_pin, 1);
}

void led_buzzer_off(uint led_pin, uint buzzer_pin) {
    gpio_put(led_pin, 0);
    gpio_put(buzzer_pin, 0);
}

void led_on(uint pin) {
    gpio_put(pin, 1); // Set the GPIO pin to HIGH
}

// Function to turn off the LED
void led_off(uint pin) {
    gpio_put(pin, 0); // Set the GPIO pin to LOW
}

void buzzer_on(uint pin) {
    gpio_put(pin, 1); // Set the GPIO pin to HIGH
}

// Function to turn off the buzzer
void buzzer_off(uint pin) {
    gpio_put(pin, 0); // Set the GPIO pin to LOW
}

void buzzer_on_for_duration(uint pin, uint duration_ms) {
    gpio_put(pin, 1); // Turn on buzzer
    sleep_ms(duration_ms); // Wait for the specified duration
    gpio_put(pin, 0); // Turn off buzzer
}