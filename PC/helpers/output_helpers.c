#include "helpers.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

void slowed_printf(char *to_print, float delay_seconds)
{
    srand(time(NULL));
    int i = 1;
    int rand_number = ((rand() % 6) + 6);
    while (*to_print != '\0')
    {
        float pause = (delay_seconds * 1000000.0) / i++;
        char printed_pause[20];
        snprintf(printed_pause, sizeof(printed_pause), "%f", pause); 
        printf("%c", *to_print++);
        fflush(stdout);
        usleep((useconds_t)pause); 
        if (i >= rand_number) {
            i = 0;
            rand_number = ((rand() % 6) + 6);
        }
    }
    return;
}

void skip_lines(int qty)
{
    for (int i = 0; i < qty; i++)
    {
        printf("\n");
    }
}