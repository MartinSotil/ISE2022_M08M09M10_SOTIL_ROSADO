/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network:Service
 * Copyright (c) 2004-2014 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    HTTP_Server_CGI.c
 * Purpose: HTTP Server CGI Module
 * Rev.:    V6.00
 *----------------------------------------------------------------------------*/
#include "HTTP_Server_CGI.h"
#include "RTC.h"
#include "gestion_FLASH.h"


bool bool_leds_barrido=false;

// http_server.c
extern uint16_t AD_in (uint32_t ch);
extern uint8_t  get_button (void);

// net_sys.c
extern  LOCALM localm[];
#define LocM   localm[NETIF_ETH]

// Net_Config.c
extern struct tcp_cfg   tcp_config;
extern struct http_cfg  http_config;
#define tcp_NumSocks    tcp_config.NumSocks
#define tcp_socket      tcp_config.Scb
#define http_EnAuth     http_config.EnAuth
#define http_auth_passw http_config.Passw

extern bool LEDrun;
extern bool SNTP_Choose_IP;
 int ganancia=1;
 char NuevoOverload[10]=" ";
 bool activarOverload=false;
	double overload=0;
	 char EstadoOverload[2][15];
  

// Local variables.
extern uint8_t P2;

char lcd_text[2][20+1];
char SNTP_ip[18];

extern bool escribirLCD1;
extern bool escribirLCD2;

// My structure of CGI status variable.
typedef struct {
  uint16_t xcnt;
  uint16_t unused;
} MY_BUF;
#define MYBUF(p)        ((MY_BUF *)p)

// Process query string received by GET request.
void cgi_process_query (const char *qstr) {
  char var[40];


  do {
    // Loop through all the parameters
    qstr = http_get_env_var (qstr, var, sizeof (var));
    // Check return string, 'qstr' now points to the next parameter
    if (var[0] != 0) {
      // First character is non-null, string exists
      if (strncmp (var, "ip=", 3) == 0) {
        // Local IP address
        ip4_aton (&var[3], LocM.IpAddr);
      }
      else if (strncmp (var, "msk=", 4) == 0) {
        // Local network mask
        ip4_aton (&var[4], LocM.NetMask);
      }
      else if (strncmp (var, "gw=", 3) == 0) {
        // Default gateway IP address
        ip4_aton (&var[3], LocM.DefGW);
      }
      else if (strncmp (var, "pdns=", 5) == 0) {
        // Primary DNS server IP address
        ip4_aton (&var[5], LocM.PriDNS);
      }
      else if (strncmp (var, "sdns=", 5) == 0) {
        // Secondary DNS server IP address
        ip4_aton (&var[5], LocM.SecDNS);
      }
    }
  } while (qstr);
}

// Process data received by POST request.
// Type code: - 0 = www-url-encoded form data.
//            - 1 = filename for file upload (null-terminated string).
//            - 2 = file upload raw data.
//            - 3 = end of file upload (file close requested).
//            - 4 = any XML encoded POST data (single or last stream).
//            - 5 = the same as 4, but with more XML data to follow.
void cgi_process_data (uint8_t code, const char *data, uint32_t len) {
  char var[40],passw[12];
	
   
	//LEDrun=true;
  if (code != 0) {
    // Ignore all other codes
    return;
  }

  P2 = 0;
  if (len == 0) {
    return;
  }
  passw[0] = 1;
  do {
    // Parse all parameters
    data = http_get_env_var (data, var, sizeof (var));
    if (var[0] != 0) {
      // First character is non-null, string exists
      
			 if (strcmp (var, "ctrl=1") == 0) {
				ganancia=1;
      }	
			else if (strcmp (var, "ctrl=5") == 0) {
				ganancia=5;
      }	
			else if (strcmp (var, "ctrl=10") == 0) {
				ganancia=10;
      }	
			else if (strcmp (var, "ctrl=50") == 0) {
				ganancia=50;
      }	
			else if (strcmp (var, "ctrl=100") == 0) {
				ganancia=100;
      }	
      else if (strncmp (var, "lcd2=", 5) == 0) {
        
        strcpy (NuevoOverload, var+5);
				overload= strtod(NuevoOverload,NULL);
		
      }
			else if (strcmp (var, "ctrl=HABILITADO") == 0) {
				activarOverload=false;
      }	
			else if (strcmp (var, "ctrl=DESHABILITADO") == 0) {
				activarOverload=true;
      }	
			else if (strcmp (var, "") == 0) {
				P2 |= !(0x02);
      }	
    }
  } while (data);
  LED_SetOut (P2);
}

// Generate dynamic web data from a script line.
uint32_t cgi_script (const char *env, char *buf, uint32_t buflen, uint32_t *pcgi) {
  TCP_INFO *tsoc;
  const char *lang;
  uint32_t len = 0;

  static uint32_t adv;
	char fecha[22];
	char fraseganancia[2];
	char fraseoverload[20];
	char fraseEstadoOverload[20];
		strcpy(EstadoOverload[0], "ESTABLE\0");
		strcpy(EstadoOverload[1], "SOBREPASADO\0");


  switch (env[0]) {


    case 'f':
     
      switch (env[2]) {
        case '1':
          len = sprintf (buf, &env[4], NuevoOverload);
          break;
      }
      break;

    

		
		case 'j':
           
          
		      sprintf(fraseganancia, "%d", ganancia);
          len = sprintf (buf, &env[4], fraseganancia);
					
         
      break;
		
		case 'a':
           
         
		      sprintf(fraseoverload, "%2f",overload);
          len = sprintf (buf, &env[1], fraseoverload);
					
         
      break;
		
		case 'l':
				

          len = sprintf (buf, &env[4], EstadoOverload[0]);
         
      break;
		
		case 'k':
					

          len = sprintf (buf, &env[1],  EstadoOverload[1]);
         
      break;
		
		
		 case 'm':
      
        
        len = sprintf (buf, &env[1], "--","1","5","10","50","100");
			
        break;
		 
		 case 'b':
      
         len = sprintf (buf, &env[1], activarOverload ?     ""     : "selected",
                                     activarOverload ? "selected" :    ""     );
			
        break;
		 
  }
  return (len);
}
