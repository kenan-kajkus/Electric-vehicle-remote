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
#include <util/atomic.h>
#include <avr/wdt.h>
#include <string.h>

#include "display.h"
#include "ringbufferAveraging.h"


#define CTC_MATCH_OVERFLOW ((F_CPU / 1000)/8)

#define SSD1306_ADDR  0x3C

int dir = 1;
int motorSpeed = 0;
volatile unsigned long timer1_millis;
long milliseconds_since;
int x;
u8g_t u8g;
ringbufferAveraging_t HallSensorApproximation;
ringbufferAveraging_t Stick;
unsigned int c;
char buffer[7];
int  num=134;

void ADC_Init()
{
	 ADMUX = (1<<REFS0);
	 ADCSRA = (1<<ADPS1) | (1<<ADPS0);   
	 ADCSRA |= (1<<ADEN);
	 ADCSRA |= (1<<ADSC);                  // eine ADC-Wandlung
	 while (ADCSRA & (1<<ADSC) ) {         // auf Abschluss der Konvertierung warten
	 }
	 (void) ADCW;
}

/* ADC Einzelmessung */
uint16_t ADC_Read( uint8_t channel )
{
	// Kanal waehlen, ohne andere Bits zu beeinflußen
	ADMUX = (ADMUX & ~(0x1F)) | (channel & 0x1F);
	ADCSRA |= (1<<ADSC);            // eine Wandlung "single conversion"
	while (ADCSRA & (1<<ADSC) ) {   // auf Abschluss der Konvertierung warten
	}
	return ADCW;                    // ADC auslesen und zurückgeben
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

ISR (TIMER1_COMPA_vect)
{
	timer1_millis++;
}

ISR(ADC_vect){

}

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
	//check if uart interrupt is enable
	if(UCSR0B&(1<<UDRIE0))
	{
		return ;
	}
	static unsigned long lasttime=0;
	//TODO 0 mit millis() tauschen
	if(lasttime+49>millis()){
		return ;
	}
	addValue(&Stick,percentage(ADC_Read(ADC1D)));
	
	motorSpeed = getRingbufferAverage(&Stick);
	char text[20] = "0q\t";
	char n[4];
	
	integerToChar(n,motorSpeed);
	strcat(text,n);
	strcat(text,"\t1\n");
		
	uart_puts(text);
	lasttime = millis();
}
void setup()
{
	x=0;
    /*
     *  Initialize UART library, pass baudrate and AVR cpu clock
     *  with the macro 
     *  UART_BAUD_SELECT() (normal speed mode )
     *  or 
     *  UART_BAUD_SELECT_DOUBLE_SPEED() ( double speed mode)
     */
    uart_init(UART_BAUD_SELECT_DOUBLE_SPEED(UART_BAUD_RATE,F_CPU)); 

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
	  //Timer ende
	  
	  DDRD |= (0<< PIND5);
	  //LED-Test-pin
	  DDRC |= (1 << PINC0);
	  
	  CLKPR = 0x80;
	  CLKPR = 0x00;

	  u8g_InitI2C(&u8g, &u8g_dev_ssd1306_128x32_i2c, U8G_I2C_OPT_NONE);
	  u8g_SetFont(&u8g,u8g_font_6x10);
	  //u8g_SetFont(&u8g, u8g_font_04b_03);
	  
	  InitRingbufferAveraging(&HallSensorApproximation);
	  InitRingbufferAveraging(&Stick);
}

void draw(void)
{
	static unsigned long lasttime=0;
	static unsigned int count = 0;
	//TODO 0 mit millis() tauschen
	if(lasttime+50>millis()){
		return ;
	}
	char a[20];
	char m[4];
	int magnet = getRingbufferAverage(&HallSensorApproximation);
	integerToChar(m,magnet);
	sprintf(a,"%d",motorSpeed);
	if(count==0)
	{
		u8g_FirstPage(&u8g);
		renderDisplay(&u8g,a,m);
		u8g_NextPage(&u8g);
		count = 1;
	}
	else
	{
		if (count>2)
		{			
			count=0;
		}
		else{
			count++;
		}
		renderDisplay(&u8g,a,m);
		u8g_NextPage(&u8g);
		
	}
	lasttime = millis();
	
}

void sample()
{	
	static unsigned long lasttime=0;
	//TODO 0 mit millis() tauschen
	if(lasttime+20>millis()){
		return ;
	}
	addValue(&HallSensorApproximation,ADC_Read(ADC3D));
	lasttime = millis();
}


int main(void)
{
	wdt_enable(WDTO_8S);
	wdt_reset();
	wdt_disable();
	ADC_Init();
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
			state = display;
			break;
		case receive:
			
			break;
		case send:
			state = display;
			wdt_enable(WDTO_1S);
			sendData();
			wdt_disable();
			wdt_reset();
			break;
		case display:
			state = send;
			wdt_enable(WDTO_1S);
			draw();
			wdt_disable();
			wdt_reset();
			
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