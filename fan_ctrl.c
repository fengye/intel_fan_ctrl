#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "get_model.h"

// Raspberry Pi only has WiringPI 1 enabled as PWM pin
#define PWM 1

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif


static const int TEMP_RANGE[] = {35000, 70000};

int main (void)
{
  const int model = get_raspi_model();
  if (model == RPi_Unknown)
  {
    fprintf(stderr, "Unknown Raspberry Pi model, exit.\n");
    exit(-1);
  }
  char sbuf[256] = {0};

  printf("Raspberry Pi wiringPi for Intel stock CPU fan control\n");
  printf("Detected model: %s\n", RaspiNames[model]) ;

  if (wiringPiSetup () == -1)
    exit (1) ;

  pinMode (PWM, PWM_OUTPUT) ;
  pwmSetMode(PWM_MODE_MS);

  pwmSetRange(PWM_HIGH_RANGES[model]);
  pwmSetClock(PWM_DIVISORS[model]);

  int speed = 0;
  const int speed_range = PWM_HIGH_RANGES[model] - PWM_LOW_RANGES[model];
  const int temp_range = TEMP_RANGE[1] - TEMP_RANGE[0];

  for (;;)
  {
    FILE *fp = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
    if (fp) 
    {
      fread(sbuf, sizeof(sbuf), 1, fp);
      int temp = atoi(sbuf);
      float interpo = (float)(temp - TEMP_RANGE[0]) / (float)temp_range; 
      interpo = min(max(0.0f, interpo), 1.0f);
      speed = min(PWM_LOW_RANGES[model] + (int)(interpo * speed_range), PWM_HIGH_RANGES[model]);
      pwmWrite(PWM, speed);
      fprintf(stdout, "\rTemp(C): %.2f\t Fan: %d", (float)(temp/1000.0f), speed);
      fclose(fp);
    }
    else
    {
      fprintf(stderr, "Error opening system file.\n");
      exit(-1);
    }
    fflush(stdout);
    delay(1000);
  }

  return 0 ;
}
