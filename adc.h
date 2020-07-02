/*
 * adc.h
 *
 * Created: 02.07.2020 20:16:12
 *  Author: Kenan
 */ 


#ifndef ADC_H_
#define ADC_H_
#include <avr/io.h>

uint16_t ADC_Read( uint8_t channel );
void ADC_Init();


#endif /* ADC_H_ */