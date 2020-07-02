/*
 * timer.c
 *
 * Created: 30.06.2020 10:34:48
 *  Author: kajku001
 */ 
#include <avr/interrupt.h>
#include <util/atomic.h>

#include "timer.h"

 void startTimer()
 {
	// Timer
	// CTC mode, Clock/8
	TCCR1B |= (1 << WGM12) | (1 << CS11);
	// Load the high byte, then the low byte
	// into the output compare
	OCR1AH = (CTC_MATCH_OVERFLOW >> 8)& 0xFF;
	OCR1AL = CTC_MATCH_OVERFLOW& 0xFF;
 
	// Enable the compare match interrupt
	TIMSK1 |= (1 << OCIE1A);
	sei();
	//Timer end
 }

 unsigned long millis ()
 {
	 unsigned long millis_return;

	 // Ensure this cannot be disrupted
	 ATOMIC_BLOCK(ATOMIC_FORCEON) {
		 millis_return = timer1_millis;
	 }
	 
	 return millis_return;
 }

 void wait(unsigned long waitMills)
 {
	unsigned long start = millis();
	while ( start+waitMills > millis())
	{
	}
 }

 ISR (TIMER1_COMPA_vect)
 {
	 timer1_millis++;
 }