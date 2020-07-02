/*
 * approximation.c
 *
 * Created: 07.11.2019 17:51:14
 *  Author: Kenan
 */ 
#include "ringbufferAveraging.h"
#include <stdlib.h>

void InitRingbufferAveraging(ringbufferAveraging_t * _ringbufferAveraging)
{
	
	_ringbufferAveraging->bufferSize = (1 <<bufferSizeShift);
	_ringbufferAveraging->bufferHead = 0;
	_ringbufferAveraging->values = (uint16_t*)calloc(_ringbufferAveraging->bufferSize,sizeof(uint16_t));
}

void addValue(ringbufferAveraging_t * _ringbufferAveraging, uint16_t newValue)
{
	 _ringbufferAveraging->values[_ringbufferAveraging->bufferHead] = newValue;
	_ringbufferAveraging->bufferHead = (_ringbufferAveraging->bufferHead+1)%_ringbufferAveraging->bufferSize;
}

uint16_t getRingbufferAverage(ringbufferAveraging_t * _ringbufferAveraging)
{
	uint16_t approximation = 0;
	for(int i = 0; i < _ringbufferAveraging->bufferSize; i++)
	{
		approximation = approximation + _ringbufferAveraging->values[i];
	}
	//shifting instead of div to be faster!!
	approximation = approximation >> bufferSizeShift;
	return approximation;
}