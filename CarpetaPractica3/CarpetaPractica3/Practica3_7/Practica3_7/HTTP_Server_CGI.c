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
      if (strcmp (var, "led0=on") == 0) {
        P2 |= 0x01;
				modificarXposicion(22,0x00040000,P2,11); //P2 es uint8_t
      }
      else if (strcmp (var, "led1=on") == 0) {
        P2 |= 0x02;
				modificarXposicion(22,0x00040000,P2,11); //P2 es uint8_t
      }
      else if (strcmp (var, "led2=on") == 0) {
        P2 |= 0x04;
				modificarXposicion(22,0x00040000,P2,11); //P2 es uint8_t
      }
      else if (strcmp (var, "led3=on") == 0) {
        P2 |= 0x08;
				modificarXposicion(22,0x00040000,P2,11); //P2 es uint8_t
      } 
			else if (strcmp (var, "led0=off") == 0) {
        P2 |= !(0x01);
				modificarXposicion(22,0x00040000,P2,11); //P2 es uint8_t
      }
      else if (strcmp (var, "led1=off") == 0) {
        P2 |= !(0x02);
				modificarXposicion(22,0x00040000,P2,11); //P2 es uint8_t
      }
      else if (strcmp (var, "led2=off") == 0) {
        P2 |= !(0x04);
				modificarXposicion(22,0x00040000,P2,11); //P2 es uint8_t
      }
      else if (strcmp (var, "led3=off") == 0) {
        P2 |= !(0x08);
				modificarXposicion(22,0x00040000,P2,11); //P2 es uint8_t
      }
      else if (strcmp (var, "ctrl=Browser") == 0) {
				LEDrun=false;
				modificarXposicion(22,0x00040000,0x00,10);
      }
			else if (strcmp (var, "ctrl=Running Lights") == 0) {
				modificarXposicion(22,0x00040000,0x01,10);
				LEDrun=true;
      }	
			else if (strcmp (var, "ctrl=150,214,94,10") == 0) {
				SNTP_Choose_IP=false;
      }	
			else if (strcmp (var, "ctrl=150,214,94,5") == 0) {
				SNTP_Choose_IP=true;
      }	
      else if ((strncmp (var, "pw0=", 4) == 0) ||
               (strncmp (var, "pw2=", 4) == 0)) {
        // Change password, retyped password
        if (http_EnAuth) {
          if (passw[0] == 1) {
            strcpy (passw, var+4);
          }
          else if (strcmp (passw, var+4) == 0) {
            // Both strings are equal, change the password
            strcpy (http_auth_passw, passw);
          }
        }
      }
      else if (strncmp (var, "lcd1=", 5) == 0) {
        // LCD Module line 1 text
        strcpy (lcd_text[0], var+5);
				osSignalSet(id_pantalla,0x0001);
      }
      else if (strncmp (var, "lcd2=", 5) == 0) {
        // LCD Module line 2 text
        strcpy (lcd_text[1], var+5);
				osSignalSet(id_pantalla,0x0002);
      }
			else if (strncmp (var, "SNTP=", 5) == 0) {
        // LCD Module line 2 text
        strcpy (SNTP_ip, var+5);
				
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
  uint8_t id;
  static uint32_t adv;
	char fecha[22];

  switch (env[0]) {
    // Analyze a 'c' script line starting position 2
    case 'a' :
      // Network parameters from 'network.cgi'
      switch (env[2]) {
        case 'i':
          // Write local IP addressd
          len = sprintf (buf, &env[4], ip4_ntoa (LocM.IpAddr));
          break;
        case 'm':
          // Write local network mask
          len = sprintf (buf, &env[4], ip4_ntoa (LocM.NetMask));
          break;
        case 'g':
          // Write default gateway IP address
          len = sprintf (buf, &env[4], ip4_ntoa (LocM.DefGW));
          break;
        case 'p':
          // Write primary DNS server IP address
          len = sprintf (buf, &env[4], ip4_ntoa (LocM.PriDNS));
          break;
        case 's':
          // Write secondary DNS server IP address
          len = sprintf (buf, &env[4], ip4_ntoa (LocM.SecDNS));
          break;
      }
      break;

    case 'b':
      // LED control from 'led.cgi'
      if (env[2] == 'c') {
        // Select Control
        len = sprintf (buf, &env[4], LEDrun ?     ""     : "selected",
                                     LEDrun ? "selected" :    ""     );
        break;
      }
      // LED CheckBoxes
      id = env[2] - '0';
      if (id > 7) {
        id = 0;
      }
      id = 1 << id;
      len = sprintf (buf, &env[4], (P2 & id) ? "checked" : "");
      break;

    case 'c':
      // TCP status from 'tcp.cgi'
      while ((len + 150) < buflen) {
        tsoc = &tcp_socket[MYBUF(pcgi)->xcnt];
        MYBUF(pcgi)->xcnt++;
        // 'sprintf' format string is defined here
        len += sprintf (buf+len,   "<tr align=\"center\">");
        if (tsoc->State <= tcpStateCLOSED) {
          len += sprintf (buf+len, "<td>%d</td><td>%s</td><td>-</td><td>-</td>"
                                   "<td>-</td><td>-</td></tr>\r\n",
                                   MYBUF(pcgi)->xcnt,tcp_ntoa(tsoc->State));
        }
        else if (tsoc->State == tcpStateLISTEN) {
          len += sprintf (buf+len, "<td>%d</td><td>%s</td><td>%d</td><td>-</td>"
                                   "<td>-</td><td>-</td></tr>\r\n",
                                   MYBUF(pcgi)->xcnt, tcp_ntoa(tsoc->State), tsoc->LocPort);
        }
        else {
          len += sprintf (buf+len, "<td>%d</td><td>%s</td><td>%d</td>"
                                   "<td>%d</td><td>%s</td><td>%d</td></tr>\r\n",
                                   MYBUF(pcgi)->xcnt, tcp_ntoa(tsoc->State), tsoc->LocPort,
                                   tsoc->AliveTimer, ip4_ntoa (tsoc->RemAddr), tsoc->RemPort);
        }
        // Repeat for all TCP Sockets
        if (MYBUF(pcgi)->xcnt == tcp_NumSocks) {
          break;
        }
      }
      if (MYBUF(pcgi)->xcnt < tcp_NumSocks) {
        // Hi bit is a repeat flag
        len |= (1u << 31);
      }
      break;

    case 'd':
      // System password from 'system.cgi'
      switch (env[2]) {
        case '1':
          len = sprintf (buf, &env[4], http_EnAuth ? "Enabled" : "Disabled");
          break;
        case '2':
          len = sprintf (buf, &env[4], http_auth_passw);
          break;
      }
      break;

    case 'e':
      // Browser Language from 'language.cgi'
      lang = http_server_get_lang ();
      if      (strncmp (lang, "en", 2) == 0) {
        lang = "English";
      }
      else if (strncmp (lang, "de", 2) == 0) {
        lang = "German";
      }
      else if (strncmp (lang, "fr", 2) == 0) {
        lang = "French";
      }
      else if (strncmp (lang, "sl", 2) == 0) {
        lang = "Slovene";
      }
      else {
        lang = "Unknown";
      }
      len = sprintf (buf, &env[2], lang, http_server_get_lang());
      break;

    case 'f':
      // LCD Module control from 'lcd.cgi'
      switch (env[2]) {
        case '1':
          len = sprintf (buf, &env[4], lcd_text[0]);
          break;
        case '2':
          len = sprintf (buf, &env[4], lcd_text[1]);
          break;
      }
      break;

    case 'g':
      // AD Input from 'ad.cgi'
      switch (env[2]) {
        case '1':
          adv = AD_in (0);
          len = sprintf (buf, &env[4], adv);
          break;
        case '2':
          len = sprintf (buf, &env[4], (float)adv*3.3f/4096);
          break;
        case '3':
          adv = (adv * 100) / 4096;
          len = sprintf (buf, &env[4], adv);
          break;
      }
      break;

    case 'x':
      // AD Input from 'ad.cgx'
      adv = AD_in (0);
      len = sprintf (buf, &env[1], adv);
      break;

    case 'y':
      // Button state from 'button.cgx'
      len = sprintf (buf, "<checkbox><id>button%c</id><on>%s</on></checkbox>",
                     env[1], (get_button () & (1 << (env[1]-'0'))) ? "true" : "false");
      break;
		
		case 'j':
           
          get_time( &hora_ver, &min_ver, &sec_ver);
		      get_date( &dia_ver,&mes_ver,&ano_ver ); 
		      sprintf(fecha, "%d : %d : %d     %d %d %d", hora_ver, min_ver, sec_ver, dia_ver, mes_ver, ano_ver);
          len = sprintf (buf, &env[4], fecha);
					osSignalSet(id_pantalla,0x0003);
         
      break;
		
		case 'k':
           
          get_time( &hora_ver, &min_ver, &sec_ver);
		      get_date( &dia_ver,&mes_ver,&ano_ver ); 
		      sprintf(fecha, "%d : %d : %d     %d %d %d", hora_ver, min_ver, sec_ver, dia_ver, mes_ver, ano_ver);
          len = sprintf (buf, &env[1], fecha);
					osSignalSet(id_pantalla,0x0003);
         
      break;
		case 'l':
            len = sprintf (buf, &env[4], SNTP_ip);
         
      break;
		
		
		 case 'm':
      
      if (env[2] == 'c') {
        
        len = sprintf (buf, &env[4], SNTP_Choose_IP ?     ""     : "selected",
                                     SNTP_Choose_IP ? "selected" :    ""     );
			}
        break;
  }
  return (len);
}
