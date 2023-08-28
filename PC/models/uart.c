#include "models.h"
#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

int fd;

void open_connection()
{
    fd = open("/dev/ttyS0", O_RDWR); // Substitua pelo dispositivo correto (ex: /dev/ttyUSB0)

    if (fd == -1)
    {
        perror("Erro ao abrir a porta serial");
        return 1;
    }

    // Configurar a porta serial com baud rate de 9600
    configure_serial_port(fd, B9600);
}

void configure_serial_port(int fd, speed_t baud_rate)
{
    struct termios options;

    // Obter as configurações atuais da porta serial
    tcgetattr(fd, &options);

    // Configurar velocidade de leitura (entrada) e escrita (saída)
    cfsetispeed(&options, baud_rate);
    cfsetospeed(&options, baud_rate);

    // Configurar opções da porta serial: 8 bits de dados, sem paridade, 1 stop bit

    // Desabilita a paridade (parity enable bit) na configuração da porta serial
    options.c_cflag &= ~PARENB;

    // Define um único bit de parada (stop bit) na configuração da porta serial
    options.c_cflag &= ~CSTOPB;

    // Limpa os bits de tamanho de caractere (character size bits) na configuração da porta serial
    options.c_cflag &= ~CSIZE;

    // Define o tamanho do caractere como 8 bits na configuração da porta serial
    options.c_cflag |= CS8;

    // Habilita a capacidade de leitura (recebimento) de dados na configuração da porta serial
    options.c_cflag |= CREAD;

    // Definir as configurações da porta serial
    tcsetattr(fd, TCSANOW, &options);
}

void tx_char(char *data)
{
    unsigned char start_bit = 0;
    write(fd, &start_bit, 1);
    write(fd, data, strlen(&data));
}

void tx_hex(unsigned int hex_value)
{
    uint8_t data = (uint8_t)hex_value;
    unsigned char start_bit = 0;
    write(fd, &start_bit, 1);
    write(fd, data, strlen(&data));
}

void can_start_protocol()
{
    
}

char *rx_char()
{
    // Aguardar um atraso antes de ler a resposta
    usleep(200000); // Recomendado: Atraso de 200 milissegundos

    // Receber dados da porta serial
    char buffer[32];
    int numBytes = read(fd, buffer, sizeof(buffer));
    if (numBytes > 0)
    {
        buffer[numBytes] = '\0';
    }
    return buffer;
}

void close_connection()
{
    // Fechar a porta serial
    close(fd);
}
