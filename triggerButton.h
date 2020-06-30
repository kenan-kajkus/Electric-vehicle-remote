/*
 * triggerButton.h
 *
 * Created: 30.06.2020 09:16:41
 *  Author: kajku001
 */ 


#ifndef TRIGGERBUTTON_H_
#define TRIGGERBUTTON_H_

#include <stdint.h>
#include "ringbufferAveraging.h"

typedef struct _triggerButton_t bufferedAnalogInput;

void InitBufferedAnalogInput(bufferedAnalogInput * _analogInput, uint8_t prescaler, uint8_t vref, uint8_t channel, uint8_t samplerate, ringbufferAveraging_t * buffer);
void sampleAnalogInput(bufferedAnalogInput * analogInput);
int getValue(bufferedAnalogInput * analogInput);

struct _triggerButton_t
{
	uint8_t prescaler;
	uint8_t vref;
	uint8_t channel;
	uint8_t samplerate;
	ringbufferAveraging_t * buffer;
};



#endif /* TRIGGERBUTTON_H_ */