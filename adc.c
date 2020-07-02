/*
 * adc.c
 *
 * Created: 02.07.2020 20:15:58
 *  Author: Kenan
 */ 
#include "adc.h"

void ADC_Init()
{
	ADMUX = (1<<REFS0);
	ADCSRA = (1<<ADPS1) | (1<<ADPS0);
	ADCSRA |= (1<<ADEN);
	ADCSRA |= (1<<ADSC);                  // eine ADC-Wandlung
	while (ADCSRA & (1<<ADSC) ) {         // auf Abschluss der Konvertierung warten
	}
	(void) ADCW;
}

/* ADC single sample  */
uint16_t ADC_Read( uint8_t channel )
{
	// Chose channel, without changing other bits
	ADMUX = (ADMUX & ~(0x1F)) | (channel & 0x1F);
	ADCSRA |= (1<<ADSC);            // single conversion
	while (ADCSRA & (1<<ADSC) ) {   // wait until conversion is finished
	}
	return ADCW;                    // return ADC value
}