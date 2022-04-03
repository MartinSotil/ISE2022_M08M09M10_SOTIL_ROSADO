#ifndef GESTION_FLASH_H
#define GESTION_FLASH_H
#include "lpc17xx_iap.h"

#define NUM_MAX_POS 12

void borrar_sector( int numSector );
void direccionSector( uint8_t numSector, int *inicioMemoria, int *finMemoria );
void escribirMemoria( int sector,uint32_t flashStart, uint8_t buffer[] );
uint8_t leerFlash(  uint32_t pos );
void leerFlashxPosicionesyRetornarArray( uint32_t posOrigen, uint8_t buffer[] );
//void escribirMemoria( uint32_t flashStart, uint8_t buffer[] );
void modificarXposicion( int sector,uint32_t posicionInicialSector, uint8_t valorNuevo,int posicionCambiar );

#endif
