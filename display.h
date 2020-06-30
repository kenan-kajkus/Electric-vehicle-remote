/*
 * display.h
 *
 * Created: 29.10.2019 07:14:24
 *  Author: Kenan
 */ 


#ifndef DISPLAY_H_
#define DISPLAY_H_
#include "u8g.h"
void InitDisplay();
void renderDisplay(u8g_t * u8g, char a[20],char b[20]);
void calibrationHello(u8g_t *u8g);
void calibrationPressedDisplay(u8g_t *u8g, uint8_t countDown);
void calibrateReslease(u8g_t *u8g);
void calibrationReleasedDisplay(u8g_t *u8g, uint8_t countDown);

#endif /* DISPLAY_H_ */