#ifndef GESTION_FLASH_H
#define GESTION_FLASH_H
#include "lpc17xx_iap.h"

//int POSICIONEMPEZAR=0x00000000;
//int tam_sector=1000;

void borrar_sector( uint8_t numSector );
//void direccionSector( uint8_t numSector, int *inicioMemoria, int *finMemoria );
void borrarSector( uint32_t Memoria_Inicio, uint32_t Memoria_final);

#endif
