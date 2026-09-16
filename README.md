# AVR-HAL for ATmega328P

A bare-metal **Hardware Abstraction Layer (HAL)** written in C for the ATmega328P microcontroller. Designed for high performance, low footprint and modularity.


## Table of Contents

- [Overview](#overview)
- [Project Structure](#project-structure)
- [Prerequisites & Development Environment](#prerequisites--development-environment)
  - [Fuse Configuration (Clock Prescaler)](#fuse-configuration-clock-prescaler)
- [Quick Start](#quick-start)
- [Usage Example](#usage-example)
- [Driver API Reference](#driver-api-reference)
  - [GPIO Driver](#gpio-driver-gpioh)
  - [UART Driver](#uart-driver-uarth)
  - [Timer Driver](#timer-driver-timerh)
  - [ADC Driver](#adc-driver-adch)
  - [External Interrupt Driver](#external-interrupt-driver-ext_interrupth)



## Overview

This project provides a clean, driver-level API for the ATmega328P peripherals, abstracting direct register operations (`DDRx`, `PORTx`, `UCSR0x`, etc.) into reliable, well-documented functions, without sacrificing performance or adding unnecessary CPU overhead.

It targets both bare ATmega328P chips and Arduino Uno boards with ATmega328P, and is well suited for embedded projects that need direct hardware control, an event-driven interrupt model, and a predictable memory footprint.

| Peripheral / Driver | Description |
| :--- | :--- |
| **GPIO** | Digital I/O configuration, pin write, read, pull-up, and toggling |
| **UART** | Asynchronous serial comms (8N1), stdio redirection (`printf`) |
| **TIMERS** | Timer1 CTC system tick, Timer0 Fast PWM (fade/frequency generation) |
| **ADC** | 10-bit Analog-to-Digital conversion with integer mV scaling |
| **EXT_INTERRUPT** | Dedicated interrupts (`INT0`/`INT1`) & Pin Change Interrupts (`PCINT0-2`) |

## Project Structure

```text
.
├── components/
│   ├── adc/
│   │   ├── adc.c
│   │   └── include/adc.h
│   ├── ext_interrupt/
│   │   ├── ext_interrupt.c
│   │   └── include/ext_interrupt.h
│   ├── gpio/
│   │   ├── gpio.c
│   │   └── include/gpio.h
│   ├── timer/
│   │   ├── timer.c
│   │   └── include/timer.h
│   └── uart/
│       ├── uart.c
│       └── include/uart.h
├── example/                         # Standalone usage examples per driver
│   ├── ADC_Read_potentiometer/
│   ├── EXT_INTERRUPT_Hardware_event.c
│   ├── GPIO_Led_blinking.c
│   ├── GPIO_Push_button_internal_pull_up.c
│   ├── TIMER_Led_fade_with_PWM.c
│   ├── TIMER_Non_blocking_led_blink.c
│   ├── UART_Led_command.c
│   └── README.md
├── main/
│   └── main.c                       # Application entry point
├── Makefile                         # Build, flash, and clean targets
└── README.md
```

Each peripheral driver lives in its own `components/<driver>/` folder with a public header under `include/`, keeping the public API separate from the implementation and making it easy to drop a single driver into another project.

## Prerequisites & Development Environment

### DevContainer (Recommended)

This repository includes a pre-configured **VS Code DevContainer** with the entire AVR GNU toolchain pre-installed (`avr-gcc`, `avr-libc`, `avrdude`, `make`). Opening this workspace in VS Code with the *Dev Containers* extension gives you an instant, zero-setup build environment.

### Toolchain & Hardware Needed

| Requirement | Details |
| :--- | :--- |
| **Compiler** | `avr-gcc` |
| **Flashing Software** | `avrdude` |
| **Programmer** | USBasp |
| **Target MCU** | ATmega328P |
| **Telemetry** | USB-to-TTL serial adapter |

### Fuse Configuration (Clock Prescaler)

By default, a factory-fresh ATmega328P has the **CKDIV8** fuse programmed, which divides the internal 8 MHz RC oscillator by 8, giving an effective `F_CPU` of **1 MHz**. This project is built for the full oscillator speed (`F_CPU = 8217600UL`, calibrated for accurate UART timing), if CKDIV8 is left enabled, every timer tick, PWM frequency, and UART baud rate will be **off by a factor of 8**.

Before flashing for the first time, check the current fuse values:

```bash
avrdude -c usbasp -p atmega328p -U lfuse:r:-:h -U hfuse:r:-:h -U efuse:r:-:h
```

If `lfuse` reads `0x62` (factory default), CKDIV8 is active. Disable it while keeping the internal 8 MHz oscillator as the clock source:

```bash
avrdude -c usbasp -p atmega328p -U lfuse:w:0xE2:m
```

## Quick Start

### 1. Build the Firmware

Compile all HAL drivers and the target application:

```bash
make
```

### 2. Flash to ATmega328P

Upload the generated `.hex` binary via USBasp:

```bash
make flash
```

### 3. Clean Build Artifacts

```bash
make clean
```

## Usage Example

A minimal example that blinks an LED on pin PB5 and prints a heartbeat message over UART:

```c
#include <avr/interrupt.h>
#include <stdio.h>

#include "gpio.h"
#include "uart.h"
#include "timer.h"

int main(void) {
    // Initialize HAL peripherals
    uart_init(9600);
    gpio_init(GPIO_PORTB, GPIO_PIN5, GPIO_MODE_OUTPUT);
    timer1_init_ctc();

    // Enable global interrupts (required for Timer1 tick)
    sei();

    printf("\r\n--- HAL Demo ---\r\n");

    uint32_t last_tick = 0;

    while (1) {
        // Non-blocking time acquisition using timer1_get_ticks()
        uint32_t current_tick = timer1_get_ticks();

        // Periodic execution every 500 ms without blocking the CPU
        if (current_tick - last_tick >= 500) {
            last_tick = current_tick;

            // Toggle status LED on PB5
            gpio_toggle(GPIO_PORTB, GPIO_PIN5);

            // Output telemetry via UART printf redirection
            printf("Tick: %lu ms\r\n", current_tick);
        }
    }

    return 0;
}
```

More driver-specific examples (ADC, external interrupts, PWM fading, non-blocking timers, UART command handling) are available in [`example/`](./example), with README.

## Driver API Reference

### GPIO Driver (`gpio.h`)

Controls digital input/output pins: configuring their direction, driving them high or low, toggling their state, and reading external signals (buttons, switches, digital sensors).

**Configuration & Control**

```c
// Initialize pin direction and pull-up state
void gpio_init(gpio_port_t port, gpio_pin_t pin, gpio_mode_t mode);

// Output management
void gpio_write(gpio_port_t port, gpio_pin_t pin, gpio_state_t state);
void gpio_set_high(gpio_port_t port, gpio_pin_t pin);
void gpio_set_low(gpio_port_t port, gpio_pin_t pin);
void gpio_toggle(gpio_port_t port, gpio_pin_t pin);

// Input management
gpio_state_t gpio_read(gpio_port_t port, gpio_pin_t pin);
```

**Parameters**

- `port`: pin group (`GPIO_PORTB`, `GPIO_PORTC`, `GPIO_PORTD`)
- `pin`: pin number within that port (0 to 7)
- `mode`: `GPIO_MODE_OUTPUT`, `GPIO_MODE_INPUT`, or `GPIO_MODE_INPUT_PULLUP`
- `state`: `GPIO_LOW` or `GPIO_HIGH`

### UART Driver (`uart.h`)

Handles asynchronous serial communication (8N1) with a computer or another device, sending debug output, receiving commands, and, via stdio redirection, letting the rest of the codebase use `printf`/`scanf` directly.

**Serial Initialization & I/O**

```c
// Initialize UART with specified baud rate (8N1 frame format)
void uart_init(uint32_t baud);

// Character transmission and reception
void uart_transmit(char data);
char uart_receive(void);

// Non-blocking availability check
bool uart_is_data_available(void);
```

**Parameters**

- `baud`: serial speed in bauds (e.g. `9600`, `115200`)
- `data`: byte to transmit / received byte

> **Note:** `uart_init()` automatically redirects standard stdout/stdin streams, enabling native usage of `printf()` and `scanf()`.

### Timer Driver (`timer.h`)

Provides time-based features: a Timer1-based millisecond tick for non-blocking delays and scheduling, and a Timer0 Fast PWM output for dimming LEDs, driving motors, or generating variable frequencies.

**System Tick & PWM**

```c
// Initialize Timer1 in CTC mode for periodic millisecond interrupts
void timer1_init_ctc(void);

// Non-blocking time utility
uint32_t timer1_get_ticks(void);

// Fast PWM configuration on Timer0 Channel A (PD6)
void timer0_pwm_init(void);
void timer0_pwm_set_duty_cycle(uint8_t duty);
```

**Parameters**

- `duty`: PWM duty cycle, from `0` (always low) to `255` (always high)

### ADC Driver (`adc.h`)

Reads analog voltages (potentiometers, analog sensors) through the 10-bit Analog-to-Digital Converter and converts the raw reading into a real-world millivolt value.

**Analog Conversion**

```c
// Initialize ADC hardware (AVCC reference, Prescaler 128)
void adc_init(void);

// Read 10-bit raw value from channel (0 to 7)
uint16_t adc_read(uint8_t channel);

// Convert raw 10-bit value to millivolts (0 to 5000 mV)
uint16_t adc_to_mv(uint16_t raw_value);
```

**Parameters**

- `channel`: analog pin number (0 to 7, corresponds to `ADC0`–`ADC7`)
- `raw_value`: raw 10-bit value (0–1023) returned by `adc_read()`

### External Interrupt Driver (`ext_interrupt.h`)

Lets the MCU react to external events without polling: dedicated hardware interrupts (`INT0`/`INT1`) for time-critical signals, and Pin Change Interrupts (PCINT) for detecting a level change on almost any pin.

**Dedicated Interrupts (INT0 / INT1)**

```c
// Enable dedicated external interrupt with trigger mode
void ext_int_enable(ext_int_t interrupt, ext_int_mode_t mode);

// Disable dedicated external interrupt
void ext_int_disable(ext_int_t interrupt);
```

**Parameters**

- `interrupt`: `EXT_INT0` (PB2) or `EXT_INT1` (PB3)
- `mode`: trigger condition, rising edge, falling edge, any change, or low level

**Pin Change Interrupts (PCINT)**

```c
// Enable/disable PCINT on a specific physical pin (0 to 23, excluding 15)
void pcint_enable_pin(uint8_t pcint_pin);
void pcint_disable_pin(uint8_t pcint_pin);

// Enable/disable PCINT for an entire port group (PORTB, PORTC, PORTD)
void pcint_enable_port(pcint_port_t port, bool enable);
```

**Parameters**

- `pcint_pin`: global PCINT pin number (0 to 23, 15 excluded, not wired on this chip)
- `port`: pin group (`PCINT_PORTB`, `PCINT_PORTC`, `PCINT_PORTD`)
- `enable`: `true` to enable, `false` to disable