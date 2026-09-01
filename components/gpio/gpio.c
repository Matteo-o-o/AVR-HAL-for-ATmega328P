#include "gpio.h"

// register struct (link port with hardware material)
typedef struct{
    volatile uint8_t *ddr;
    volatile uint8_t *port;
    volatile uint8_t *pin;
}gpio_register_t;

// Mapping registers ATmega328P
static const gpio_register_t GPIO_BUS[] = {
    [GPIO_PORTB] = { &DDRB, &PORTB, &PINB },
    [GPIO_PORTC] = { &DDRC, &PORTC, &PINC },
    [GPIO_PORTD] = { &DDRD, &PORTD, &PIND }
};

void gpio_init(gpio_port_t port,gpio_pin_t pin,gpio_mode_t mode){
    gpio_register_t *bus = &GPIO_BUS[port];

    switch (mode)
    {
    case GPIO_MODE_INPUT:
        // Bit DDRx 0 => entry , bit PORT 0 => floating
        *(bus->ddr) &= ~(1<<pin);
        *(bus->port) &= ~(1<<pin);
        break;
    case GPIO_MODE_OUTPUT:
        // Bit DDRx 1 => output
        *(bus->ddr) |= (1<<pin);
        break;
    case GPIO_MODE_INPUT_PULLUP:
        // Bit DDRx 0 => entry , bit PORT 1 => Intern Pull Up
        *(bus->ddr) &= ~(1<<pin);
        *(bus->port) |= (1<pin);
        break;
    }
}
