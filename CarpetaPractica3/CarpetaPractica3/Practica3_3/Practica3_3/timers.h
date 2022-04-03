#ifndef TIMERS_H
#define TIMERS_H

#include "GPIO_LPC17xx.h"
#include "LPC17xx.h"
#include "PIN_LPC17xx.h"


void inicializarTimer0(uint32_t ciclosReloj);
void inicializarTimer1(uint32_t ciclosReloj);
void inicializarTimer2(uint32_t ciclosReloj);
void inicializarTimer3(uint32_t ciclosReloj);

/*void TIMER2_IRQHandler(void){
  flag_timer=true;
	LPC_TIM2-> MR0 = 49999999;//500ms (Volvemos a lanzar el timer)
	LPC_TIM2->IR |= (1<<0); //Limpiamos flag de la interrupcion
}*/

#endif
