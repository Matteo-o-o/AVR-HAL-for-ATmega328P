#ifdef TIMER_H
#define TIMER_H

// Enum of PWM availible on Timer0
typedef enum{
    TIMER0_PWM_CHANNEL_A = 0;  // PD6
    TIMER0_PWM_CHANNEL_B;      // PD5
}timer0_pwm_channel_t;

// Initizialisation for Timer 1 in mode CTC 
void timer1_init_system_tick(void);

// Time elapsed since start of timer1_init_system_tick
uint32_t timer_millis(void);

bool timer_has_elapsed(uint32_t start_time,uint32_t interval_ms);

void time0_pwm_init(void);
void timer0_pwm_set_duty(timer0_pwm_channel_t channel,uint8_t duty_cycle);











#endif