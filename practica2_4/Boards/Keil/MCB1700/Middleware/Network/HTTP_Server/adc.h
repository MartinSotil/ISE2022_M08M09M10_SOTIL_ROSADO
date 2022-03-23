#ifndef __ADC_H
#define __ADC_H


/***************************************************************************************************
                                        LIBRERIAS
 *****************************************************************************************************/

//#include "LPC17xx.h"
#include "stdint.h"


/***************************************************************************************************
                                        CONSTANTES
 *****************************************************************************************************/

//PUERTO Y PIN DE LOS POTENCIOMETROS

#define PORT_POTs 			1
#define PIN_POT1 				30
#define PIN_POT2 				31

#define S_ADC						0x01
#define MUESTRAS 				5  

//PINSEL
#define PINSEL_ADC04        ((uint32_t) (1<<28) | (1<<29)) //Define La función AD0.4 del registro PIN Function Select Register 3
#define PINSEL_ADC05        ((uint32_t) (1<<30) | (1<<31)) //Define La función AD0.5 del registro PIN Function Select Register 3
#define PCONP_PCADC         ((uint32_t) (1<<12))           //Bit 12 del registro PCONP.


/*-----------------------------------------------------------------------------------------------------------
*																																																						*
*																	CLOCK ADC.																																*
*																																																						*																	
--------------------------------------------------------------------------------------------------------------*/
#define CCLKDIV4_ADC        ((uint32_t) (0<<25)) 						//CCLK/4.
#define CCLKDIV8_ADC        ((uint32_t) (1<<24) | (1<<25)) 	//CCLK/8.
#define CCLKDIV2_ADC        ((uint32_t) (1<<25)) 						//CCLK/2.
#define CCLKDIV1_ADC        ((uint32_t) (1<<24)) 						//CCLK.

//ADCR
#define ADC0_4					     ((uint32_t) (1<<4)) 
#define ADC0_5					     ((uint32_t) (1<<5)) 
#define ADC_PDN					     ((uint32_t) (1<<21)) 
#define ADC_START					   ((uint32_t) (1<<24)) 
//#define ADC_CLK							 ((uint32_t) (10<<8))
#define ADC_CLK							 ((uint32_t) (1<<8))
#define ADC_INTEN5					 ((uint32_t) (1<<5))

//AGDR
#define FLAG_DONE					   ((uint32_t) (1U<<31)) 
#define ADC_RESULT					 	4UL // El resultado esta en los bits del 15:4 del registro AD0GDR. Al ser 12 bits como máximo da 4096


/***************************************************************************************************
                                   PROTOTIPO DE FUNCIONES EXTERNAS
 *****************************************************************************************************/
 
/* 		
 * FUNCIÓN: init_adc
 *      Inicializa el ADC.
 * Parámetros de entrada:
 *      Ninguno.
 * Parámetros de salida:
 *      Ninguno.
 * Valor devuelto por la función:
 *      Ninguno.		
*/
extern void init_adc(void);

/* 		
 * FUNCIÓN: conversion_ADC
 *      Realiza la conversión del potenciometro.
 * Parámetros de entrada:
 *      Ninguno.
 * Parámetros de salida:
 *      Ninguno.
 * Valor devuelto por la función:
 *      uint16_t : Entero de 16 bits que contiene el valor de la conversión.		
*/
extern uint32_t conversion_ADC_P0T1(void);

extern uint32_t conversion_ADC_P0T2(void);

void conversion2ADCS( void );


#endif // __ADC_H
