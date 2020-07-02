/*
 * main.h
 *
 * Created: 31.05.2019 21:16:18
 *  Author: Kenan
 */ 


#ifndef MAIN_H_
#define MAIN_H_

/* Define UART baud rate here */
#define UART_BAUD_RATE      38400

volatile  int receivedUart = 0;
enum statemachine
{
	empty,
	boot,
	receive,
	send,
	display,
	calibrate	
	};
void setup();
void readInput();
void sent();
void integerToChar(char* chararray, int number);
void calibration();
long map(long x, long in_min, long in_max);

#endif /* MAIN_H_ */