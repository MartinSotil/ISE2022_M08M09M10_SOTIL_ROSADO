#include "LCD.h"
#include "SNTP.h"
#include "gestor_pulsador.h"

#define DOWN                  17
#define LEFT                  15
#define CENTER                16
#define UP                    23
#define RIGHT                 24


void falling (void const *argument);
void rising (void const *argument);

osThreadId tid_rising; 
osThreadId tid_falling;

osThreadDef (rising, osPriorityNormal, 1, 0);   
osThreadDef (falling, osPriorityNormal, 1, 0); 


void Init_pulsador(void) {
	
	 PIN_Configure(0,LEFT,PIN_FUNC_0,PIN_PINMODE_PULLDOWN,PIN_PINMODE_NORMAL);          
	 PIN_Configure(0,CENTER,PIN_FUNC_0,PIN_PINMODE_PULLDOWN,PIN_PINMODE_NORMAL);
	 PIN_Configure(0,DOWN,PIN_FUNC_0,PIN_PINMODE_PULLDOWN,PIN_PINMODE_NORMAL);
	 PIN_Configure(0,UP,PIN_FUNC_0,PIN_PINMODE_PULLDOWN,PIN_PINMODE_NORMAL);
	 PIN_Configure(0,RIGHT,PIN_FUNC_0,PIN_PINMODE_PULLDOWN,PIN_PINMODE_NORMAL);

	LPC_GPIOINT->IO0IntEnR |= ( 1 << LEFT)|( 1 << UP) |( 1 << CENTER)|( 1 << DOWN)|( 1 << RIGHT);
	tid_rising = osThreadCreate (osThread(rising), NULL);
  
	tid_falling = osThreadCreate (osThread(falling), NULL);
  
	
	LPC_GPIOINT->IO0IntClr |= ( 1 << LEFT);
	LPC_GPIOINT->IO0IntClr |= ( 1 << DOWN);
  LPC_GPIOINT->IO0IntClr |= ( 1 << UP);
	LPC_GPIOINT->IO0IntClr |= ( 1 << CENTER);
	LPC_GPIOINT->IO0IntClr |= ( 1 << RIGHT);
	NVIC_EnableIRQ(EINT3_IRQn);


}

void rising (void const *argument) {

	osEvent pulso_rise;

	while(1){
	pulso_rise = osSignalWait(0,osWaitForever); //El 0 hace que se pare
  switch(pulso_rise.value.signals){
     case 0x0001:
			  osDelay(100); //Eliminamos los rebotes
				LPC_GPIOINT->IO0IntEnF |= ( 1 << LEFT)|( 1 << UP) |( 1 << CENTER)|( 1 << DOWN)|( 1 << RIGHT);	//Activamos las de bajada
				osSignalClear(tid_rising,0x0001);
		break;
  }
 }
}
void falling (void const *argument) {

	osEvent pulso_fall;

	while(1){
	pulso_fall = osSignalWait(0,osWaitForever); //El 0 hace que se pare
  switch(pulso_fall.value.signals){
     case 0x0001:
				osDelay(100); //Eliminamos rebotes
		    LPC_GPIOINT->IO0IntEnR |= ( 1 << LEFT)|( 1 << UP) |( 1 << CENTER)|( 1 << DOWN)|( 1 << RIGHT); //Activamos las de subida
				osSignalClear(tid_falling,0x0001);
		break;
  }
 }
}

