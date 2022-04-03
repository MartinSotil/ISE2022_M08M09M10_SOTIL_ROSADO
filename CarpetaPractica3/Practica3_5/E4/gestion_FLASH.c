#include "gestion_FLASH.h"

void borrar_sector( uint8_t numSector ){
	
	
}

/*void direccionSector( uint8_t numSector, int *inicioMemoria, int *finMemoria ){
	*inicioMemoria=POSICIONEMPEZAR+(tam_sector*numSector);
	*finMemoria=POSICIONEMPEZAR+(tam_sector*(numSector+1))-1;
}*/

/*void escribirMemoriaFlash(){
  
}*/


void borrarSector( uint32_t Memoria_Inicio, uint32_t Memoria_final){
  
  IAP_STATUS_CODE status;
  
  status=EraseSector(0x00000008, 0x00000009 );
  
  if(status != CMD_SUCCESS)
  {

     while(1); 
  }

}