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

//Funci�n para el retardo de 1 us 
void retardoMicro(void);

//Funci�n para el retardo de 1 ms
void retardoMili(void);

//Funci�n para resetear e inicializar el display
void init(void);

//Funci�n para enviar un dato al LCD
void wr_data(unsigned char data);

//Funci�n para enviar un comando al LCD
void wr_cmd(unsigned char cmd);

//Funci�n para fijar el contraste y dem�s par�metros para 
//el fondo del display, sobre el que luego escribiremos
void  LCD_reset(void);

//Funci�n para copiar algo en el LCD, se debe invocar esta funci�n cada vez que 
//queramos enviar una frase o una letra al display
void copy_to_lcd( uint8_t pagina );

//Funci�n que escribe una letra en la pagina correspondiente del display
int EscribeLetra (uint8_t letra,uint8_t pagina);

//Funci�n para escribir una frase en el display, se debe declarar de la siguiente forma:
//const char frase[] = "mi frase" (lo escribe en la l�nea 1), es decir, p�ginas 1 y 2
void EscribeFrase( char Linea1[] );

//Funci�n para escribir una frase en el display, se debe declarar de la siguiente forma:
//const char frase2[] = "mi frase" (lo escribe en la l�nea 2), es decir, p�ginas 3 y 4
void EscribeFrase2( char Linea2[] );

//Funci�n para escribir un entero (pasado como par�metro) en el LCD, lo escribe en la l�nea 1 
//(p�ginas 1 y 2)
void EscribeEntero(int entero);


//Funci�n para escribir un decimal (pasado como par�metro) en el LCD, lo escribe en la l�nea 2 
//(p�ginas 3 y 4)
void EscribeDecimal(float decimal);


//Limpia la pantalla entera
void refrescoPantalla( int linea );

//Escribe una hora en la primera linea
void pintarHoraPrimeraLinea( int hora, int min, int seg );

//Escribe una fecha en la segunda linea
void pintarFechaLinea( int dia, int mes, int ano );


#endif
