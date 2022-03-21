
#include "SNTP.h"


void get_time_ntp(void) {
  const uint8_t ntp_server[4] = {217,79,179,106};
 
  if (sntp_get_time (&ntp_server[0], time_cback) == true) {
    printf ("SNTP request sent.\n");
  }
  else {
    printf ("Failed, SNTP not ready or bad parameters.\n");
  }
}
 
void time_cback (uint32_t time) {
  if (time == 0) {
    printf ("Error, server not responding or bad response.\n");
  }
  else {
    printf ("%d seconds elapsed since 1.1.1970\n", time);
  }
}