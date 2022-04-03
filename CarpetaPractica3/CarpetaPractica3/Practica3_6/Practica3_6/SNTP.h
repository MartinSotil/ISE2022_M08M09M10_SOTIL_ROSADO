#ifndef SNTP_H
#define SNTP_H

#include "LPC17xx.h"
#include "HTTP_Server_CGI.h"
#include "LCD.h"
#include "cmsis_os.h"
#include "RTC.h"
#include "rl_net.h" //Para el servidor sntp
#include <time.h> //Para crear la estructura y que nos 
//cambie en horas, minutos y segundos, los segundos desde
//el 1 de enero de 1990 (porque usamos otro servidor)

 extern uint32_t tiempo_sntp;
 
  void Servidor_SNTP (void const *arg);
 extern osThreadId id_SNTP;

 
 void get_time_ntp (void);
 void time_cback (uint32_t time);
 void reloj_sntp( uint32_t tiempo_segundos );
 void init_SNTP(void);
 
 #endif
 