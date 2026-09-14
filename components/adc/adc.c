#include "adc.h"
#include <avr/io.h>


static uint16_t g_vref_adc_mv = 5000;

void adc_init(adc_ref_t ref){
    // Clean previous adc config 
    ADMUX = (ADMUX & (~((1 << REFS0) | (1 << REFS1))));
    // Set config (BIT REFS0:1 in ADMUX)
    ADMUX |= (ref & 0x03) << REFS0;

    if(ref == ADC_REF_INTERNAL_1V1){
        g_vref_adc_mv = 1100;
    }
    else{
        g_vref_adc_mv = 5000;
    }
    // Configure Prescaler
    // Target ADC clock frequency: 50 kHz to 200 kHz (for full 10-bit accuracy)
    // F_CPU = 8 MHz -> Prescaler 64 gives: 8 000 000 / 64 = 125 kHz 
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1); // ADPS[2:0] = 110 (Prescaler 64)

    // Enable ADC 
    ADCSRA |= (1 << ADEN);
}


uint16_t adc_read_single(adc_channel_t channel){
    // Channel selection bit MUX0:MUX3
    // Clean previous config + set the current setup
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F );

    // start conversion 
    ADCSRA |= (1 << ADSC);

    //wait end of conversion (ADSC -> 0 => end)
    while( ADCSRA & (1<<ADSC));

    return ADC; // ADCH + ADCL
} 

uint16_t adc_read_voltage(adc_channel_t channel){
    uint16_t raw_value = adc_read_single(channel);

    return (uint16_t)(((uint32_t)raw_value * g_vref_adc_mv) / 1024UL);
}