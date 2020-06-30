/*
 * display.c
 *
 * Created: 29.10.2019 07:14:08
 *  Author: Kenan
 */ 
#include "display.h"
#include "u8g.h"

void renderDisplay(u8g_t *u8g, char a[20],char b[20])
{
	u8g_FirstPage(u8g);
	do
	{
		u8g_DrawStr90(u8g, 110, 0, "Joy-");
		u8g_DrawStr90(u8g, 100, 0, "Stick");
		u8g_DrawStr90(u8g, 90, 0, a);
		u8g_DrawStr90(u8g, 80, 0, "Magnet");
		u8g_DrawStr90(u8g, 70, 0, b);
	} while ( u8g_NextPage(u8g) );
}


void calibrationHello(u8g_t *u8g)
{
	u8g_FirstPage(u8g);
	do
	{
		u8g_DrawStr90(u8g,110,0, "Welcome");
		u8g_DrawStr90(u8g,100,0, "to");
		u8g_DrawStr90(u8g,90,0, "calibration");
		u8g_DrawStr90(u8g,80,0, "mode!");
		u8g_DrawStr90(u8g,60,0, "press");
		u8g_DrawStr90(u8g,50,0, "trigger");
		u8g_DrawStr90(u8g,40,0, "to the");
		u8g_DrawStr90(u8g,30,0, "end point");
	} while ( u8g_NextPage(u8g) );
}

void calibrationPressedDisplay(u8g_t *u8g, uint8_t countDown)
{
	u8g_DrawStr90(u8g,110,0, "hold");
	u8g_DrawStr90(u8g,100,0, "trigger");
	u8g_DrawStr90(u8g,90,0, "to the");
	u8g_DrawStr90(u8g,80,0, "end point");
	u8g_DrawStr90(u8g,70,0, "for");
	u8g_DrawStr90(u8g,60,0, (char*) &countDown);
	u8g_DrawStr90(u8g,50,0, "seconds");
}

void calibrateReslease(u8g_t *u8g)
{
	u8g_DrawStr90(u8g,110,0, "release");
	u8g_DrawStr90(u8g,100,0, "trigger");
	u8g_DrawStr90(u8g,90,0, "to the");
	u8g_DrawStr90(u8g,80,0, "end point");
}

void calibrationReleasedDisplay(u8g_t *u8g, uint8_t countDown)
{
	u8g_DrawStr90(u8g,110,0, "release");
	u8g_DrawStr90(u8g,100,0, "trigger");
	u8g_DrawStr90(u8g,90,0, "to the");
	u8g_DrawStr90(u8g,80,0, "end point");
	u8g_DrawStr90(u8g,70,0, "for");
	u8g_DrawStr90(u8g,60,0, (char*) &countDown);
	u8g_DrawStr90(u8g,50,0, "seconds");
}