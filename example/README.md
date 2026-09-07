# Test AVR-HAL-for-ATmega328P

This directory contains unit tests and hardware verification setups for the AVR-HAL ATmega328P drivers.


## Setup

### USBasp Wiring

Connect the **USBasp** programmer to the ATmega328P ISP pins using the following pinout:

| USBasp Pin | Signal | → | ATmega328P Pin | Function |
|:----------:|:------:|:-:|:---------------|:---------|
| 1 | MOSI | → | Pin 17 | MOSI |
| 2 | VCC | → | - | 5V |
| 3 | NC | - | - | *(not connected)* |
| 4 | GND | → | Pin 8, 22 | GND |
| 5 | RST | → | Pin 1 | RESET |
| 6 | GND | → | Pin 8, 22 | GND |
| 7 | SCK | → | Pin 19 | SCK |
| 8 | GND | → | Pin 8, 22 | GND |
| 9 | MISO | → | Pin 18 | MISO |
| 10 | GND | → | Pin 8, 22 | GND |

Note: Other devices can be used to flash (e.g., Arduino as ISP or AVRISP mkII). You will need to adapt the `PROGRAMMER` variable in the `Makefile`.

### USB to TTL

This is used to drive UART/USART serial communication and debugging tests.

Working progress

## GPIO

### LED Blinking

- Pin: `PB5` (PIN 19)
- Wiring:
   - Connect the anode of the LED to PB5 through a 220Ω resistor.
   - Connect the cathode directly to GND.
- Behavior: 500ms LED toggle

### Push-Button Input with Internal Pull-Up

- Pins: `PD2`(PIN 4)(Input) and `PB5`(PIN 19)(Output)
- Wiring:
   - Setup the LED like in LED Blinking
   - Connect one terminal of a push-button to PD2 and the other derectly to the GND.
- Behavior: Pressing the button pulls `PD2` to ground and turns ON the LED on `PB5`.

## TIMER

### Non-Blocking 1 Hz LED Blink

- Pin: `PB5` (PIN 19)
- Wiring:
   - Setup the LED like in LED Blinking
- Behavior: Uses Timer1 CTC interrupt tick (1 ms resolution) to toggle PB5 every 500 ms without blocking CPU execution.

### LED Fade with Timer0 Fast PWM

- Pin: `PD6` (PIN 12)
- Wiring :
   - Connect LED anode to PD6 via a 220Ω resistor, connect cathode to GND.
- Behavior: Generates an 8-bit Fast PWM signal on Timer0 Channel A. Modulates the duty cycle non-blockingly from 0 to 255 and back every 5 ms per step for a smooth fade effect.
