#include "Esclavo_I2C.h"

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 



uint32_t addr = Esclavo_I2C_ADDR;
int32_t status = 0, contador = 0;
uint8_t cmd, buffer_esclavo[10];
float temperatura;
uint8_t Overload_actual;
bool estado_led=false;
bool enviar_data=false;
uint8_t ganancia_actual= 0x01;
uint8_t EstadoOverload_Actual=0x00;

/* I2C driver instance */
extern ARM_DRIVER_I2C            Driver_I2C2;
static ARM_DRIVER_I2C *I2Cdrv = &Driver_I2C2;


static volatile uint32_t I2C_Event;


void Esclavo_I2C (void const *argument);                             // thread function
osThreadId tid_Esclavo_I2C;                                          // thread id
osThreadDef (Esclavo_I2C, osPriorityNormal, 1, 0);                   // thread object
 
void Interrupcion_led (void const *argument);                             // thread function
osThreadId tid_int_led;                                          // thread id
osThreadDef (Interrupcion_led, osPriorityNormal, 1, 0);                   // thread object
/* I2C Signal Event function callback */
void I2C_SignalEvent (uint32_t event) {
 
  /* Save received events */
  I2C_Event |= event;
 
  /* Optionally, user can define specific actions for an event */
 
  if (event & ARM_I2C_EVENT_TRANSFER_INCOMPLETE) {
    /* Less data was transferred than requested */
  }
 
  if (event & ARM_I2C_EVENT_TRANSFER_DONE) {
    /* Transfer or receive is finished */
		
		osSignalSet (tid_Esclavo_I2C, SIG_Esclavo_I2C);

  }
 
  if (event & ARM_I2C_EVENT_ADDRESS_NACK) {
    /* Slave address was not acknowledged */
  }
 
  if (event & ARM_I2C_EVENT_ARBITRATION_LOST) {
    /* Master lost bus arbitration */
  }
 
  if (event & ARM_I2C_EVENT_BUS_ERROR) {
    /* Invalid start/stop position detected */
  }
 
  if (event & ARM_I2C_EVENT_BUS_CLEAR) {
    /* Bus clear operation completed */
  }
 
  if (event & ARM_I2C_EVENT_GENERAL_CALL) {
    /* Slave was addressed with a general call address */
  }
 
  if (event & ARM_I2C_EVENT_SLAVE_RECEIVE) {
     
    /* Slave addressed as receiver but SlaveReceive operation is not started */
  }
 
  if (event & ARM_I2C_EVENT_SLAVE_TRANSMIT) {
    /* Slave addressed as transmitter but SlaveTransmit operation is not started */
  }
}

void Init_i2c(void){
	
	int32_t status;
	
  status = I2Cdrv->Initialize (I2C_SignalEvent);
  status = I2Cdrv->PowerControl (ARM_POWER_FULL);
  I2Cdrv->Control(ARM_I2C_OWN_ADDRESS, Esclavo_I2C_ADDR );
	
}

int Init_Esclavo_I2C(void) {
  GPIO_SetDir(0,23,GPIO_DIR_OUTPUT);
  tid_Esclavo_I2C = osThreadCreate (osThread(Esclavo_I2C), NULL);
	tid_int_led=osThreadCreate (osThread(Interrupcion_led), NULL);
  if (!tid_Esclavo_I2C) return(-1);
	if (!tid_int_led) return(-1);
  
  return(0);
}

void Interrupcion_led(void const *argument){
	while(1){
		osSignalWait (0x02	, osWaitForever);
		estado_led=((~cmd)&0x04);
	  GPIO_PinWrite(0,23,0);
		osDelay(10);
		GPIO_PinWrite(0,23,estado_led);
		osDelay(10);
		GPIO_PinWrite(0,23,0);
		osSignalClear(tid_int_led,0x02);
	}
}

void Esclavo_I2C  (void const *argument) {
	 
	
  while (1) {
    //osDelay(2000);
    I2Cdrv->SlaveReceive(buffer_esclavo,1);
    osSignalWait (SIG_Esclavo_I2C	, osWaitForever);
		//analizador_tramas(buffer_esclavo[0]);																	
		//Complemento a 1 del dato recibido:		
		cmd=~buffer_esclavo[0];
		osSignalSet(tid_int_led,0x02);
		enviar_data=true; //Para siempre responder en este caso
		//Para retransmitir el dato si hace falta
		if(enviar_data){
			status = I2Cdrv->SlaveTransmit(&cmd,1);
			osSignalWait (SIG_Esclavo_I2C	, osWaitForever);  
			enviar_data=false; 
		}
		
		contador++;
		
				
    osThreadYield ();                                           // suspend thread
  }
} 

void analizador_tramas(uint8_t trama){
	    switch(trama){
			case REG_GANANCIA_1:
				ganancia_actual=REG_GANANCIA_1;
			break;
			
		  case REG_GANANCIA_5:
			  ganancia_actual=REG_GANANCIA_5;
			break;
						
			case REG_GANANCIA_10:
				ganancia_actual=REG_GANANCIA_10;
			break;
									
			case REG_GANANCIA_50:
				ganancia_actual=REG_GANANCIA_50;
			break;
												
			case REG_GANANCIA_100:
				ganancia_actual=REG_GANANCIA_100;
			break;
															
		  case REG_HABILITAR_UMBRAL:
				EstadoOverload_Actual=true;
			break;
																		
			case REG_HABILITAR_INTERRUPCIONES:
				
			break;
			
			case REG_DESHABILITAR_INTERRUPCIONES:
				
			break;
															
		  case REG_PROGRAMAR_UMBRAL:
				Overload_actual=buffer_esclavo[1];
			break;
																		
			case REG_PETICION_GANANCIA:
				enviar_data=true;
			  cmd=ganancia_actual;
			break;
		  
			case REG_PETICION_OVERLOAD:
				enviar_data=true;
			  cmd=Overload_actual;
			break;
																		
			case REG_PETICION_ESTADO_OVERLOAD:
				enviar_data=true;
			  cmd=EstadoOverload_Actual;
			break;
		}
}