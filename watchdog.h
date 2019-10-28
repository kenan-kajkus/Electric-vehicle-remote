/*
 * watchdog.h
 *
 * Created: 10.08.2019 23:03:01
 *  Author: Kenan
 */ 

#ifndef WATCHDOG_H_
#define WATCHDOG_H_
#include "bollerwagen.h"
#include <avr/wdt.h>

// Initialize watchdog with a x second interrupt time
void watchdog_init();

// Reset watchdog. at least every x seconds after the
// first watchdog_init or AVR will go into emergency procedures.
inline void watchdog_reset() { wdt_reset(); }



#endif /* WATCHDOG_H_ */