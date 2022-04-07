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
#include "Led_RGB.h"

//*****************************OBTENER IP***************
extern ARM_DRIVER_ETH_MAC Driver_ETH_MAC0;
 ARM_DRIVER_ETH_MAC *ETH_MAC = &Driver_ETH_MAC0;
 
extern  LOCALM localm[];
#define LocM   localm[NETIF_ETH]

ARM_ETH_MAC_ADDR ETH_MAC_struct;

//OBTENER IP y MAC:
uint8_t bf_MAC[6];
uint8_t ip_device[4];
//******************************************************


bool LEDrun;
uint8_t P2;
//*************************VARIABLES FLASH***********************
uint8_t array[NUM_MAX_POS];
uint8_t arrayRetorno[NUM_MAX_POS];
//***************************************************************

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

    val = conversion_ADC_P0T2();
		if( val>arrayRetorno[12] ){
			led_azul();
		}else{
			apagar_led();
		}
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

void request_IP_device(uint8_t *bf_IP){
	
	int i;
	
	for(i = 0; i<4; i++){
		bf_IP[i] = LocM.IpAddr[i];
	}
	
}


void request_MAC_device(uint8_t *bf_MAC){
	
	int i;
	ETH_MAC->GetMacAddress(&ETH_MAC_struct);
	
	for(i = 0; i<6; i++){
		bf_MAC[i]  = ETH_MAC_struct.b[i];
	}
}


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
	Init_RGB(); //Para el Led RGB de la fase 7
	

	osThreadCreate (osThread(BlinkLed), NULL);
  //osThreadCreate (osThread(ADC_hilo), NULL);
	habilitar_deshabilitar_RTC( HABILITADO ); //Habilitar el RTC
	interrupciones_RTC_deshabilitar(); //Deshabilitamos las interrupciones
	generar_alarma_RTC( SEGUNDOS );
	set_alarma( SEGUNDOS,  0 );
  osSignalSet(id_SNTP,0x0004);
	osKernelStart ();
	
	//**********************************************************
	//Fase 7
  //**********************************************************
	//Guardamos un valor en la posicion 12 de la flash:
	modificarXposicion(22,0x00040000,0x0A,12); //0x7A=122
	//**********************************************************
	//Fase 6
	//**********************************************************
	
	request_IP_device(ip_device);
	request_MAC_device(bf_MAC);
	
   leerFlashxPosicionesyRetornarArray( 0x00040000,arrayRetorno);
	 borrar_sector(22); //Por si acaso no está borrado aún
   //Almacenamos la ip y la mac en las primeras 10 posiciones de la flash
	 array[0]=(uint8_t)ip_device[0];
	 array[1]=(uint8_t)ip_device[1];
	 array[2]=(uint8_t)ip_device[2];
	 array[3]=(uint8_t)ip_device[3];
	 array[4]=(uint8_t)bf_MAC[0];
	 array[5]=(uint8_t)bf_MAC[1];
	 array[6]=(uint8_t)bf_MAC[2];
	 array[7]=(uint8_t)bf_MAC[3];
	 array[8]=(uint8_t)bf_MAC[4];
	 array[9]=(uint8_t)bf_MAC[5];
	 escribirMemoria(22,0x00040000,array);
   if( arrayRetorno[10] == 0x01 ){ //Modo Running Leds
     LEDrun=true;
   }else if ( arrayRetorno[10] == 0x00 ){ //Modo Browser
     LEDrun=false;
		 P2=arrayRetorno[11];
		 LED_SetOut(P2);
   }

	 modificarXposicion(22,0x00040000,0x0A,12); //0x7A=122
	
  while(1) {
    net_main ();
    osThreadYield ();
		
  }
}
