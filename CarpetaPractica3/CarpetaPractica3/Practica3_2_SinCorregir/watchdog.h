#ifndef WATCHDOG_H
#define WATCHDOG_H
#include "LPC17xx.h"

void watchdog_init( int time );
void modo_watchDog( int modo );
void watchdog_Alimentacion( void );

#endif