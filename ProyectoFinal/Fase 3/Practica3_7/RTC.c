#include "RTC.h"
#include "HTTP_Server_CGI.h"
#include "SNTP.h"

int hora_ver;
int min_ver;
int sec_ver;
int dia_ver;
int mes_ver;
int ano_ver;
int ii=0;
bool actualizar_segundo=false;
int contador_alarma=0;

osThreadId id_alarma;
 osThreadId id_pantalla;
osThreadDef(pantalla, osPriorityNormal, 1, 0);
osThreadDef(alarma, osPriorityNormal, 1, 0);

void pantalla_RTC (void ){
  

		get_time( &hora_ver, &min_ver, &sec_ver); //Recogemos la hora actual
		pintarHoraPrimeraLinea(hora_ver,min_ver,sec_ver);
    get_date( &dia_ver,&mes_ver,&ano_ver ); //Recogemos la fecha actual
		pintarFechaLinea( dia_ver,mes_ver,ano_ver );
		
	
}

 void pantalla (void const *arg){
	
	osEvent eleccion_pantalla;
	while(1){
		
		eleccion_pantalla = osSignalWait(0,osWaitForever);
		
		switch(eleccion_pantalla.value.signals){
		
			  case 0x0001:
				EscribeFrase( lcd_text[0] );
				osSignalClear(id_pantalla, 0x0001);
				break;
				
				case 0x0002:
				EscribeFrase2( lcd_text[1] );
				osSignalClear(id_pantalla, 0x0002);
				break;
				
				case 0x0003:
				pantalla_RTC();
				osSignalClear(id_pantalla, 0x0003);
				break;
				
				
   }
  }
 }

	
void alarma (void const *arg){

	osEvent alarma_pulso;
	
	while(1){
	
		alarma_pulso = osSignalWait(0,osWaitForever);
		
		switch(alarma_pulso.value.signals){
		
			case 0x0001:
				
			  contador_alarma++;
			  if(contador_alarma == 3){
					contador_alarma=0;
					osSignalSet(id_SNTP, 0x0004);
				}
				for(ii=0; ii<5; ii++){
					
					GPIO_PinWrite(1,23,1);
					osDelay(500);
					GPIO_PinWrite(1,23,0);
					osDelay(500);
				}
				osSignalClear(id_alarma, 0x0001);
				break;
	  }
	
	}
}



void init_RTC( void ){
	
	LPC_SC->PCONP |= (1<<9); //Alimentar el RTC
	LPC_RTC->ILR &= ~(3<<0); //RTCCIF para limpiar la
	//interrupcion y el bit de alarma
	LPC_RTC->CCR |= (1<<0); //Habilitamos el reloj
	LPC_RTC->CCR &= ~(3<<2); //Por si acaso no estan a cero
	LPC_RTC->CCR &= ~(1<<1); //Se resetea el divisor interno
	//del oscilador hasta que valga 0
  
	id_alarma=osThreadCreate (osThread(alarma), NULL);
	id_pantalla=osThreadCreate (osThread(pantalla), NULL);
	GPIO_SetDir(1,23,GPIO_DIR_OUTPUT);
  NVIC_EnableIRQ( RTC_IRQn );
	//LPC_RTC->CCR |= (1<<4); //Registro de calibracion
}

void habilitar_deshabilitar_RTC( estado_reloj estado ){
	if( estado == HABILITADO ){
		LPC_RTC->CCR |= (1<<0); //Habilitar el reloj
	} else{
		LPC_RTC->CCR &= ~(1<<0); //Deshabilitar el reloj
	}
}

void interrupciones_RTC( tipo_interrupcion tipo ){
	
	int i;
	for( i=0; i<7; i++){
		LPC_RTC->CIIR &= ~(1<<i); //Para comprobar que solo haya una a la vez a 1
	}
	LPC_RTC->CIIR |= (1<<tipo); //Ponemos a 1 la correspondiente
}

void interrupciones_RTC_deshabilitar( void ){
	
	int i;
	for( i=0; i<7; i++){
		LPC_RTC->CIIR &= ~(1<<i); //Para comprobar que solo haya una a la vez a 1
	}
}

void generar_alarma_RTC( tipo_interrupcion tipo ){
	LPC_RTC->AMR = 0xFF; //Para poner a 1 todos los bits del registro
	LPC_RTC->AMR &= ~(1<<tipo); //Ponemos a 0 el bit de la mascara
	//pasado por parametro
}

void get_date( int *dia, int *mes, int *ano){
	*dia= LPC_RTC->DOM;
	*mes= LPC_RTC->MONTH;
	*ano= LPC_RTC->YEAR;
}

void get_time( int *hora, int *minuto, int *segundo){
	*hora= LPC_RTC->HOUR;
	*minuto= LPC_RTC->MIN;
	*segundo= LPC_RTC->SEC;
}

void set_time( tipo_interrupcion tipo, uint8_t valor ){
	switch( tipo ){
		case 	SEGUNDOS:
			LPC_RTC->SEC = valor;
		break;
	  
		case MINUTOS:
			LPC_RTC->MIN = valor;
		break;
		
		case HORA:
			LPC_RTC->HOUR = valor;
		break;
		
		case DIA_MES:
			LPC_RTC->DOM = valor;
		break;
		
		case DIA_SEMANA:
			LPC_RTC->DOW = valor;
		break;
	
		case DIA_ANO:
			LPC_RTC->DOY = valor;
		break;
		
		case MES:
			LPC_RTC->HOUR = valor;
		break;
		
	}
}

void set_alarma( tipo_interrupcion tipo, uint8_t valor ){
	switch( tipo ){
		case 	SEGUNDOS:
			LPC_RTC->ALSEC = valor;
		break;
	  
		case MINUTOS:
			LPC_RTC->ALMIN = valor;
		break;
		
		case HORA:
			LPC_RTC->ALHOUR = valor;
		break;
		
		case DIA_MES:
			LPC_RTC->ALDOM = valor;
		break;
		
		case DIA_SEMANA:
			LPC_RTC->ALDOW = valor;
		break;
	
		case DIA_ANO:
			LPC_RTC->ALDOY = valor;
		break;
		
		case MES:
			LPC_RTC->ALMON = valor;
		break;
		
	}
}

void RTC_IRQHandler( void ){  
  LPC_RTC->ILR|=(1<<0)|(1<<1);
	osSignalSet(id_alarma,0x0001);
}

