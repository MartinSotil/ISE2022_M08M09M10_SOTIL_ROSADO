#include "LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "timers.h"
#include "watchdog.h"
#include "Led_RGB.h"

void init_joystick( void );

bool primera_vez=true;
bool flag_timer=false;
bool encendido_led3=false;
int encendido_led=0;
int estado=0;

int main(void){
	
	GPIO_SetDir(1,18,GPIO_DIR_OUTPUT);
	GPIO_SetDir(1,20,GPIO_DIR_OUTPUT);
	GPIO_SetDir(1,21,GPIO_DIR_OUTPUT);
	GPIO_SetDir(1,23,GPIO_DIR_OUTPUT);
	Init_RGB();
	init_joystick();
	inicializarTimer2( 99999999 ); //Para hacer el timer de 1s (100000000-1 tiks)
	GPIO_PinWrite(1,18,1);
	watchdog_init(125000000);
	inicializarTimer0(500000000); //Para el led RGB (4 segudnos)
	
	if(LPC_WDT->WDMOD & 0x04){
		led_rojo();
	}
	else{
		led_verde();
	}
	
	while(1){
		watchdog_Alimentacion(); //Alimentamos el watchdog
		if(flag_timer){
			
				if(primera_vez){
					GPIO_PinWrite(1,18,0);
					primera_vez=false;
				}else{
					encendido_led=!encendido_led;
					GPIO_PinWrite(1,18,!encendido_led);
					GPIO_PinWrite(1,20,encendido_led);
				}
				flag_timer=false;
			}
   }
}

void TIMER2_IRQHandler(void){
  flag_timer=true;
	LPC_TIM2-> MR0 = 49999999;//500ms (Volvemos a lanzar el timer)
	LPC_TIM2->IR |= (1<<0); //Limpiamos flag de la interrupcion
}

void TIMER0_IRQHandler(void){
	apagar_led();
	LPC_TIM0->IR |= (1<<0); //Limpiamos flag de la interrupcion
}

void WDT_IRQHandler(void){
	//Deshabilitamos las interrupciones del watchdog:
	encendido_led3=!encendido_led3;
	NVIC_DisableIRQ(WDT_IRQn);
	GPIO_PinWrite(1,21,encendido_led3);
	watchdog_Alimentacion(); //Volvemos a alimentar el watchdog
	LPC_WDT->WDMOD &= ~(1<<2); //Limpiamos el flag de la interrupcion
}

void init_joystick( void ){
	GPIO_SetDir(0,17,GPIO_DIR_INPUT); //Posicion abajo del joystick
	PIN_Configure(0,17, PIN_FUNC_0,PIN_PINMODE_PULLDOWN,PIN_PINMODE_NORMAL);
	LPC_GPIOINT->IO0IntEnR=(1<<17);
	NVIC_EnableIRQ(EINT3_IRQn);
}

void EINT3_IRQHandler (void) {
   static uint32_t cuenta;
   if (cuenta++ < 10) {
	   estado = ~estado;
	   GPIO_PinWrite(1,23, estado);
   }
	 LPC_GPIOINT->IO0IntClr |= (1<<17);  //Correccion, si no entraba en el if, no se producia el borrado del flag, y saltaba el watchdog
 }
