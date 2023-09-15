#include "../helpers.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void slowed_printf(char *to_print, float delay_seconds)
{
    srand(time(NULL));
    int i = 1;
    int rand_number = ((rand() % 6) + 6);
    while (*to_print != '\0')
    {
        float pause = (delay_seconds * 100000000.0) / i++;
        char printed_pause[20];
        snprintf(printed_pause, sizeof(printed_pause), "%f", pause);
        printf("%c", *to_print++);
        fflush(stdout);
        sleep_micros(pause);
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

void execute(const char *program_name, char *arguments[])
{
    pid_t child_pid = fork();

    if (child_pid == -1)
    {
        exit(1);
    }
    else if (child_pid == 0)
    {
        // setsid();
        execvp(program_name, arguments);
    }
    else
    {
        return;
    }
}