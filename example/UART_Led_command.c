#include <avr/io.h>
#include "uart.h"
#include "gpio.h"

// PB5 : LED
#define LED_PORT PORT_B
#define LED_PIN  PIN_5

int main(void) {
    // Initialize LED pin as OUTPUT using custom GPIO driver
    gpio_pin_mode(LED_PORT, LED_PIN, GPIO_OUTPUT);
    gpio_pin_write(LED_PORT, LED_PIN, GPIO_LOW); // Ensure LED is OFF at boot

    // 2. Initialize USART0 peripheral at 9600 baud rate
    uart_init(9600);

    // Send banner messages to serial terminal
    uart_transmit_string("=== Serial LED Control ===\r\n");
    uart_transmit_string("Send '1' to TURN ON, '0' to TURN OFF\r\n");

    while (1) {
        // Non-blocking serial line check
        if (uart_data_available()) {
            uint8_t command = uart_receive_byte();

            if (command == '1') {
                gpio_pin_write(LED_PORT, LED_PIN, GPIO_HIGH);
                uart_transmit_string("LED: ON\r\n");
            } 
            else if (command == '0') {
                gpio_pin_write(LED_PORT, LED_PIN, GPIO_LOW);
                uart_transmit_string("LED: OFF\r\n");
            } 
            else if (command != '\r' && command != '\n') {
                // Ignore CR/LF carriage returns sent by serial monitors
                uart_transmit_string("Unknown command! Please use '1' or '0'.\r\n");
            }
        }

        // Main loop remains free for other concurrent tasks
    }
}