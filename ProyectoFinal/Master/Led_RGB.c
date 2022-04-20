#include "Led_RGB.h"

void Init_RGB(void){
	GPIO_SetDir(2, 3,   GPIO_DIR_OUTPUT); //Led rojo
	GPIO_SetDir(2, 2, GPIO_DIR_OUTPUT); //Led verde
	GPIO_SetDir(2, 1,  GPIO_DIR_OUTPUT);//Led azul
	GPIO_PinWrite(2,3,1);
  GPIO_PinWrite(2,2,1);
  GPIO_PinWrite(2,1,1);
}
//Activos a nivel bajo
void led_verde(void){
  GPIO_PinWrite(2,3,1);
  GPIO_PinWrite(2,1,1);
  GPIO_PinWrite(2,2,0);
}
void led_rojo(void){
  GPIO_PinWrite(2,3,0);
  GPIO_PinWrite(2,2,1);
  GPIO_PinWrite(2,1,1);
}
void apagar_led(void){
	GPIO_PinWrite(2,3,1);
  GPIO_PinWrite(2,2,1);
  GPIO_PinWrite(2,1,1);
}
void led_azul(void){
  GPIO_PinWrite(2,3,1);
  GPIO_PinWrite(2,1,0);
  GPIO_PinWrite(2,2,1);
}