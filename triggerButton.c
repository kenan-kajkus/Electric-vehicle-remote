/*
 * triggerButton.c
 *
 * Created: 30.06.2020 09:16:59
 *  Author: kajku001
 */ 
#include "triggerButton.h"
#include "adc.h"
#include "timer.h"

void InitBufferedAnalogInput(bufferedAnalogInput * _analogInput, uint8_t channel, unsigned long samplerate, ringbufferAveraging_t * buffer)
{
	_analogInput->channel = channel;
	_analogInput->samplerate = samplerate;
	_analogInput->buffer = buffer;
	_analogInput->lasttime = 0;
}

void sampleAnalogInput(bufferedAnalogInput * _analogInput)
{
	if((_analogInput->lasttime + _analogInput->samplerate) > millis()){
		return ;
	}
	uint16_t sampleValue = ADC_Read(_analogInput->channel);
	addValue(_analogInput->buffer, sampleValue);

	_analogInput->lasttime = millis();
}

uint16_t getValue(bufferedAnalogInput * analogInput)
{	
	ringbufferAveraging_t * tempBuffer = analogInput->buffer; 
	return getRingbufferAverage(tempBuffer);
}