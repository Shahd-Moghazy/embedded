#ifndef SWITCH_H
#define SWITCH_H

#include <stdbool.h>

void switch_init(uint switch_pin);
bool switch_pressed(uint switch_pin);

#endif
