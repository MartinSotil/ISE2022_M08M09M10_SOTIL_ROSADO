#define LCD_H
#ifdef LCD_H

#include "GPIO_LPC17xx.h"
#include "SPI_LPC17xx.h"
#include "RTE_Device.h"
#include "RTE_Components.h"
#include "Driver_SPI.h"
#include "stdio.h"
#include <string.h>
#include "RTC.h"

extern unsigned char buffer[128];

//Función para el retardo de 1 us 
void retardoMicro(void);

//Función para el retardo de 1 ms
void retardoMili(void);

//Función para resetear e inicializar el display
void init(void);

//Función para enviar un dato al LCD
void wr_data(unsigned char data);

//Función para enviar un comando al LCD
void wr_cmd(unsigned char cmd);

//Función para fijar el contraste y demás parámetros para 
//el fondo del display, sobre el que luego escribiremos
void  LCD_reset(void);

//Función para copiar algo en el LCD, se debe invocar esta función cada vez que 
//queramos enviar una frase o una letra al display
void copy_to_lcd( uint8_t pagina );

//Función que escribe una letra en la pagina correspondiente del display
int EscribeLetra (uint8_t letra,uint8_t pagina);

//Función para escribir una frase en el display, se debe declarar de la siguiente forma:
//const char frase[] = "mi frase" (lo escribe en la línea 1), es decir, páginas 1 y 2
void EscribeFrase( char Linea1[] );

//Función para escribir una frase en el display, se debe declarar de la siguiente forma:
//const char frase2[] = "mi frase" (lo escribe en la línea 2), es decir, páginas 3 y 4
void EscribeFrase2( char Linea2[] );

//Función para escribir un entero (pasado como parámetro) en el LCD, lo escribe en la línea 1 
//(páginas 1 y 2)
void EscribeEntero(int entero);


//Función para escribir un decimal (pasado como parámetro) en el LCD, lo escribe en la línea 2 
//(páginas 3 y 4)
void EscribeDecimal(float decimal);


//Limpia la pantalla entera
void refrescoPantalla( int linea );

//Escribe una hora en la primera linea
void pintarHoraPrimeraLinea( int hora, int min, int seg );

//Escribe una fecha en la segunda linea
void pintarFechaLinea( int dia, int mes, int ano );


#endif
