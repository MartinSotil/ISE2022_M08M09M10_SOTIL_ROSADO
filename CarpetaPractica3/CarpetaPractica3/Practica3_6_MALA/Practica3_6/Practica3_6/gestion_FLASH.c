#include "gestion_FLASH.h"

void borrar_sector( int numSector ){
	
	IAP_STATUS_CODE status;
	uint32_t numAux;
	numAux= (uint32_t)numSector;
	
	status=EraseSector( numAux,(numAux+0x00000001) );
	
	if(status != CMD_SUCCESS) while(1);
}


void escribirMemoria( int sector,uint32_t startSector, uint8_t buffer[],int tamano ){
	IAP_STATUS_CODE status;
	uint32_t numAux;
	uint8_t *destino;
	int i;
	uint8_t buffer_aux[256];
	destino= (uint8_t*)(0x000+startSector);
	
	
	for(i=0;i<256;i++){
		if(i<tamano){
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

/*void escribirMemoria2( int sector, uint8_t buffer[] ){
	IAP_STATUS_CODE status;
	uint32_t numAux;
	uint8_t *destino;
	numAux= (uint32_t)(sector*0x00001000);
	destino= (uint8_t*)(0x40000);
	
  status=CopyRAM2Flash( destino, buffer , IAP_WRITE_256 );
	
	if(status != CMD_SUCCESS){
		while(1);
	}
}*/

void leerFlash(  uint32_t pos,uint8_t *lectura ){
	uint8_t *posicion;
	posicion=(uint8_t*)(0x000+pos);
	*lectura=(uint8_t)*posicion;
}

void leerFlashxPosicionesyRetornarArray( uint32_t posOrigen, int numPosiciones, uint8_t buffer[] ){
	int i;
	uint8_t retorno;
	for(i=0;i<numPosiciones;i++){
		leerFlash(posOrigen+i,&retorno);
		buffer[i]=retorno;
	}
}

void modificarXposicion( int sector,uint32_t posicionInicialSector, uint8_t valorNuevo,int posicionCambiar ){
	/*uint8_t copiaFlash[256];
	leerFlashxPosicionesyRetornarArray( posicionInicialSector,256,copiaFlash);
  copiaFlash[posicionCambiar]=valorNuevo;
	escribirMemoria2(8,copiaFlash);*/
	  uint8_t copiaFlash[256];
	  int i;
	  uint8_t retorno;
	 for(i=0;i<256;i++){
		 if(i==(posicionCambiar-1)){
				copiaFlash[i]=valorNuevo;
		 }else{
				leerFlash(posicionInicialSector+i,&retorno);
			  copiaFlash[i]=retorno;
		 }
	 }
	  borrar_sector( sector );
	  escribirMemoria( sector,posicionInicialSector, copiaFlash,256);
		
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
