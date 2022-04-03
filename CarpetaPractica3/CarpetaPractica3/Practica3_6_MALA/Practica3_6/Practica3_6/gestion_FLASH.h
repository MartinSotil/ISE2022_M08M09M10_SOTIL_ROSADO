#ifndef GESTION_FLASH_H
#define GESTION_FLASH_H
#include "lpc17xx_iap.h"

#define DIR_INICIO 

void borrar_sector( int numSector );
void direccionSector( uint8_t numSector, int *inicioMemoria, int *finMemoria );
void escribirMemoria( int sector,uint32_t startSector, uint8_t buffer[],int tamano );
void leerFlash(  uint32_t pos,uint8_t *lectura );
void leerFlashxPosicionesyRetornarArray( uint32_t posOrigen, int numPosiciones, uint8_t buffer[] );
//void escribirMemoria2( int sector, uint8_t buffer[] );
void modificarXposicion( int sector,uint32_t posicionInicialSector, uint8_t valorNuevo,int posicionCambiar );

#endif
