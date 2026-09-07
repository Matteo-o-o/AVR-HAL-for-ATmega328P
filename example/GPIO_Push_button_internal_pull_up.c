#include "gpio.h"

// PB 5 : Pin 5 on port B (PIN 19)
#define LED_PORT    GPIO_PORTB
#define LED_PIN     GPIO_PIN5

// PD 2 : Pin 2 on port D (PIN 4)
#define BUTTON_PORT GPIO_PORTD
#define BUTTON_PIN  GPIO_PIN2

int main(void) {
    // Configure PB5 as an Output pin
    gpio_init(LED_PORT, LED_PIN, GPIO_MODE_OUTPUT);

    // Configure PD2 as an Input pin with internal pull up on
    gpio_init(BUTTON_PORT, BUTTON_PIN, GPIO_MODE_INPUT_PULLUP);

    while (1) {
        // if the button is pressed, PD2 goes to GND
        if (gpio_read(BUTTON_PORT, BUTTON_PIN) == GPIO_STATE_LOW) {
            gpio_write(LED_PORT, LED_PIN, GPIO_STATE_HIGH); // LED ON
        } else {
            gpio_write(LED_PORT, LED_PIN, GPIO_STATE_LOW);  // LED OFF
        }
    }

    return 0;
}