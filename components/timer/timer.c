#include "timer.h"


#ifndef F_CPU
#error "F_CPU need to be define in Makefile"
#endif

#define TIMER1_PRESCALER 64UL

static volatile uint32_t g_system_millis = 0;

// ISR TIMER1 : Reference time : 1ms 
ISR(TIMER1_COMPA_vect){
    g_system_millis++;

}

// TIMER 1 : 1 MS tick

void timer1_init_system_tick(void){

    // Reset : clean start
    TCNT1 = 0 ;

    // Compare value  : 
    OCR1A = (uint16_t)((F_CPU/TIMER1_PRESCALER)/1000UL)-1UL

    // Mode CTC (WGM12 = 1)
    TCCR1A = 0;
    TCCR1B = (1 << WGM12);

    // Prescaler config  64 = (CS11 = 1, CS10 = 1):
    TCCR1B |= (1 << CS11 ) | (1 << CS10);

    // Comparaison activation 
    TIMSK1 |= (1<<OCIE1A);

    sei();
}

uint32_t timer_millis(void)
{
    uint32_t millis_copy;


    uint8_t sreg = SREG;
    cli(); // 32 bits read with 8 bit bus (disable interupt)
    millis_copy = g_system_millis;    
    SREG = sreg; // Statu register restore (enable interupt)

    return millis_copy;

}

bool timer_has_elapsed(uint32_t start_time, uint32_t interval_ms){
    return (timer_millis() - start_time) >= interval_ms;
}

// TIMER 0 : PWM

void timer0_pwm_init(void){

    //Mode fast pwm (WGM01 = 1, WGM00=1)
    TCCR0A = (1 << WGM01) | (1 << WGM00);

    // Clear OC0A/OC0B on Compare Match (COM0A1 = 1, COM0B1 = 1)
    TCCR0A |= (1 << COM0A1) | (1 << COM0B1);

    // Prescaler 64 : CS01 = 1, CS00 = 1  |  Freq PWM = 8 MHz / (64 * 256) = ~488 Hz 
    TCCR0B = (1 << CS01) | (1 << CS00);

    // Rapports cycliques initialization
    OCR0A = 0;
    OCR0B = 0;

}

void timer0_pwm_set_duty(timer_pwm_channel_t channel, uint8_t duty_cycle){
    if (channel == TIMER0_PWM_CHANNEL_A) {
        OCR0A = duty_cycle;
    } else if (channel == TIMER0_PWM_CHANNEL_B) {
        OCR0B = duty_cycle;
    }
}




