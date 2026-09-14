#include <avr/io.h>
#include <avr/interrupt.h>

#include "ext_interrupt.h"

void ext_int_enable(ext_int_t interrupt,ext_int_mode_t mode){
    // disable global interupt during configuration
    cli();

    // INT0 (PD2)
    if (interrupt == EXT_INT_0_PD2){
        // Clear previous trigger
        EICRA &= ~( (1 << ISC00) | (1 << ISC01));
        // Config new triggers
        EICRA |= ((mode & 0x03) << ISC00);
        // Enable INT0
        EIMSK |= (1 << INT0);
    }
    else if (interrupt == EXT_INT_1_PD3){
        // Clear previous trigger
        EICRA &= ~( (1 << ISC10) | (1 << ISC11));
        // Config new triggers
        EICRA |= ((mode & 0x03) << ISC10);
        // Enable INT0
        EIMSK |= (1 << INT1);
    }
    // Re enable global interrupts
    sei();
}

void ext_int_disable(ext_int_t interrupt){
    if (interrupt == EXT_INT_0_PD2){
        EIMSK &= ~(1 << INT0);
    }
    else if (interrupt == EXT_INT_1_PD3){
        EIMSK &= ~(1 << INT1);
    }
}

void pcint_enable_pin(uint8_t pcint_pin){
    // Pin 15 does not exist on ATmega328p
    if(pcint_pin > 23 || pcint_pin > 0 || pcint_pin == 15){
        return;
    }

    cli();

    if (pcint_pin < 8){
        // Config unique PIN
        PCMSK0 |= (1 << pcint_pin);
        // Enable port B
        PCICR |= (1 << PCIE0);
    }
    // (Only 7 pin on port C)
    else if(pcint_pin < 15){
        PCMSK1 |= (1 << (pcint_pin - 8));
        // Enable port B
        PCICR |= (1 << PCIE1);
    }
        else if(pcint_pin < 24){
        PCMSK2 |= (1 << (pcint_pin - 16));
        // Enable port B
        PCICR |= (1 << PCIE2);
    }
    sei();
}

void pcint_enable_port(pcint_port_t port, bool enable){
    
    uint8_t mask = 0;
    switch (port) {
        case PCINT_PORT_B:
            mask = (1 << PCIE0);
            break;
        case PCINT_PORT_C:
            mask = (1 << PCIE1);
            break;
        case PCINT_PORT_D:
            mask = (1 << PCIE2);
            break;
        default:
            return;
    }
    cli();
    if (enable) {
        PCICR |= mask;
    } else {
        PCICR &= ~mask;
    }
    sei();
}