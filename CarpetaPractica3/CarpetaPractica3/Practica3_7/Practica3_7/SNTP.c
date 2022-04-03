#include "SNTP.h"

const uint8_t ntp_server[2][4] = {{150,214,94,10},{150,214,94,5}}; //Dos servidores sntp distintos

uint32_t tiempo_sntp;

osThreadId id_SNTP;

osThreadDef(Servidor_SNTP, osPriorityNormal, 1, 0);

bool SNTP_Choose_IP;


void init_SNTP(void){
	
		id_SNTP=osThreadCreate(osThread(Servidor_SNTP), NULL);
	
}

 void Servidor_SNTP (void const *arg) {
  osEvent sntp_pulso;
	
	while(1){
	
		sntp_pulso = osSignalWait(0,osWaitForever);
		
		switch(sntp_pulso.value.signals){
		
			  case 0x0004: //Senal para sincronizar SNTP
				get_time_ntp();
				osSignalClear(id_SNTP, 0x0004);
				break;
   }
	}
}



void get_time_ntp(void) {
	
 
  if (sntp_get_time(NULL,time_cback) == netOK) {
		osDelay(100);
  }
  else {
    habilitar_deshabilitar_RTC( HABILITADO );
  }
}



void reloj_sntp( uint32_t tiempo_segundos ){
	int s;
	int m;
	int h;
	int D;
	int M;
	int A;
	struct tm ts;
	time_t rawtime = tiempo_segundos;
	ts = *localtime(&rawtime);
	
	h= ts.tm_hour + 2; //Porque estamos en horario de verano
	if (h>23) h=0; //Para corregir la suma que hemos hecho antes
	m=ts.tm_min;
	s=ts.tm_sec;
	D=ts.tm_mday;
	M=ts.tm_mon + 1;
	A=ts.tm_year + 1900; //Porque te da el año actual "-1900"
  
	//Ponemos el reloj en hora:
	LPC_RTC->SEC=s;
	LPC_RTC->MIN = m;
	LPC_RTC->HOUR = h;
	LPC_RTC->DOM = D;
	LPC_RTC->MONTH = M;
	LPC_RTC->YEAR = A;
	osSignalSet(id_pantalla,0x0003);
}
 
void time_cback (uint32_t time) {
	
	osDelay(100);
  if (time == 0) {
    //habilitar_deshabilitar_RTC( DESHABILITADO ); //Deshabilitamos el RTC
  }
  else {
		reloj_sntp(time);
		tiempo_sntp=time;
  }
}
