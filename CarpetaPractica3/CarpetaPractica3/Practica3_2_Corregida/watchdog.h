#ifndef WATCHDOG_H
#define WATCHDOG_H
#include "LPC17xx.h"
#include "PIN_LPC17xx.h"

void watchdog_init( int time );
void modo_watchDog( int modo );
void watchdog_Alimentacion( void );
bool reset_por_watchdog(void);

#endif