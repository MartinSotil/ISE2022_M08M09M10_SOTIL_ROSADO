/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "Esclavo_I2C.h"

int main (void) {
	
	
	Init_i2c();
	
  osKernelInitialize ();                    // initialize CMSIS-RTOS

  Init_Esclavo_I2C();
  
	osKernelStart ();                         // start thread execution 
}
