#include "helpers.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>

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

void execute(const char *program_name, char *arguments[])
{
    char *command_line = _strdup(program_name);;
    for (int i = 0; arguments[i] != NULL; i++)
    {
        command_line = realloc(command_line, strlen(command_line) + strlen(arguments[i]) + 2);
        strcat(command_line, " ");
        strcat(command_line, arguments[i]);
    }

    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    PROCESS_INFORMATION pi;

    if (CreateProcess(
            program_name, // Nome do programa a ser executado (não é necessário)
            command_line, // Argumentos formatados em uma única string
            NULL,         // Processo pai não herda handles de processo
            NULL,         // Processo pai não herda handles de thread
            FALSE,        // Não herdar os handles do processo pai
            0,            // Flags de criação (normalmente 0)
            NULL,         // Variáveis de ambiente (herda as do processo pai)
            NULL,         // Diretório de trabalho (mesmo do processo pai)
            &si,          // Informações de inicialização do processo filho
            &pi           // Informações de identificação do processo e thread
            ))
    {
        printf("Programa %s foi iniciado em segundo plano (PID: %lu).\n", program_name, pi.dwProcessId);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else
    {
        fprintf(stderr, "Erro ao executar o programa. Código de erro: %lu\n", GetLastError());
    }

    free(command_line);
}