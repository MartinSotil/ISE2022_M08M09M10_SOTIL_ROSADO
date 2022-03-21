#ifndef SNTP_H
#define SNTP_H

#include "LPC17xx.h"
#include "RTL.h"
#include "LCD.h"
#include "cmsis_os.h"

 void get_time_ntp (void);
 void time_cback (uint32_t time);


 
 #endif