#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
// PWM pin only works on 1
#define PWM 1
// Raspberry Pi 4 has a PWM clock hardware of 54MHz. In order to make 25KHz Intel CPU fan control signal
// we have to divide it by 16, and make the range [0, 135]. Thus 54M / 16 / 135 = 25K
// Other model of Raspberry Pi need to adjust those two values based on their hardward respectively
#define PWM_RANGE 135
#define PWM_DIVISOR 16


#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

static int SPEED_RANGE[] = {40, 135};
static int TEMP_RANGE[] = {35000, 70000};
int main (void)
{
  char sbuf[256] = {0};

  printf ("Raspberry Pi +  wiringPi for Intel stock CPU fan control\n") ;

  if (wiringPiSetup () == -1)
    exit (1) ;

  pinMode (PWM, PWM_OUTPUT) ;
  pwmSetMode(PWM_MODE_MS);

  pwmSetRange(PWM_RANGE);
  pwmSetClock(PWM_DIVISOR);

  int speed = 0;
  const int speed_range = SPEED_RANGE[1] - SPEED_RANGE[0];
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
      speed = min(SPEED_RANGE[0] + (int)(interpo * speed_range), SPEED_RANGE[1]);
      pwmWrite(PWM, speed);
      fprintf(stdout, "\rTemp(C): %.2f\t Fan: %d", (float)(temp/1000.0f), speed);
      fclose(fp);
    }
    else
    {
      fprintf(stderr, "Error opening system file.\n");
    }
    fflush(stdout);
    delay(1000);
  }

  return 0 ;
}
