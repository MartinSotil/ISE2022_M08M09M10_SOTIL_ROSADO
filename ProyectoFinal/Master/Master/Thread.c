
#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "Driver_I2C.h"

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 

#define LM75B_I2C_ADDR       	0x28      
 
 
#define Esclavo_I2C_ADDR       	      0x28      
 
 
#define REG_GANANCIA_1   					    				0x01
#define REG_GANANCIA_5   					    				0x02
#define REG_GANANCIA_10    					  				0x03
#define REG_GANANCIA_50    					  				0x04
#define REG_GANANCIA_100    									0x05


#define REG_PROGRAMAR_UMBRAL	    						0x06
#define REG_HABILITAR_UMBRAL	    						0x07
#define REG_DESHABILITAR_UMBRAL   						0x08
#define REG_PETICION_GANANCIA	    						0x09
#define REG_PETICION_OVERLOAD	        				0x0A
#define REG_HABILITAR_INTERRUPCIONES	    		0x0B
#define REG_DESHABILITAR_INTERRUPCIONES	   		0x0C
#define REG_PETICION_ESTADO_OVERLOAD		   		0x0D

#define SIG_TEMP							0x0001

uint32_t addr = LM75B_I2C_ADDR;
int32_t status = 0, nlect = 0;
uint8_t cmd, buf[10];
uint16_t temp;
float temperatura;


/* I2C driver instance */
extern ARM_DRIVER_I2C            Driver_I2C2;
static ARM_DRIVER_I2C *I2Cdrv = &Driver_I2C2;


static volatile uint32_t I2C_Event;


void Thread (void const *argument);                             // thread function
osThreadId tid_Thread;                                          // thread id
osThreadDef (Thread, osPriorityNormal, 1, 0);                   // thread object
 
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
		
		osSignalSet (tid_Thread, SIG_TEMP);

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
  status = I2Cdrv->Control      (ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_STANDARD);
  status = I2Cdrv->Control      (ARM_I2C_BUS_CLEAR, 0);
	
}

int Init_Thread (void) {

  tid_Thread = osThreadCreate (osThread(Thread), NULL);
  if (!tid_Thread) return(-1);
  
  return(0);
}

void enviar_comando(uint8_t comando){
	osSignalSet();
	cmd=comando;
}

void Thread (void const *argument) {

  while (1) {
    osDelay(2000);
		
		
		
		
		
		
		cmd = REG_TEMP;
		status = I2Cdrv->MasterTransmit(addr, &cmd, 1, true);
		osSignalWait (SIG_TEMP, osWaitForever); 
		
	
		
		status = I2Cdrv->MasterReceive(addr, buf, 2, true);
		osSignalWait (SIG_TEMP, osWaitForever); 
		

		
		temp = ((buf[0] << 8) | buf[1]) >> 5;
	
		temperatura = temp * 0.125;
		nlect++;
		
				
    osThreadYield ();                                           // suspend thread
  }
}
