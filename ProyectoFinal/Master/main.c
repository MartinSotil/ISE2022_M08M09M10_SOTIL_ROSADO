/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "Maestro_I2C.h"

/*
 * main: initialize and start the system
 */
int main (void) {
	
	
	Init_i2c();
	Init_RGB();
	
  osKernelInitialize ();                    // initialize CMSIS-RTOS

	Init_Maestro_I2C();

  osKernelStart ();                         // start thread execution 
}
