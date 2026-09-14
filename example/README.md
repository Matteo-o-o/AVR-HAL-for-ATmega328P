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

| USB-TTL Pin | Signal | → | ATmega328P Pin | Function |
|:-----------:|:------:|:-:|:---------------|:---------|
| TXD | TX | → | Pin 2 (PD0) | RXD (ATmega328P Receive) |
| RXD | RX | → | Pin 3 (PD1) | TXD (ATmega328P Transmit) |
| GND | GND | → | Pin 8, 22 | Common Ground |
| VCC | 5V / 3.3V | → | Pin 7 | Power Supply *(optional if powered via USBasp)* |

*Note: Ensure cross-wiring between the adapter and the MCU (`TX → RX` and `RX → TX`). Always connect a common ground between the USB-TTL adapter and the MCU.*

#### Serial Terminal Setup

To interact with the UART tests, you must use an external serial terminal software configured to **(value in uart_init()) baud, 8 data bits, no parity, 1 stop bit (8N1)**:

- **Linux (`picocom`)**:
  ```bash
  picocom -b 9600 /dev/ttyUSB0 --echo
Other serial terminal software can also be used, such as PuTTY, Minicom, Screen, Tera Term, or Serial Monitor in the Arduino IDE

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

## UART / USART

### Serial Interactive LED Control

- Pins: 
  - `PD0` (PIN 2) (RXD) & `PD1` (PIN 3) (TXD)
  - `PB5` (PIN 19) (Output LED)
- Wiring:
  - Wire the USB to TTL serial adapter as described in the setup section.
  - Setup the LED on `PB5` like in LED Blinking.
- Behavior:
  - Displays a welcome banner and instructions upon startup over the serial interface.
  - Listens for serial commands non-blockingly:
    - Sending `'1'` turns the LED on `PB5` ON and responds with `LED: ON`.
    - Sending `'0'` turns the LED on `PB5` OFF and responds with `LED: OFF`.
    - Ignores `\r` / `\n` characters and reports unhandled commands.



# ADC

### Potentiometer Readout on ADC0

- Pins:
  - `PD0` (PIN 2) (RXD) & `PD1` (PIN 3) (TXD)
  - `PC0` (PIN 23) (ADC0)
- Wiring:
  - Wire the USB to TTL serial adapter as described in the setup section.
  - Connect the middle terminal (wiper) of a 10kΩ potentiometer to `PC0` (PIN 23).
  - Connect one outer terminal of the potentiometer to `VCC` (PIN 7) and the other to `GND` (PIN 8/22).
- Behavior:
  - Reads the analog voltage from the potentiometer on `ADC0` every 500 ms.
  - Converts the 10-bit raw digital reading ($0$ to $1023$) to millivolts using 32-bit integer arithmetic.
  - Formats and outputs both raw values and calculated voltages (as `X.XXX V`) over UART via redirected `printf()`.

## EXTERNAL INTERRUPTS

### INT0 Dedicated Hardware Interrupt

- Pins:
  - `PD2` (PIN 4) (INT0 Input)
  - `PB5` (PIN 19) (Output LED)
  - `PD0` (PIN 2) (RXD) & `PD1` (PIN 3) (TXD)
- Wiring:
  - Setup the USB-TTL adapter as described in the setup section and `PB5` LED like in LED Blinking example.
  - Connect one terminal of a push-button to `PD2` and the other directly to `GND`.
- Behavior:
  - Configures `PD2` with internal pull-up and enables `INT0` falling-edge interrupt.
  - On button press, `ISR(INT0_vect)` debounces the signal and sets a volatile event flag.
  - The main loop processes the flag non-blockingly, toggles `PB5` LED, and outputs event telemetry via UART `printf()`.
