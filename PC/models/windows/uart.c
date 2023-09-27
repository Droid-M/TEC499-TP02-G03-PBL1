#include "../models.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../../helpers/helpers.h"

// REVIEW - Implementar funções do C
int open_connection() { return 1; }
void close_connection() {}
char *rx_char()
{
    char *data = (char *)malloc(2 * sizeof(char)); // Alocar memória para um caractere + terminador nulo
    if (data != NULL)
    {
        srand(time(NULL));
        data[0] = (char)(rand() % 128); // Armazenar um número aleatório entre 0 e 128
        data[1] = '\0';                 // Adicionar o terminador nulo
    }
    else
    {
        free(data);
    }
    return data;
}

int rx_int()
{
    char *byte_rx = rx_char();
    if (byte_rx[0] == '\0')
        return -1;
    // return (int)strtol(byte_rx, NULL, 16);
    return (int)byte_rx[0];
}
void tx_char(char *data) {}
void tx_hex(unsigned int hex_value) {}