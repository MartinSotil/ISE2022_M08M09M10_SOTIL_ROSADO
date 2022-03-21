/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network
 * Copyright (c) 2004-2014 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    HTTP_Server.c
 * Purpose: HTTP Server example
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include "HTTP_Server_CGI.h"
#include "cmsis_os.h"                   /* CMSIS RTOS definitions             */
#include "rl_net.h"                     /* Network definitions                */
#include "pinoutConfigure.h"
#include "LCD.h"
#include "adc.h"

//extern GLCD_FONT GLCD_Font_6x8;
//extern GLCD_FONT GLCD_Font_16x24;

bool LEDrun;

bool escribirLCD1=false;
bool escribirLCD2=false;
bool bool_ADC=false;

uint16_t medida=0;

static void BlinkLed (void const *arg);

osThreadDef(BlinkLed, osPriorityNormal, 1, 0);

static void pantalla (void const *arg);

osThreadDef(pantalla, osPriorityNormal, 1, 0);

//static void ADC_hilo (void const *arg);

//osThreadDef(ADC_hilo, osPriorityNormal, 1, 0);

/// Read analog inputs
uint16_t AD_in (uint32_t ch) {
  int32_t val = 0;

  if (ch == 0) {

    val = conversion_ADC_P0T1();
  }
  return (val);
}

/// Read digital inputs
uint8_t get_button (void) {
 // return (Buttons_GetState ());
}

/// IP address change notification
void dhcp_client_notify (uint32_t if_num,
                         dhcpClientOption opt, const uint8_t *val, uint32_t len) {
  if (opt == dhcpClientIPaddress) {
    // IP address has changed
   
  }
}

/*----------------------------------------------------------------------------
  Thread 'BlinkLed': Blink the LEDs on an eval board
 *---------------------------------------------------------------------------*/
static void BlinkLed (void const *arg) {
	LEDrun=true;
	while(1) {
			barrido_leds(LEDrun);
	}
}

static void pantalla (void const *arg){
	int i;
	while(1){
		if(escribirLCD1){
		  EscribeFrase( lcd_text[0] );
			escribirLCD1=false;
 		} else if (escribirLCD2){
      EscribeFrase2( lcd_text[1] );
      escribirLCD2=false;
    }
	}
}

/*static void ADC_hilo (void const *arg) {
	while(1) {
		osDelay(10);
		conversion2ADCS();
		
		
		
	}
}*/


/*----------------------------------------------------------------------------
  Main Thread 'main': Run Network
 *---------------------------------------------------------------------------*/
int main (void) {
	
	osKernelInitialize ();
  net_initialize     ();
	pinConfiguration();
  init_adc();
	init(); //Inicializar el display LCD

	osThreadCreate (osThread(BlinkLed), NULL);
  osThreadCreate (osThread(pantalla), NULL);
  //osThreadCreate (osThread(ADC_hilo), NULL);
	osKernelStart ();

  while(1) {
    net_main ();
    osThreadYield ();
		
  }
}
