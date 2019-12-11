#ifndef _H_GET_MODEL_
#define _H_GET_MODEL_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Raspberry Pi 2-3 has a PWM clock hardware of 19.2MHz. In order to make 25KHz Intel CPU fan control signal
// we have to divide it by 8, and make the range [0, 96]. Thus 19.2M / 8 / 96 = 25K
// Raspberry Pi 4 has a PWM clock hardware of 54MHz. In order to make 25KHz Intel CPU fan control signal
// we have to divide it by 16, and make the range [0, 135]. Thus 54M / 16 / 135 = 25K
// Other model of Raspberry Pi need to adjust those two values based on their hardward respectively

typedef enum _RaspiModel {
	RPi_1 = 0,
	RPi_Zero,
	RPi_2,
	RPi_3,
	RPi_4,
	RPi_Unknown
} RaspiModel;

const int PWM_HIGH_RANGES[] = {
	96, 96, 96, 96, 135
};

const int PWM_LOW_RANGES[] = {
	30, 30, 30, 30, 40
};

const int PWM_DIVISORS[] = {
	8, 8, 8, 8, 16
};

const char* RaspiNames[] = {
	"Raspberry Pi 1",
	"Raspberry Pi Zero",
	"Raspberry Pi 2",
	"Raspberry Pi 3",
	"Raspberry Pi 4",
	"Unknown model"
};

#define BUF_LEN 256
#define BUF_MAX 255


int get_raspi_model()
{
  FILE* cpuinfo_fp = fopen("/proc/cpuinfo", "rb");
  if (!cpuinfo_fp)
  {
    fprintf(stderr, "Cannot open /proc/cpuinfo, exiting\n");
    return RPi_Unknown;
  }

  char line[BUF_LEN];
  while(fgets(line, BUF_LEN, cpuinfo_fp))
  {
  }

  fclose(cpuinfo_fp);
  // last line contains model info
  int idx = 0;
  char* tokens[16] = {NULL};
  char* pch = strtok(line, " ,:-");
  while((tokens[idx++] = pch))
  {
    pch = strtok(NULL, " ,:-");
  }
  if (strcmp(tokens[1], "Raspberry") == 0 &&
      strcmp(tokens[2], "Pi") == 0)
  {
     if (strcmp(tokens[3], "1") == 0)
     {
        return RPi_1;
     }
     else if (strcmp(tokens[3], "2") == 0)
     {
        return RPi_2;
     }
     else if (strcmp(tokens[3], "3") == 0)
     {
        return RPi_3;
     }
     else if (strcmp(tokens[3], "4") == 0)
     {
        return RPi_4;
     }
     // TODO: Pi Zero
  }

  return RPi_Unknown;
}

#endif // _H_GET_MODEL_
