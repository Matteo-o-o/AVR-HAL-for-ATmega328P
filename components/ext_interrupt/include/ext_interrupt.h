#ifndef EXT_INTERRUPT_H
#define EXT_INTERRUPT_H

#include <stdbool.h>
#include <stdint.h>

// config External Interrupt Trigger Modes (Register : EICRA)
typedef enum{
    EXT_INT_MODE_LOW_LEVEL = 0,
    EXT_INT_MODE_ANY_CHANGE,
    EXT_INT_MODE_FALLING_EDGE,
    EXT_INT_MODE_RISING_EDGE
} ext_int_mode_t;

// Hardware External Interrupt channel
typedef enum{
    EXT_INT_0_PD2 = 0,
    EXT_INT_1_PD3
} ext_int_t;

// Pin Change Interrupt Port 
typedef enum{
    PCINT_PORT_B = 0,   // PCINT[7:0]   -> PORTB (PB0 to PB7)
    PCINT_PORT_C,       // PCINT[14:8]  -> PORTC (PC0 to PC6) (Only 7 pin on port C)
    PCINT_PORT_D        // PCINT[23:16] -> PORTD (PD0 to PD7)
} pcint_port_t;

void ext_int_enable(ext_int_t interrupt,ext_int_mode_t mode);

void ext_int_disable(ext_int_t interrupt);

// Enables Pin Change Interrupt on a specific PCINT pin index (0 to 23), pcint_pin Index of the PCINT pin ( 0 for PCINT0/PB0, 18 for PCINT18/PD2)
void pcint_enable_pin(uint8_t pcint_pin);

void pcint_disable_pin(uint8_t pcint_pin);

// Enables or disables an entire Pin Change Interrupt Port group.
void pcint_enable_port(pcint_port_t port, bool enable);

#endif