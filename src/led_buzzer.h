#ifndef LED_BUZZER_H
#define LED_BUZZER_H

void led_buzzer_init(uint led_pin, uint buzzer_pin);
void led_buzzer_on(uint led_pin, uint buzzer_pin);
void led_buzzer_off(uint led_pin, uint buzzer_pin);
void led_on(uint pin);
void led_off(uint pin);
void buzzer_on(uint pin);
void buzzer_off(uint pin);
void buzzer_on_for_duration(uint pin, uint duration_ms);
#endif
