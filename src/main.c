#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "led_buzzer.h"
#include "infrared_sensor.h"
#include "sound_sensor.h"
#include "light_sensor.h" // Include TSL2561 header
#include "servo_motor.h"  // Include the servo control driver

#define LED_PIN 20
#define BUZZER_PIN 21
#define INFRARED_PIN 15
#define SOUND_SENSOR_PIN 26
#define SWITCH_PIN 16 // GPIO pin for the slide switch

#define LIGHT_THRESHOLD 400 // Adjust based on your environment
#define SERVO_PIN 3         // GPIO pin to control the servo motor
#define SOUND_THRESHOLD 500 // Threshold for detecting sound

// Shared state variables
bool motion_detected = false; // Tracks if motion was handled

// Task handles
TaskHandle_t ledTaskHandle;
TaskHandle_t buzzerTaskHandle;
TaskHandle_t motionTaskHandle;
TaskHandle_t switchTaskHandle;

// Mutex for shared resources
SemaphoreHandle_t resourceMutex;

// Task to manage the LED
void ledTask(void *params) {
    while (1) {
        xSemaphoreTake(resourceMutex, portMAX_DELAY);

        uint16_t light = tsl2561_read_light(i2c0);
        bool sound_detected = sound_sensor_is_triggered(SOUND_SENSOR_PIN);

        xSemaphoreGive(resourceMutex);

        // Turn on LED if light, motion, or sound is detected
        if (light > LIGHT_THRESHOLD || motion_detected || sound_detected) {
            led_on(LED_PIN);
        } else {
            led_off(LED_PIN);
        }

        vTaskDelay(pdMS_TO_TICKS(100)); // Delay to allow other tasks to run
    }
}

// Task to manage the buzzer
void buzzerTask(void *params) {
    while (1) {
        // Wait for the buzzer to be triggered by motor rotation
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Turn on the buzzer for 3 seconds
        buzzer_on(BUZZER_PIN);
        vTaskDelay(pdMS_TO_TICKS(3000));
        buzzer_off(BUZZER_PIN);
    }
}

// Task to manage the servo motor based on motion detection
void motionTask(void *params) {
    while (1) {
        xSemaphoreTake(resourceMutex, portMAX_DELAY);
        bool motion = !infrared_sensor_is_triggered(INFRARED_PIN); // Negative logic (active low)
        xSemaphoreGive(resourceMutex);

        if (motion && !motion_detected) {
            // Rotate the servo motor to larger angles
            servo_set_position(SERVO_PIN, 180); // Rotate to 180 degrees
            vTaskDelay(pdMS_TO_TICKS(1000));   // Pause for motor movement
            servo_set_position(SERVO_PIN, 0);  // Reset to 0 degrees

            motion_detected = true; // Mark motion as handled

            // Use buzzer_on_for_duration for a 3-second buzz
            buzzer_on_for_duration(BUZZER_PIN, 3000);
        } else if (!motion) {
            motion_detected = false; // Reset motion detection
        }

        vTaskDelay(pdMS_TO_TICKS(100)); // Allow other tasks to run
    }
}


// Task to monitor the slide switch
void switchTask(void *params) {
    while (1) {
        bool switch_state = gpio_get(SWITCH_PIN);

        if (!switch_state) {
            // Turn off everything if the switch is OFF
            led_off(LED_PIN);
            buzzer_off(BUZZER_PIN);
            servo_set_position(SERVO_PIN, 0); // Rest the servo
            vTaskSuspend(ledTaskHandle);
            vTaskSuspend(buzzerTaskHandle);
            vTaskSuspend(motionTaskHandle);
        } else {
            // Enable tasks if the switch is ON
            vTaskResume(ledTaskHandle);
            vTaskResume(buzzerTaskHandle);
            vTaskResume(motionTaskHandle);
        }

        vTaskDelay(pdMS_TO_TICKS(50)); // Small delay for stability
    }
}

int main() {
    stdio_init_all();

    // Initialize components
    led_buzzer_init(LED_PIN, BUZZER_PIN);
    infrared_sensor_init(INFRARED_PIN);
    sound_sensor_init(SOUND_SENSOR_PIN);

    // Initialize the TSL2561 light sensor
    i2c_init(i2c0, 100 * 1000);       // I2C0 at 100kHz
    gpio_set_function(0, GPIO_FUNC_I2C); // SDA on GP0
    gpio_set_function(1, GPIO_FUNC_I2C); // SCL on GP1
    gpio_pull_up(0);                  // Enable pull-ups
    gpio_pull_up(1);
    tsl2561_init(i2c0);               // Power on the TSL2561 sensor

    // Initialize the slide switch with an internal pull-down resistor
    gpio_init(SWITCH_PIN);
    gpio_set_dir(SWITCH_PIN, GPIO_IN);
    gpio_pull_down(SWITCH_PIN);

    // Initialize the servo motor
    servo_enable(SERVO_PIN); // Enable PWM on the servo pin

    // Create a mutex for shared resource access
    resourceMutex = xSemaphoreCreateMutex();

    // Create tasks
    xTaskCreate(ledTask, "LED Task", 256, NULL, 1, &ledTaskHandle);
    xTaskCreate(buzzerTask, "Buzzer Task", 256, NULL, 1, &buzzerTaskHandle);
    xTaskCreate(motionTask, "Motion Task", 256, NULL, 1, &motionTaskHandle);
    xTaskCreate(switchTask, "Switch Task", 256, NULL, 1, &switchTaskHandle);

    // Start the FreeRTOS scheduler
    vTaskStartScheduler();

    // Should never reach here
    while (1)
        ;

    return 0;
}
