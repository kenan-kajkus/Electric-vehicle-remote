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

void InitBufferedAnalogInput(bufferedAnalogInput * _analogInput, uint8_t channel, unsigned long samplerate, ringbufferAveraging_t * buffer);
void sampleAnalogInput(bufferedAnalogInput * analogInput);
uint16_t getValue(bufferedAnalogInput * analogInput);

struct _triggerButton_t
{
	uint8_t channel;
	unsigned long samplerate;
	ringbufferAveraging_t * buffer;
	unsigned long lasttime;
};



#endif /* TRIGGERBUTTON_H_ */