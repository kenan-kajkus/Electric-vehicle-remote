/*
 * main.h
 *
 * Created: 31.05.2019 21:16:18
 *  Author: Kenan
 */ 


#ifndef MAIN_H_
#define MAIN_H_

/* Define UART baud rate here */
#define UART_BAUD_RATE      9600
volatile  int receivedUart = 0;
enum statemachine
{
	empty,
	boot,
	receive,
	send,
	display	
	};
void setup();
void readInput();
void sent();
void integerToChar(char* chararray, int number);
int percentage(long x);
#endif /* MAIN_H_ */