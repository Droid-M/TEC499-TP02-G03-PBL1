#include "models.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define IN_UART_SIMULATION // Remova esta linha para habilitar a conexão UART

#ifdef IN_UART_SIMULATION

#include "../helpers/helpers.h"

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

#else

#ifdef _WIN32

// REVIEW - Implementar funções do C
int open_connection() { return 1; }
void close_connection() {}
char *rx_char() { return '9'; }
int rx_int() { return 7; }
void tx_char(char *data) {}
void tx_hex(unsigned int hex_value) {}

#else

#include <termios.h>

int fd;

int open_connection()
{
    fd = open("/dev/ttyS0", O_RDWR); // Substitua pelo dispositivo correto (ex: /dev/ttyUSB0)

    if (fd == -1)
    {
        perror("Erro ao abrir a porta serial");
        return 0;
    }

    // Configurar a porta serial com baud rate de 9600
    configure_serial_port(fd, B9600);
    return 1;
}

void close_connection()
{
    // Fechar a porta serial
    close(fd);
}

void configure_serial_port(int fd, speed_t baud_rate)
{
    struct termios options;

    // Obter as configurações atuais da porta serial
    tcgetattr(fd, &options);

    // Configurar velocidade de leitura (entrada) e escrita (saída)
    cfsetispeed(&options, baud_rate);
    cfsetospeed(&options, baud_rate);

    // Configurar opções da porta serial...

    // Definir as configurações da porta serial
    tcsetattr(fd, TCSANOW, &options);
}

void tx_char(char *data)
{
    unsigned char start_bit = 0;
    write(fd, &start_bit, 1);
    write(fd, data, strlen(data));
}

void tx_hex(unsigned int hex_value)
{
    uint8_t data = (uint8_t)hex_value;
    unsigned char start_bit = 0;
    sleep_micros(200000);
    write(fd, &start_bit, 1);
    write(fd, &data, sizeof(data));
}

char *rx_char()
{
    char buffer[3]; // Buffer para armazenar o byte lido

    // Aguardar um atraso antes de ler o byte
    sleep_micros(200000); // Recomendado: Atraso de 200 milissegundos

    // Receber dados da porta serial
    int numBytes = read(fd, buffer, sizeof(buffer));

    if (numBytes == 1)
    {
        return buffer; // Retornar o byte lido
    }
    else
    {
        return "\0"; // Retorna caractere nulo se ocorrer um erro
    }
}

int rx_int()
{
    char *byte_rx = rx_char();
    if (byte_rx[0] == '\0')
        return -1;
    return (int)strtol(byte_rx, NULL, 16);
}

#endif

#endif