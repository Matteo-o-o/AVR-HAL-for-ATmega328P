#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"
#include "timer.h"

// PD6 : PWM channel A (registr : OC0A)
#define PWM_PORT GPIO_PORTD
#define PWM_PIN  GPIO_PIN6  

int main(void) {
    // Initialize system tick for timing the fade updates
    timer1_init_system_tick();

    // Configure PD6 (OC0A) as Output
    gpio_init(PWM_PORT, PWM_PIN, GPIO_MODE_OUTPUT);

    // Initialize Timer0 8-bit Fast PWM
    timer0_pwm_init();

    uint32_t last_step_time = timer_millis();
    uint8_t duty_cycle = 0;
    int8_t fade_direction = 1; // +1 = Increasing brightness, -1 = Decreasing brightness

    while (1) {
        // Update duty cycle every 5 ms for a smooth breathing effect
        if (timer_has_elapsed(last_step_time, 5)) {
            duty_cycle += fade_direction;

            // Change direction at duty cycle boundaries (0 and 255)
            if (duty_cycle == 255 || duty_cycle == 0) {
                fade_direction = -fade_direction;
            }

            // Apply duty cycle to Timer0 Channel A (PD6 / OC0A)
            timer0_pwm_set_duty(TIMER0_PWM_CHANNEL_A, duty_cycle);

            last_step_time = timer_millis();
        }
    }

    return 0;
}