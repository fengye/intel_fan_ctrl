#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main (void)
{
    char sbuf[256] = {0};

    FILE *fp = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
    if (fp) 
    {
      fread(sbuf, sizeof(sbuf), 1, fp);
      int temp = atoi(sbuf);
      fprintf(stdout, "%.2f\n", (float)(temp/1000.0f));
      fclose(fp);
    }
    else
    {
      fprintf(stderr, "Error opening system file.\n");
    }
    fflush(stdout);

    return 0;
}
