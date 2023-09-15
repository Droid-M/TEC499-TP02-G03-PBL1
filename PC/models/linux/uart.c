#include "../models.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../../helpers/helpers.h"
#include <termios.h>
#include <stdint.h>

int fd;

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
    char *buffer = (char *)malloc(2); // Alocar memória para o byte lido e o terminador nulo
    if (buffer != NULL)
    {
        // Aguardar um atraso antes de ler o byte
        usleep(200000); // Recomendado: Atraso de 200 milissegundos

        // Receber dados da porta serial
        int numBytes = read(fd, buffer, 1); // Lê apenas 1 byte
        if (numBytes == 1)
        {
            buffer[1] = '\0'; // Adicionar o terminador nulo
            return buffer;
        }
        else
        {
            free(buffer);
            return NULL; // Retorna NULL se ocorrer um erro
        }
    }
    else
    {
        return NULL; // Retorna NULL em caso de falha na alocação de memória
    }
}

int rx_int()
{
    char *byte_rx = rx_char();
    if (byte_rx[0] == '\0')
        return -1;
    return (int)strtol(byte_rx, NULL, 16);
}