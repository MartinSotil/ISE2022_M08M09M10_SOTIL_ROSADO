#include "gestion_FLASH.h"

uint8_t *destino;
int i;

void borrar_sector( int numSector ){
	
	IAP_STATUS_CODE status;
	uint32_t numAux;
	numAux= (uint32_t)numSector;
	
	status=EraseSector( numAux,(numAux+0x00000001) );
	
	if(status != CMD_SUCCESS) while(1);
}


void escribirMemoria( int sector,uint32_t flashStart, uint8_t buffer[] ){
	IAP_STATUS_CODE status;
	uint8_t* destino;
	uint8_t buffer_aux[256];
	
	destino=(uint8_t*)(flashStart);
	for(i=0;i<256;i++){
		if(i<NUM_MAX_POS){
			buffer_aux[i]=buffer[i];
		}else{
			buffer_aux[i]=0xFF;
		}
	}
	borrar_sector(sector);
  status=CopyRAM2Flash( destino, buffer_aux , IAP_WRITE_256 );
	
	if(status != CMD_SUCCESS){
		while(1);
	}
}

uint8_t leerFlash(  uint32_t pos ){
	uint8_t lectura;
	uint8_t *posicion;
	posicion=(uint8_t*)(pos);
	lectura=*posicion;
	return lectura;
}

void leerFlashxPosicionesyRetornarArray( uint32_t posOrigen, uint8_t buffer[] ){
	int i;
	for(i=0;i<NUM_MAX_POS;i++){
		buffer[i]=leerFlash(posOrigen+i);
	}
}

void modificarXposicion( int sector,uint32_t posicionInicialSector, uint8_t valorNuevo,int posicionCambiar ){
	  uint8_t copiaFlash[NUM_MAX_POS];
	  if( posicionCambiar> NUM_MAX_POS){
		  while(1);
	  }
	  leerFlashxPosicionesyRetornarArray(posicionInicialSector,copiaFlash);
	  copiaFlash[posicionCambiar-1]=valorNuevo;
	  escribirMemoria( sector,posicionInicialSector, copiaFlash);
}


/*void escribirXposicionesFLASH( uint8_t buffer[], int posicionInicio, int numSector,int numPosiciones ){
	
	IAP_STATUS_CODE status;
	uint8_t *destino;
	uint32_t inicionMemoria;
	inicionMemoria=(uint32_t)numSector;
	int i;
	
	uint8_t bufferMemoria[ 256 ];
	for( i=0;i<256;i++ ){
		if(i<posicionInicio){
			bufferMemoria[i]=0xff;
		}else if( (i>posicionInicio) && (i<(posicionInicio+numPosiciones+1))){
			bufferMemoria[i]=buffer[
		}
	}
	destino= (uint8_t*)(inicionMemoria + i*BUFF_SIZE);
	
	status=CopyRAM2Flash( &destino, uint8_t* source, IAP_WRITE_SIZE size);
	
	if(status != CMD_SUCCESS) while(1);
}*/

/*void direccionSector( uint8_t numSector, int *ini

cioMemoria, int *finMemoria ){
	*inicioMemoria=FLASH_PROG_AREA_START+(tam_sector*numSector);
	*finMemoria=FLASH_PROG_AREA_START+(tam_sector*(numSector+1))-1;
}*/
