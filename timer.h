/*
 * timer.h
 *
 * Created: 30.06.2020 10:35:03
 *  Author: kajku001
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#define CTC_MATCH_OVERFLOW ((F_CPU / 1000)/8)
volatile unsigned long timer1_millis;

void startTimer();
unsigned long millis();



#endif /* TIMER_H_ */