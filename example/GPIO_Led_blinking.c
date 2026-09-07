#include <util/delay.h>
#include "gpio.h"

// PB 5 : Pin 5 on port B (PIN 19)
#define LED_PORT GPIO_PORTB
#define LED_PIN  GPIO_PIN5

int main(void) {
    // Configure PB5 as an Output pin
    gpio_init(LED_PORT, LED_PIN, GPIO_MODE_OUTPUT);

    while (1) {
        // Toggle PB5
        gpio_toggle(LED_PORT, LED_PIN);

        // delay (500ms)
        _delay_ms(500);
    }

    return 0;
}