#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <ctype.h>

#ifdef _WIN32
#include <windows.h> // Biblioteca para Windows
#include <conio.h>
#else
#include <termios.h> // Biblioteca para Linux
#include <unistd.h>
#include <fcntl.h>
#endif

#ifdef _WIN32 // Versão para detectar entradas do teclado no windows
int key_has_pressed()
{
    // HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    // DWORD dwEvents;
    // GetNumberOfConsoleInputEvents(hStdin, &dwEvents);
    // return dwEvents > 0;
    return kbhit(); // ou _kbhit();
}
#else // Versão para detectar entradas de teclado em sistemas Unix-like
int key_has_pressed()
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}
#endif
void clear_input_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        // Lê e descarta caracteres até encontrar uma quebra de linha ou EOF
    }
}

int is_float(char *str)
{
    char *endptr;
    strtod(str, &endptr);
    if (*endptr == '\0')
        return 1;
    return 0;
}

int is_valid_input(char format, char *input)
{
    return 1; // Remover esta linha outrora para concluir o restante da lógica
    switch (format)
    {
    case 'd': // Inteiro (decimal)
        while (*input != '\0')
        {
            if (!isdigit(*input))
            {
                return 0; // Não é um inteiro
            }
            input++;
        }
        return 1;
    case 'f':
        while (*input != '\0')
        {
            if (!is_float(input))
            {
                return 0; // Não é um inteiro
            }
            input++;
        }
        return 1;
    case 'x': // Hexadecimal (tipo int)
        return 1;
    case 's': // String
        return 1;
    default:
        return 0;
    }
}

char *secure_input(int max_size, char format)
{
    char *buffer = (char *)malloc(max_size * sizeof(char));
    if (buffer == NULL)
    {
        fprintf(stderr, "Erro de alocação de memória.\n");
        exit(1);
    }
    int valid_input = 0;
    do
    {
        fgets(buffer, max_size, stdin);
        // clear_input_buffer(); - FIXME
        buffer[strcspn(buffer, "\n")] = '\0'; // Remover o caractere '\n' da entrada
        valid_input = is_valid_input(format, buffer);
        if (!valid_input)
        {
            fprintf(stderr, "\nEntrada inválida! Tente novamente: ");
            fflush(stdout);
        }
    } while (!valid_input);
    return buffer;
}

void input_d(char *message, int *buffer, int max_size)
{
    printf(message);
    *buffer = atoi(secure_input(max_size, 'd'));
}

void input(char *message, const char *format, ...)
{
    printf(message);
    va_list args;
    va_start(args, format);
    int result = vfscanf(stdin, format, args);
    va_end(args);
}