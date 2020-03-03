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
	u8g_DrawStr90(u8g, 110, 0, "Joy-");
	u8g_DrawStr90(u8g, 100, 0, "Stick");
	u8g_DrawStr90(u8g, 90, 0, a);
	u8g_DrawStr90(u8g, 80, 0, "Magnet");
	u8g_DrawStr90(u8g, 70, 0, b);
}