#ifndef ESCLAVO_I2C_H
#define ESCLAVO_I2C_H

#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "Driver_I2C.h"
#include "GPIO_LPC17xx.h"

#define Esclavo_I2C_ADDR       	      0x28      
 
 
#define REG_GANANCIA_1   					    				0x01
#define REG_GANANCIA_5   					    				0x02
#define REG_GANANCIA_10    					  				0x03
#define REG_GANANCIA_50    					  				0x04
#define REG_GANANCIA_100    									0x05


#define REG_PROGRAMAR_UMBRAL	    						0x06
#define REG_HABILITAR_UMBRAL	    						0x07
#define REG_DESHABILITAR_UMBRAL   						0x08
#define REG_PETICION_GANANCIA	    						0x09
#define REG_PETICION_OVERLOAD	        				0x0A
#define REG_HABILITAR_INTERRUPCIONES	    		0x0B
#define REG_DESHABILITAR_INTERRUPCIONES	   		0x0C
#define REG_PETICION_ESTADO_OVERLOAD		   		0x0D


#define SIG_Esclavo_I2C							0x0001

void Init_i2c(void);
void Esclavo_I2C (void const *argument);
int Init_Esclavo_I2C(void);
void analizador_tramas(uint8_t trama);

#endif
