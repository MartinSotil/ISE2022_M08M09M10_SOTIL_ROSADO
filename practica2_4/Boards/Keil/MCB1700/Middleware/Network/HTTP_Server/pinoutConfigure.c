#include "pinoutConfigure.h"

typedef struct{
	int puerto;
	int pin;
}led;

led leds[4];

uint8_t num_led=0x01;

void pinConfiguration(void){
	
	int i;

	for(i=0;i<4;i++){
	leds[i].puerto=portLeds;
	}
	
  leds[0].pin=pinLed1;
	leds[1].pin=pinLed2;
	leds[2].pin=pinLed3;
	leds[3].pin=pinLed4;
	
	GPIO_SetDir(leds[0].puerto,leds[0].pin,GPIO_DIR_OUTPUT);
	GPIO_SetDir(leds[1].puerto,leds[1].pin,GPIO_DIR_OUTPUT);
	GPIO_SetDir(leds[2].puerto,leds[2].pin,GPIO_DIR_OUTPUT);
	GPIO_SetDir(leds[3].puerto,leds[3].pin,GPIO_DIR_OUTPUT);
	GPIO_PinWrite(leds[0].puerto,leds[0].pin,0);
	GPIO_PinWrite(leds[1].puerto,leds[1].pin,0);
	GPIO_PinWrite(leds[2].puerto,leds[2].pin,0);
	GPIO_PinWrite(leds[3].puerto,leds[3].pin,0);
	
}

void funcionalidadLeds( uint8_t led ){
	if( led==1 ){
		GPIO_PinWrite(portLeds,pinLed1,1);
	}else if( led==2 ){
		GPIO_PinWrite(portLeds,pinLed2,1);
	}else if( led==4 ){
		GPIO_PinWrite(portLeds,pinLed3,1);
	}else if( led==8 ){
		GPIO_PinWrite(portLeds,pinLed4,1);
	}
	
}

int32_t LED_SetOut (uint32_t val) {
  uint32_t n;

  for (n = 0; n < 4; n++) {
    if (val & (1 << n)) LED_On (n);
    else                LED_Off(n);
  }

  return 0;
}

int32_t LED_Off (uint32_t num) {
  int32_t retCode = 0;

  if (num < 4) {
    GPIO_PinWrite (leds[num].puerto, leds[num].pin, 0);
  }
  else {
    retCode = -1;
  }

  return retCode;
}

int32_t LED_On (uint32_t num) {
  int32_t retCode = 0;

  if (num < 4) {
    GPIO_PinWrite (leds[num].puerto, leds[num].pin, 1);
  }
  else {
    retCode = -1;
  }

  return retCode;
}

void barrido_leds( bool barrido ){
	
	if(barrido){
		  osDelay(500);
		  LED_SetOut(num_led);
		  osDelay(500);
		  LED_SetOut(!num_led);
		  num_led=num_led*2;
		  if(num_led>0x08) num_led=0x01;
		}
}
