#include "adc.h"
#include "LPC17xx.h"
#include "cmsis_os.h" 

/***************************************************************************************************
                                   CÓDIGO DE FUNCIONES EXTERNAS
 *****************************************************************************************************/

uint32_t f;
uint32_t p;
uint32_t resultado1;
uint32_t resultado2;

void  retar_ms_for(uint32_t ms);

void init_adc(void) {
	
	LPC_SC->PCONP |= PCONP_PCADC;								//Activamos el control sobre el módulo ADC
	LPC_SC->PCLKSEL0 = CCLKDIV8_ADC;						
  LPC_ADC-> ADCR	= ADC_PDN | ADC_CLK;				//ADC operativo
  LPC_PINCON->PINSEL3 |= PINSEL_ADC04;
	LPC_PINCON->PINSEL3 |= PINSEL_ADC05;
	LPC_ADC->ADINTEN |= (3<<4); //Habilitamos interrupciones del ACD 4 y 5;
	//NVIC_EnableIRQ(ADC_IRQn);                  
}

uint32_t conversion_ADC_P0T1(void){

  LPC_ADC-> ADCR &= ~ADC0_5;
  LPC_ADC-> ADCR |= 0xFFFFFF10; //  In software-controlled mode, only one of these bits should be 1
	LPC_ADC-> ADCR |= ADC0_4;
	retar_ms_for(9);
	

		LPC_ADC -> ADCR |= ADC_START;   //START  "001" en los bits [26:24]
	
		while((LPC_ADC -> ADGDR & FLAG_DONE) == 0); //Esperamos a que se realice la conversón
	
		resultado1 = ((LPC_ADC -> ADGDR) >> ADC_RESULT) & 0xFFF;

	return resultado1;
}

uint32_t conversion_ADC_P0T2(void){
	
  LPC_ADC-> ADCR &= ~ADC0_4;
  LPC_ADC-> ADCR |= 0xFFFFFF20; //  In software-controlled mode, only one of these bits should be 1
	LPC_ADC-> ADCR |= ADC0_5;
	retar_ms_for(9);
	

		LPC_ADC -> ADCR |= ADC_START;   //START  "001" en los bits [26:24]
	
		while((LPC_ADC -> ADGDR & FLAG_DONE) == 0); //Esperamos a que se realice la conversón
	
		resultado2 = ((LPC_ADC -> ADGDR) >> ADC_RESULT) & 0xFFF;

	return resultado2;
}


//Esta funcion tiene que ir en el while(1)
void conversion2ADCS( void ){
	LPC_ADC-> ADCR &= ~ADC0_5; //Desactivamos el ADC5
  LPC_ADC-> ADCR |= 0xFFFFFF10; //Activamos el ADC4
	
	LPC_ADC -> ADCR |= ADC_START;   //START  "001" en los bits [26:24]
	osDelay(10);
	
	
	LPC_ADC-> ADCR &= ~ADC0_4;    //Desactivamos el ADC4
  LPC_ADC-> ADCR |= 0xFFFFFF20;  //Activamos el ADC5
	LPC_ADC -> ADCR |= ADC_START;   //START  "001" en los bits [26:24]
	osDelay(10);
	
	
	
}

/*void ADC_IRQHandler(void){
	if( ((LPC_ADC -> ADSTAT) & 16 ) >> 4 ){
		resultado1 = ((LPC_ADC -> ADGDR) >> ADC_RESULT) & 0xFFF;
		
	}else if( ((LPC_ADC -> ADSTAT) & 32) >> 5 ){
	  resultado2 = ((LPC_ADC -> ADGDR) >> ADC_RESULT) & 0xFFF;
	}
	
}*/


void  retar_ms_for(uint32_t ms){
   for (f=0 ; f<ms; f++)
     for (p=0; p<6250 ; p++);
} 

