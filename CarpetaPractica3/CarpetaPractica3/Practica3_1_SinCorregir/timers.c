#include "timers.h"

/*********************************************************************//**
 * @brief 		initialize the value and register of TIMER0 in the LPC1768
 * @param[in]	ciclosReloj: number of clock cycles  
 * @return 		None
 **********************************************************************/
void inicializarTimer0(uint32_t ciclosReloj){
	LPC_SC-> PCLKSEL0 |= (1<<2);//le metemos 01 => son 100MHz CCLK, 3:2
	LPC_SC-> PCONP |=  (1<<1);// De esta manera hablitamos la configuración de energía. PG 65.
	
	//LPC_TIM0->CTCR = 1;// ¿Cuándo se usan?
	LPC_TIM0-> MCR |= (1<<0)|(1<<1);//interrupción será generada cuando MR0=TC. TC será reseteado is MR0 coinciden.
	LPC_TIM0-> MR0 = ciclosReloj;
	
	LPC_TIM0-> TCR = (1<<1);//resetear el contador
	LPC_TIM0-> TCR &= ~(1<<1);
	LPC_TIM0->TCR |= (1<<0); //Start timer
	NVIC_EnableIRQ(TIMER0_IRQn);
}

/*********************************************************************//**
 * @brief 		initialize the value and register of TIMER1 in the LPC1768
 * @param[in]	ciclosReloj: number of clock cycles  
 * @return 		None
 **********************************************************************/
void inicializarTimer1(uint32_t ciclosReloj){
	LPC_SC-> PCLKSEL0 |= (1<<4);//le metemos 01 => son 100MHz CCLK, 5:4
	LPC_SC-> PCONP |=  (1<<2);// De esta manera hablitamos la configuración de energía
	
	//LPC_TIM1->CTCR = 1;// ¿Cuándo se usan?
	LPC_TIM1-> MCR |= (1<<0)|(1<<1);//interrupción será generada cuando MR0=TC. TC será reseteado si MR0 coinciden.
	LPC_TIM1-> MR0 = ciclosReloj;//100ms
	
	LPC_TIM2-> TCR = (1<<1);//resetear el contador
	LPC_TIM2-> TCR &= ~(1<<1);
	LPC_TIM1->TCR |= (1<<0); //Start timer
	NVIC_EnableIRQ(TIMER1_IRQn);
}

/*********************************************************************//**
 * @brief 		initialize the value and register of TIMER2 in the LPC1768
 * @param[in]	ciclosReloj: number of clock cycles  
 * @return 		None
 **********************************************************************/
void inicializarTimer2(uint32_t ciclosReloj){
	LPC_SC-> PCLKSEL1 |= (1<<12);//le metemos 01 => son 100MHz CCLK, 13:12
	LPC_SC-> PCLKSEL1 &= ~(1<<13);
	LPC_SC-> PCONP |= (1<<22);// De esta manera hablitamos la configuración de energía
	
	LPC_TIM2-> TCR = (1<<1);//resetear el contador
	//LPC_TIM2->CTCR = 1;// ¿Cuándo se usan?
	LPC_TIM2-> MCR |= (1<<0)|(1<<1);//interrupción será generada cuando MR0=TC. TC será reseteado is MR0 coinciden.
	LPC_TIM2-> MR0 = ciclosReloj;//100ms
	
	LPC_TIM2-> TCR = (1<<1);//resetear el contador
	LPC_TIM2-> TCR &= ~(1<<1);
	LPC_TIM2->TCR |= (1<<0); //Start timer
	NVIC_EnableIRQ(TIMER2_IRQn);
}

/*********************************************************************//**
 * @brief 		initialize the value and register of TIMER3 in the LPC1768
 * @param[in]	ciclosReloj: number of clock cycles  
 * @return 		None
 **********************************************************************/

void inicializarTimer3(uint32_t ciclosReloj){
	LPC_SC-> PCLKSEL1 |= (1<<14);//le metemos 01 => son 100MHz CCLK, 15:14
	LPC_SC-> PCONP |= (1<<23);// De esta manera hablitamos la configuración de energía
	
	LPC_TIM3-> TCR = (1<<1);//resetear el contador
	//LPC_TIM3->CTCR = 1;// ¿Cuándo se usan?
	LPC_TIM3-> MCR |= (1<<0)|(1<<1);//interrupción será generada cuando MR0=TC. TC será reseteado is MR0 coinciden.
	LPC_TIM3-> MR0 = ciclosReloj;//100ms
	
	LPC_TIM3-> TCR = (1<<1);//resetear el contador
	LPC_TIM3-> TCR &= ~(1<<1);
	LPC_TIM3->TCR |= (1<<0); //Start timer
	NVIC_EnableIRQ(TIMER3_IRQn);
}
