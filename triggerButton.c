/*
 * triggerButton.c
 *
 * Created: 30.06.2020 09:16:59
 *  Author: kajku001
 */ 
#include "triggerButton.h"
#include "atmega-adc/atmega-adc.h"
#include "timer.h"

void InitBufferedAnalogInput(bufferedAnalogInput * _analogInput, uint8_t prescaler, uint8_t vref, uint8_t channel, uint8_t samplerate, ringbufferAveraging_t * buffer)
{
	_analogInput->prescaler = prescaler;
	_analogInput->vref = vref;
	_analogInput->channel = channel;
	_analogInput->samplerate = samplerate;
	_analogInput->buffer = buffer;
}

void sampleAnalogInput(bufferedAnalogInput * _analogInput)
{
	static unsigned long lasttime=0;
	if(lasttime + _analogInput->samplerate >millis()){
		return ;
	}
	uint16_t sampleValue = adc_read(_analogInput->prescaler, _analogInput->vref, _analogInput->channel);
	addValue(_analogInput->buffer, sampleValue);

	lasttime = millis();
}

int getValue(bufferedAnalogInput * analogInput)
{	
	ringbufferAveraging_t * tempBuffer = analogInput->buffer; 
	return getRingbufferAverage(tempBuffer);
}