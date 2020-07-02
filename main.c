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

#include "adc.h"
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
ringbufferAveraging_t StickYBuffer;
ringbufferAveraging_t triggerButtonBuffer;

bufferedAnalogInput triggerButton;
bufferedAnalogInput stickX;
bufferedAnalogInput stickY;

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
	int sy = getValue(&stickY);
	int mag = getValue(&triggerButton);
	char text[20] = "";
	char n[5];
	char magStr[5];
	
	//integerToChar(n,motorSpeed);
	char test[5];
	sprintf(test,"%d",motorSpeed);
	sprintf(n,"%d",sy);
	sprintf(magStr,"%d",mag);
	strcat(text,"x");
	strcat(text,test);
	strcat(text,"\t");
	strcat(text,"y");
	strcat(text,n);
	strcat(text,"\t");
	strcat(text,"m");
	strcat(text,magStr);
	strcat(text,"\n");
		
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
	ADC_Init();
	
	PORTD |= (1<<PIND5);  
	DDRD |= (0<< PIND5);
	//LED-Test-pin
	DDRC |= (1 << PINC0);
	  
	CLKPR = 0x80;
	CLKPR = 0x00;

	u8g_InitI2C(&u8g, &u8g_dev_ssd1306_128x32_i2c, U8G_I2C_OPT_FAST);
	u8g_SetFont(&u8g,u8g_font_5x8);
	//u8g_SetFont(&u8g, u8g_font_04b_03);
	  
	InitRingbufferAveraging(&HallSensorApproximation);
	InitRingbufferAveraging(&StickXBuffer);
	InitRingbufferAveraging(&StickYBuffer);
	InitRingbufferAveraging(&triggerButtonBuffer);

	InitBufferedAnalogInput(&triggerButton,ADCH3,50,&triggerButtonBuffer);
	InitBufferedAnalogInput(&stickX,ADCH1,10,&StickXBuffer);
	InitBufferedAnalogInput(&stickY,ADCH2,10,&StickYBuffer);
}

void calibration()
{
	if((PIND & (1 << PIND5)) == (1 << PIND5)) {
		return;
	}
	calibrationHello(&u8g);
	wait(1000);
	while ((PIND & (1 << PIND5)) == (1 << PIND5))
	{
	}

	int seconds = 5;
	do 
	{
		calibrationPressedDisplay(&u8g, seconds);
		for(int i = 20;i>0;i--)
		{
			wait(50);
			sampleAnalogInput(&triggerButton);
			if((PIND & (1 << PIND5)) == (1 << PIND5)) {
				seconds = 6;
			}
		}
		seconds--;
	} while (seconds>0 /*& pin*/ );

	//int a = getValue(&triggerButton);
	
	calibrateReslease(&u8g);
	while ((PIND & (1 << PIND5)) != (1 << PIND5))
	{
	}

	seconds=5;
	do
	{
		calibrationReleasedDisplay(&u8g, seconds);
		for(int i = 20; i>0; i--)
		{
			wait(50);
			sampleAnalogInput(&triggerButton);
			if((PIND & (1 << PIND5)) != (1 << PIND5)) 
			{
				seconds = 6;
			}
		}
		seconds--;
	} while (seconds>0 /*& pin*/ );
	
	//int b = getValue(&triggerButton);

}

void draw(void)
{
	static unsigned long lasttime=0;
	wdt_enable(WDTO_1S);

	//TODO 0 mit millis() tauschen
	if(lasttime+100>millis()){
		return ;
	}
	char a[20];
	char b[20];
	uint16_t a1 = getValue(&stickX);
	uint16_t b1 = getValue(&triggerButton);
	sprintf(a,"%d",a1);
	sprintf(b,"%d",b1);

	renderDisplay(&u8g,a,b);
	
	lasttime = millis();
	wdt_disable();
	wdt_reset();
}

void sample()
{	
	sampleAnalogInput(&triggerButton);
	sampleAnalogInput(&stickX);
	sampleAnalogInput(&stickY);
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


void integerToChar(char* chararray, int number)
{
	chararray[0] = (number/100)%10+48;
	chararray[1] = (number/10)%10+48;
	chararray[2] = (number/1)%10+48;
	chararray[3] = '\0';
}

long map(long x, long in_min, long in_max)
{
	long out_min = 0;
	long out_max = 254;
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}