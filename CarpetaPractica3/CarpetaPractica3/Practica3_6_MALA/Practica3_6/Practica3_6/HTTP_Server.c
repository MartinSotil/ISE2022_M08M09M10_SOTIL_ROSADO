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
#include "SNTP.h"
#include "gestor_pulsador.h"
#include "HTTP_Server_CGI.h"
#include "gestion_FLASH.h" 

//extern GLCD_FONT GLCD_Font_6x8;
//extern GLCD_FONT GLCD_Font_16x24;

bool LEDrun;

uint8_t array[16]={0xA,0xB,0xC,0xA,0xB,0xC,0xA,0xB,0xC,0xA,0xB,0xC,0xA,0xB,0xC,0xE};
uint8_t lectura;
uint8_t arrayRetorno[16];


bool escribirLCD1=false;
bool escribirLCD2=false;
bool bool_ADC=false;

uint16_t medida=0;

static void BlinkLed (void const *arg);


osThreadDef(BlinkLed, osPriorityNormal, 1, 0);





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
	return 0;
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
	init_RTC();
	init(); //Inicializar el display LCD
  Init_pulsador();
	init_SNTP();
	

	osThreadCreate (osThread(BlinkLed), NULL);
  //osThreadCreate (osThread(ADC_hilo), NULL);
	habilitar_deshabilitar_RTC( HABILITADO ); //Habilitar el RTC
	interrupciones_RTC_deshabilitar(); //Deshabilitamos las interrupciones
	generar_alarma_RTC( SEGUNDOS );
	set_alarma( SEGUNDOS,  0 );
  osSignalSet(id_SNTP,0x0004);
	osKernelStart ();
	

	 borrar_sector(22);
	 escribirMemoria(22,0x00040000,array,16);
	 leerFlash(0x00040000,&lectura);
	 leerFlashxPosicionesyRetornarArray( 0x00040000,16, arrayRetorno);
	 modificarXposicion( 22,0x00040000, 0xD,14);
  

		
	
  while(1) {
    net_main ();
    osThreadYield ();
		
  }
}
