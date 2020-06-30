/*
 * evcu-remote.c
 *
 * Created: 31.05.2019 21:04:56
 * Author : Kenan
 */ 

#include "main.h"
#include "uart.h"

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <string.h>

#include "atmega-adc/atmega-adc.h"
#include "display.h"
#include "ringbufferAveraging.h"
#include "triggerButton.h"
#include "timer.h"

#define SSD1306_ADDR  0x3C

int dir = 1;
int motorSpeed = 0;
long milliseconds_since;
u8g_t u8g;
ringbufferAveraging_t HallSensorApproximation;
ringbufferAveraging_t StickXBuffer;
ringbufferAveraging_t triggerButtonBuffer;

bufferedAnalogInput triggerButton;
bufferedAnalogInput stickX;

unsigned int c;
char buffer[7];
int  num=134;

void flash_led ()
{
	unsigned long milliseconds_current = millis();

	if (milliseconds_current - milliseconds_since > 10) {
		// LED connected to PC0/Analog 0
		PORTC ^= (1 << PINC0);
		milliseconds_since = milliseconds_current;
	}
}

void sendData(){
	wdt_enable(WDTO_1S);
	//check if UART interrupt is enable
	if(UCSR0B&(1<<UDRIE0))
	{
		return ;
	}
	static unsigned long lasttime=0;
	//TODO 0 mit millis() tauschen
	if(lasttime+49>millis()){
		return ;
	}
	
	motorSpeed = getValue(&stickX);
	char text[20] = "0q\t";
	char n[4];
	
	integerToChar(n,motorSpeed);
	strcat(text,n);
	strcat(text,"\t1\n");
		
	uart_puts(text);

	wdt_disable();
	wdt_reset();
	lasttime = millis();
}
void setup()
{
	/*
    *  Initialize UART library, pass baud rate and AVR CPU clock
    *  with the macro 
    *  UART_BAUD_SELECT() (normal speed mode )
    *  or 
    *  UART_BAUD_SELECT_DOUBLE_SPEED() ( double speed mode)
	*/
	uart_init(UART_BAUD_SELECT_DOUBLE_SPEED(UART_BAUD_RATE,F_CPU)); 
	startTimer();
	  
	DDRD |= (0<< PIND5);
	//LED-Test-pin
	DDRC |= (1 << PINC0);
	  
	CLKPR = 0x80;
	CLKPR = 0x00;

	u8g_InitI2C(&u8g, &u8g_dev_ssd1306_128x32_i2c, U8G_I2C_OPT_FAST);
	u8g_SetFont(&u8g,u8g_font_6x10);
	//u8g_SetFont(&u8g, u8g_font_04b_03);
	  
	InitRingbufferAveraging(&HallSensorApproximation);
	InitRingbufferAveraging(&StickXBuffer);
	InitRingbufferAveraging(&triggerButtonBuffer);

	InitBufferedAnalogInput(&triggerButton,ADC_PRESCALER_2,ADC_VREF_AVCC,ADCH3,5,&triggerButtonBuffer);
	InitBufferedAnalogInput(&stickX,ADC_PRESCALER_2,ADC_VREF_AVCC,ADCH1,5,&StickXBuffer);
}

void calibration()
{
	//TODO
}

void draw(void)
{
	static unsigned long lasttime=0;
	wdt_enable(WDTO_1S);

	//TODO 0 mit millis() tauschen
	if(lasttime+50>millis()){
		return ;
	}
	char a[20];
	char m[4];
	int magnet = getValue(&triggerButton);

	integerToChar(m,magnet);
	sprintf(a,"%d",motorSpeed);

	u8g_FirstPage(&u8g);
	do
	{
		renderDisplay(&u8g,a,m);
	} while ( u8g_NextPage(&u8g) );
	
	lasttime = millis();
	wdt_disable();
	wdt_reset();
}

void sample()
{	
	sampleAnalogInput(&triggerButton);
	sampleAnalogInput(&stickX);
}


int main(void)
{
	wdt_enable(WDTO_8S);
	wdt_reset();
	wdt_disable();
	enum statemachine state = empty;   
    while (1) 
    {
		switch (state)
		{
		case empty:
			state = boot;
			break;
		case boot:
			setup();
			state = calibrate;
			break;
		case calibrate:
			calibration();
			state = display;
			break;
		case receive:
			
			break;
		case send:
			state = display;
			sendData();
			break;
		case display:
			state = send;
			draw();			
			break;
		}
		sample();
		//flash_led();
		
    }
}

void readInput()
{
	static unsigned long lasttime=0;
	//TODO 0 mit millis() tauschen
	if(lasttime+50>millis()){
		return ;
	}
}


void integerToChar(char* chararray, int number)
{
	chararray[0] = (number/100)%10+48;
	chararray[1] = (number/10)%10+48;
	chararray[2] = (number/1)%10+48;
	chararray[3] = '\0';
}

int percentage(long x)
{
	return (100*x)/1023;
} 