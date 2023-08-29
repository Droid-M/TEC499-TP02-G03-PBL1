#include "helpers.h"
#include <time.h>
#include <stdio.h>

int sleep_nanos(unsigned long long ns)
{
    struct timespec delay;
    delay.tv_sec = ns / 1000000000;  // segundos
    delay.tv_nsec = ns % 1000000000; // nanossegundos
    int result = nanosleep(&delay, NULL);
    return result;
}

int sleep_micros(unsigned long long us)
{
    return sleep_nanos(us / 1000);
}