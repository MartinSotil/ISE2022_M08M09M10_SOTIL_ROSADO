#ifndef TIMERS_H
#define TIMERS_H

#include "GPIO_LPC17xx.h"
#include "LPC17xx.h"
#include "PIN_LPC17xx.h"


void inicializarTimer0(uint32_t ciclosReloj);
void inicializarTimer1(uint32_t ciclosReloj);
void inicializarTimer2(uint32_t ciclosReloj);
void inicializarTimer3(uint32_t ciclosReloj);

#endif