void EINT3_IRQHandler (void){    
	 
	  //*********************************************
	  //FLANCO SUBIDA
	  //*********************************************
		if(LPC_GPIOINT->IO0IntStatR&(1 << UP)){    
	  	
			

			osSignalSet(tid_rising,0x0001);	                       
			LPC_GPIOINT->IO0IntEnR &= ~( 1 << LEFT)|~( 1 << UP) |~( 1 << CENTER)|~( 1 << DOWN)|~( 1 << RIGHT);
		  LPC_GPIOINT->IO0IntClr =( 1 << UP);
	  }
		if(LPC_GPIOINT->IO0IntStatR&(1 << DOWN)){   
	  	

			osSignalSet(tid_rising,0x0001);	                      
			LPC_GPIOINT->IO0IntEnR &= ~( 1 << LEFT)|~( 1 << UP) |~( 1 << CENTER)|~( 1 << DOWN)|~( 1 << RIGHT);
		  LPC_GPIOINT->IO0IntClr =( 1 << DOWN);
		
	  }
		if(LPC_GPIOINT->IO0IntStatR&(1 << LEFT)){   
	  	
      
			
			osSignalSet(tid_rising,0x0001);	                       
			LPC_GPIOINT->IO0IntEnR &= ~( 1 << LEFT)|~( 1 << UP) |~( 1 << CENTER)|~( 1 << DOWN)|~( 1 << RIGHT);
	    LPC_GPIOINT->IO0IntClr =( 1 << LEFT);
		
	  }
		if(LPC_GPIOINT->IO0IntStatR&(1 << RIGHT)){  
	  	

			
			osSignalSet(tid_rising,0x0001);	                       
			LPC_GPIOINT->IO0IntEnR &= ~( 1 << LEFT)|~( 1 << UP) |~( 1 << CENTER)|~( 1 << DOWN)|~( 1 << RIGHT);
	    LPC_GPIOINT->IO0IntClr =( 1 << RIGHT);
		
	  }
		if(LPC_GPIOINT->IO0IntStatR&(1 << CENTER)){   
	  	
      LPC_RTC->SEC=0;
			LPC_RTC->MIN = 0;
			LPC_RTC->HOUR = 0;
			LPC_RTC->DOM = 1;
			LPC_RTC->MONTH = 1;
			LPC_RTC->YEAR = 2000;
			osSignalSet(id_pantalla,0x0003);
			
			osSignalSet(tid_rising,0x0001);
			LPC_GPIOINT->IO0IntEnR &= ~( 1 << LEFT)|~( 1 << UP) |~( 1 << CENTER)|~( 1 << DOWN)|~( 1 << RIGHT);
	    LPC_GPIOINT->IO0IntClr =( 1 << CENTER);
      
	  }
		
		
    //*********************************************************************
		//FLANCO BAJADA
		//*********************************************************************
		if( LPC_GPIOINT->IO0IntStatF&(1 << UP)){   
			osSignalSet(tid_falling,0x0001);	
      LPC_GPIOINT->IO0IntEnF &= ~( 1 << LEFT)|~( 1 << UP) |~( 1 << CENTER)|~( 1 << DOWN)|~( 1 << RIGHT);
      LPC_GPIOINT->IO0IntClr =( 1 << UP);				
	  } 
		if( LPC_GPIOINT->IO0IntStatF&(1 << DOWN)){   	   
			osSignalSet(tid_falling,0x0001);
      LPC_GPIOINT->IO0IntEnF &= ~( 1 << LEFT)|~( 1 << UP) |~( 1 << CENTER)|~( 1 << DOWN)|~( 1 << RIGHT);			
      LPC_GPIOINT->IO0IntClr =( 1 << DOWN);			
	  }
		if( LPC_GPIOINT->IO0IntStatF&(1 << LEFT)){    	   
			osSignalSet(tid_falling,0x0001);
      LPC_GPIOINT->IO0IntEnF &= ~( 1 << LEFT)|~( 1 << UP) |~( 1 << CENTER)|~( 1 << DOWN)|~( 1 << RIGHT);			
      LPC_GPIOINT->IO0IntClr =( 1 << LEFT);			
	  }
		if( LPC_GPIOINT->IO0IntStatF&(1 << RIGHT)){       
			osSignalSet(tid_falling,0x0001);	
			LPC_GPIOINT->IO0IntEnF &= ~( 1 << LEFT)|~( 1 << UP) |~( 1 << CENTER)|~( 1 << DOWN)|~( 1 << RIGHT);
      LPC_GPIOINT->IO0IntClr =( 1 << RIGHT);			
	  }
		if( LPC_GPIOINT->IO0IntStatF&(1 << CENTER)){       
			osSignalSet(tid_falling,0x0001);
			LPC_GPIOINT->IO0IntEnF &= ~( 1 << LEFT)|~( 1 << UP) |~( 1 << CENTER)|~( 1 << DOWN)|~( 1 << RIGHT);
      LPC_GPIOINT->IO0IntClr =( 1 << CENTER);			
	  }
		
}
	
