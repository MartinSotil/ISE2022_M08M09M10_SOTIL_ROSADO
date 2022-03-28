#ifndef RTC_H
#define RTC_H

#include "LPC17xx.h"
#include "LCD.h"
#include "cmsis_os.h"
#include "HTTP_Server_CGI.h"

typedef enum{ 
	HABILITADO=0, 
  DESHABILITADO=1     
}estado_reloj;

typedef enum{
	SEGUNDOS=0,
	MINUTOS=1,
	HORA=2,
	DIA_MES=3,
	DIA_SEMANA=4,
	DIA_ANO=5,
	MES=6
} tipo_interrupcion;

extern int hora_ver;
extern int min_ver;
extern int sec_ver;
extern int dia_ver;
extern int mes_ver;
extern int ano_ver;

extern bool actualizar_segundo;


void alarma (void const *arg);
 void pantalla (void const *arg);

extern osThreadId id_alarma;
extern osThreadId id_pantalla;


void pantalla_RTC (void );

void init_RTC( void );



void habilitar_deshabilitar_RTC( estado_reloj estado );

void interrupciones_RTC( tipo_interrupcion tipo );

void generar_alarma_RTC( tipo_interrupcion tipo );

void get_date( int *dia, int *mes, int *ano);

void get_time( int *hora, int *minuto, int *segundo);

void set_time( tipo_interrupcion tipo, uint8_t valor );

void set_alarma( tipo_interrupcion tipo, uint8_t valor );

void interrupciones_RTC_deshabilitar( void );

#endif
