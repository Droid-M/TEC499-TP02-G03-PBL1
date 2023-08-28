#include "helpers.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>

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
        if (i >= rand_number)
        {
            i = 0;
            rand_number = ((rand() % 6) + 6);
        }
    }
    return;
}

void skip_lines(int qty, int delay_per_line, char *line_content)
{
    for (int i = 0; i < qty; i++)
    {
        if (delay_per_line > 0)
            sleep(delay_per_line);
        printf("%s\n", line_content);
        fflush(stdout);
    }
}

#ifdef _WIN32
void c_log(const char *to_output, ...)
{
    va_list args;
    va_start(args, to_output);
    size_t len = strlen(to_output);
    wchar_t wformat[len + 1];
    mbstowcs(wformat, to_output, len + 1);
    wprintf(wformat, args);
    va_end(args);
}

void clear_console()
{
    system("cls");
}

#else
void c_log(const char *to_output, ...)
{
    va_list args;
    va_start(args, to_output);
    vprintf(to_output, args);
    va_end(args);
}

void clear_console()
{
    system("clear");
}
#endif