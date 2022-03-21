#include "LPC17xx.h"
#include "RTC.h"
#include "LCD.h"
#include "cmsis_os.h"


int main(void){
	
	osKernelInitialize ();
	init_RTC();
	init(); //Inicializar la pantalla
	habilitar_deshabilitar_RTC( HABILITADO ); //Habilitar el RTC
	interrupciones_RTC_deshabilitar(); //Deshabilitamos las interrupciones
	generar_alarma_RTC( SEGUNDOS );
	set_alarma( SEGUNDOS,  0 );
	osKernelStart ();
	
	while(1);
}
