#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>

#include "gpio.h"
#include "ext_interrupt.h"
#include "uart.h"

// Volatile flag bridging the hardware ISR event to the main execution loop
static volatile bool g_button_events_pending = false;


// Interrupt Service Routine for INT0 (mapped to physical pin PD2). Triggers automatically on the falling edge of the signal (button press).
ISR(INT0_vect) {
    // short delay to filter out mechanical contact bounce 
    _delay_ms(20);

    // Verify if the pin is still pulled LOW
    if (gpio_read(GPIO_PORTD, GPIO_PIN2) == GPIO_STATE_LOW) {
        g_button_events_pending = true;
    }
}

int main(void) {
    //Initialize UART baud rate at 9600 bps for debugging
    uart_init(9600);

    // Configure PB5 (Status LED) as output and ensure it starts LOW
    gpio_init(GPIO_PORTB, GPIO_PIN5, GPIO_MODE_OUTPUT);
    gpio_set_low(GPIO_PORTB, GPIO_PIN5);

    // Configure PD2 (INT0) as input with internal pull-up resistor
    gpio_init(GPIO_PORTD, GPIO_PIN2, GPIO_MODE_INPUT_PULLUP);

    // Configure External Interrupt HAL for INT0 on Falling Edge
    ext_int_enable(EXT_INT_0_PD2, EXT_INT_MODE_FALLING_EDGE);

    printf("\r\n==============================================\r\n");
    printf("   AVR HAL : External Interrupts   \r\n");
    printf("==============================================\r\n");
    printf("Status: System initialized and listening on INT0 (PD2)...\r\n\r\n");

    uint32_t press_count = 0;

    while (1) {
        // Asynchronous event processing dispatched from ISR
        if (g_button_events_pending) {
            // Clear the flag
            g_button_events_pending = false;

            // Toggle the status LED on PB5
            gpio_toggle(GPIO_PORTB, GPIO_PIN5);

            press_count++;
            gpio_state_t led_state = gpio_read(GPIO_PORTB, GPIO_PIN5);

            printf("[EVENT #%lu] Hardware INT0 triggered! LED state: %s\r\n",
                   press_count,
                   (led_state == GPIO_STATE_HIGH) ? "ON" : "OFF");
        }
    }

    return 0;
}