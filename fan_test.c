#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "get_model.h"

// PWM pin only works on 1
#define PWM 1

int main (void)
{
  const int model = get_raspi_model();
  if (model == RPi_Unknown)
  {
    fprintf(stderr, "Unknown Raspberry Pi model, exit.\n");
    exit(-1);
  }

  printf("Raspberry Pi wiringPi for Intel stock CPU fan test\n");
  printf("Detected model: %s\n", RaspiNames[model]) ;

  if (wiringPiSetup () == -1)
    exit (1) ;

  pinMode (PWM, PWM_OUTPUT) ;
  pwmSetMode(PWM_MODE_MS);

  pwmSetRange(PWM_HIGH_RANGES[model]);
  pwmSetClock(PWM_DIVISORS[model]);

  int test = 0;

  for (;;)
  {
    int speed = (test == 0) ? PWM_HIGH_RANGES[model] : PWM_LOW_RANGES[model];
    pwmWrite(PWM, speed);
    printf("Speed: %d\n", speed);
    delay(2500);

    test = (test + 1) % 2;
  }

  return 0 ;
}
