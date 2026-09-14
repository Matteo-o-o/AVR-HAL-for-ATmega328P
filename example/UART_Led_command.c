#include <avr/io.h>
#include "uart.h"
#include "gpio.h"

// PB5: Pin 5 on PORT B
#define LED_PORT GPIO_PORTB
#define LED_PIN  GPIO_PIN5

int main(void) {
    // Initialize PB5 pin as Output
    gpio_init(LED_PORT, LED_PIN, GPIO_MODE_OUTPUT);
    gpio_write(LED_PORT, LED_PIN, GPIO_STATE_LOW); // Ensure LED is OFF at boot

    // Initialize USART peripheral at 9600 baud
    uart_init(9600);

    // Send banner messages to serial terminal
    uart_transmission_string("=== Serial LED Control ===\r\n");
    uart_transmission_string("Send '1' to TURN ON, '0' to TURN OFF\r\n");

    while (1) {
        // Non-blocking serial line check
        if (uart_data_available()) {
            uint8_t command = uart_receive_byte();

            if (command == '1') {
                gpio_set_high(LED_PORT, LED_PIN);
                uart_transmission_string("LED: ON\r\n");
            } 
            else if (command == '0') {
                gpio_set_low(LED_PORT, LED_PIN);
                uart_transmission_string("LED: OFF\r\n");
            } 
            else if (command != '\r' && command != '\n') {
                // Ignore CR/LF carriage returns sent by serial monitors
                uart_transmission_string("Unknown command! Please use '1' or '0'.\r\n");
            }
        }

        // Main loop remains unblocked for concurrent execution
    }

    return 0;
}