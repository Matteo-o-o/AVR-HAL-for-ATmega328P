#ifndef ADC_H
#define ADC_H

#include <stdio.h>

typedef enum{
    ADC_REF_AREF = 0, // AREF, internal VREF turned off
    ADC_REF_AVCC = 1, // AVCC with external capacitor at AREF pin
    ADC_REF_INTERNAL_1V1 = 3 // Internal 1.1V voltage reference with external capacitor at AREF pin

}adc_ref_t;

typedef enum{
    ADC_CHANNEL_0 = 0,
    ADC_CHANNEL_1,
    ADC_CHANNEL_2,
    ADC_CHANNEL_3,
    ADC_CHANNEL_4,
    ADC_CHANNEL_5,
    ADC_CHANNEL_6,
    ADC_CHANNEL_TEMP = 8,
    // Nexts are  usefull for testing
    ADC_CHANNEL_1V1  = 14,  // Internal 1.1V reference voltage
    ADC_CHANNEL_GND  = 15   // Ground (0V)
}adc_channel_t;

// Define Vref et enable the ADC
void adc_inti(adc_ref_t ref);

// Read numérique value
uint16_t adc_read_single(adc_channel_t); 

// Read numerique value and converte in voltage (volt)
uint16_t adc_read_voltage(adc_channel_t);

#endif