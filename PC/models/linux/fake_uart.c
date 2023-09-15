#include "../models.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../../helpers/helpers.h"

int open_connection()
{
    return 1;
}
void close_connection() {}

char *rx_char()
{
    static char hex_char[3]; // Tamanho suficiente para dois dígitos + '\0'
    int hex_int = random_hexa(2);
    // printf("\n\nGerando valores aleatórios. Isto vai demorar um segundo (literalmente)...\n\n");
    sleep(1); // Para dar tempo da semente do random ser regenerada
    if (random_decimal(4) > 1)
        snprintf(hex_char, sizeof(hex_char), "%02X", 0x07); // Usar %02X para sempre gerar 2 dígitos
    else
        snprintf(hex_char, sizeof(hex_char), "%02X", hex_int);
    return hex_char;
}

int rx_int()
{
    char *byte_rx = rx_char();
    if (byte_rx[0] == '\0')
        return -1;
    return (int)strtol(byte_rx, NULL, 16);
}

void tx_char(char *data) {}

void tx_hex(unsigned int hex_value) {}