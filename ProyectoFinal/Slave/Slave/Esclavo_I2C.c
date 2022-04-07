
#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "Driver_I2C.h"
#include "Esclavo_I2C.h"

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 



uint32_t addr = Esclavo_I2C_ADDR;
int32_t status = 0, nlect = 0;
uint8_t cmd, buffer_esclavo[10];
float temperatura;
uint8_t Overload_actual;
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

  tid_Esclavo_I2C = osThreadCreate (osThread(Esclavo_I2C), NULL);
  if (!tid_Esclavo_I2C) return(-1);
  
  return(0);
}

void Esclavo_I2C  (void const *argument) {
	 
	
  while (1) {
    osDelay(2000);
   
		
    I2Cdrv->SlaveReceive(buffer_esclavo,1);
    osSignalWait (SIG_Esclavo_I2C	, osWaitForever);
		
		if(buffer_esclavo[0]==REG_GANANCIA_1){ 
				
				ganancia_actual=REG_GANANCIA_1;
			
		}else if(buffer_esclavo[0]==REG_GANANCIA_5){
			
				ganancia_actual=REG_GANANCIA_5;
			
		}else if(buffer_esclavo[0]==REG_GANANCIA_10){
			
				ganancia_actual=REG_GANANCIA_10;
			
		}else if(buffer_esclavo[0]==REG_GANANCIA_50){
		
				ganancia_actual=REG_GANANCIA_50;
			
		}else if(buffer_esclavo[0]==REG_GANANCIA_100){
			
				ganancia_actual=REG_GANANCIA_100;
			
		}else if(buffer_esclavo[0]==REG_HABILITAR_UMBRAL){
      
      EstadoOverload_Actual=true;
		
		}else if(buffer_esclavo[0]==REG_DESHABILITAR_UMBRAL){
      
      EstadoOverload_Actual=false;
			
		}else if(buffer_esclavo[0]==REG_HABILITAR_INTERRUPCIONES){
		
		}else if(buffer_esclavo[0]==REG_DESHABILITAR_INTERRUPCIONES){
		
		}else if(buffer_esclavo[0]==REG_PROGRAMAR_UMBRAL){
		
			Overload_actual=buffer_esclavo[1];
			
		}else if(buffer_esclavo[0]==REG_PETICION_GANANCIA	 ){
			
			enviar_data=true;
			cmd=ganancia_actual;
			
		}else if(buffer_esclavo[0]==REG_PETICION_OVERLOAD	 ){
			
			enviar_data=true;
			cmd=Overload_actual;
		
	
		}else if(buffer_esclavo[0]==REG_PETICION_ESTADO_OVERLOAD	 ){
			
			enviar_data=true;
			cmd=EstadoOverload_Actual;
		
	
		}
		
		
		
		
		if(enviar_data){
			status = I2Cdrv->SlaveTransmit(&cmd,1);
			osSignalWait (SIG_Esclavo_I2C	, osWaitForever);  
			enviar_data=false; 
		}

		

		nlect++;
		
				
    osThreadYield ();                                           // suspend thread
  }
} 