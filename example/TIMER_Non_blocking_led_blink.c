#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"
#include "timer.h"

// PB 5 : Pin 5 on port B (PIN 19)
#define LED_PORT GPIO_PORTB
#define LED_PIN  GPIO_PIN5

int main(void) {
    // Initialize system tick timer (Timer1 CTC, 1 ms tick)
    timer1_init_system_tick();

    // Initialize PB5 as Output
    gpio_init(LED_PORT, LED_PIN, GPIO_MODE_OUTPUT);

    uint32_t last_toggle_time = timer_millis();

    while (1) {
        // Non-blocking check every 500 ms (500 ms HIGH + 500 ms LOW = 1 Hz cycle)
        if (timer_has_elapsed(last_toggle_time, 500)) {
            gpio_toggle(LED_PORT, LED_PIN);
            last_toggle_time = timer_millis();
        }

        // Main loop remains unblocked to run other task logic here
    }

    return 0;
}