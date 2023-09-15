#include "../helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <ctype.h>
#include <windows.h> // Biblioteca para Windows
#include <conio.h>

int key_has_pressed()
{
    // HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    // DWORD dwEvents;
    // GetNumberOfConsoleInputEvents(hStdin, &dwEvents);
    // return dwEvents > 0;
    return kbhit(); // ou _kbhit();
}

char input_char()
{
    return getch();
}

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

short int is_valid_hex(const char *str)
{
    if (str == NULL)
    {
        return 0;
    }

    // Verificar se a string começa com "0x"
    if (strncmp(str, "0x", 2) == 0)
    {
        str += 2; // Pular os caracteres "0x"
    }

    // Verificar se cada caractere é um dígito hexadecimal
    for (; *str != '\0'; str++)
    {
        if (!isxdigit((unsigned char)*str))
        {
            return 0;
        }
    }

    return 1;
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
        return is_valid_hex(input);
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
    printf("%s", message);
    *buffer = atoi(secure_input(max_size, 'd'));
}

void input_x(char *message, int *buffer, int max_size)
{
    char *checker;
    printf("%s", message);
    *buffer = strtol(secure_input(max_size, 'x'), &checker, 16);
}

void input(char *message, const char *format, ...)
{
    printf("%s", message);
    va_list args;
    va_start(args, format);
    int result = vfscanf(stdin, format, args);
    va_end(args);
}

void pause_program(char *message)
{
    printf("\n");
    printf("%s", message);
    fflush(stdout);
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        // Lê e descarta os caracteres até que seja pressionada a tecla Enter
    }
}