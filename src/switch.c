#include "pico/stdlib.h"
#include "switch.h"

void switch_init(uint switch_pin) {
    gpio_init(switch_pin);
    gpio_set_dir(switch_pin, GPIO_IN);
    gpio_pull_down(switch_pin);
}

bool switch_pressed(uint switch_pin) {
    return gpio_get(switch_pin);
}
