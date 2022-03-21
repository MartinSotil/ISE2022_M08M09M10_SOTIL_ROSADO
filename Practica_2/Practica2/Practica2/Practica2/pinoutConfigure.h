#ifndef PINOUTCONFIGURE_H
#define PINOUTCONFIGURE_H

#include "LPC17xx.h"
#include "PIN_LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "cmsis_os.h"  

#define portLeds 1
#define pinLed1 18
#define pinLed2 20
#define pinLed3 21
#define pinLed4 23

void pinConfiguration();

void funcionalidadLeds( uint8_t led);

int32_t LED_Off (uint32_t num);

int32_t LED_SetOut (uint32_t val);

int32_t LED_On (uint32_t num);

void barrido_leds( bool barrido );

#endif