#include "watchdog.h"

void watchdog_init( int time ){
	LPC_SC->PCLKSEL0 |= (1<<0);
	LPC_SC->PCLKSEL0 &= ~(1<<1); //Elegimos el cclk/1 (10MH<z)
  LPC_WDT->WDCLKSEL |= (1<<0); //Elegimos el PCLK como reloj
  modo_wdYreset(); //Modo reset y interrupciones del wd
	LPC_WDT->WDTC = time;
	watchdog_Alimentacion();
	NVIC_EnableIRQ(WDT_IRQn);
}

//0->WDEN||1->WDRESET||2->WDTOF||3->WDINT
void modo_watchDog( int modo ){
	if( modo == 0){
		 LPC_WDT->WDMOD |= (1<<0);
		 LPC_WDT->WDMOD &= ~(1<<1);
		 LPC_WDT->WDMOD &= ~(1<<2);
		 LPC_WDT->WDMOD &= ~(1<<3);
	}else if( modo == 1 ){
		 LPC_WDT->WDMOD |= (1<<1);
		 LPC_WDT->WDMOD &= ~(1<<0);
		 LPC_WDT->WDMOD &= ~(1<<2);
		 LPC_WDT->WDMOD &= ~(1<<3);
	}else if( modo == 2 ){
		 LPC_WDT->WDMOD |= (1<<2);
		 LPC_WDT->WDMOD &= ~(1<<1);
		 LPC_WDT->WDMOD &= ~(1<<0);
		 LPC_WDT->WDMOD &= ~(1<<3);
	}else if( modo == 3 ){
		 LPC_WDT->WDMOD |= (1<<3);
		 LPC_WDT->WDMOD &= ~(1<<1);
		 LPC_WDT->WDMOD &= ~(1<<2);
		 LPC_WDT->WDMOD &= ~(1<<0);
	}
}

void watchdog_Alimentacion( void ){
	__disable_irq();
	LPC_WDT->WDFEED = 0xAA;
	LPC_WDT->WDFEED = 0x55;
	__enable_irq();
}

bool reset_por_watchdog(void){
	return (LPC_WDT->WDMOD & 0x04);
}

void modo_wdYreset(void){
	LPC_WDT->WDMOD |= 0x03; //No puedo usar la funcion, ponemos dos modos, el de antes y el de reset
}