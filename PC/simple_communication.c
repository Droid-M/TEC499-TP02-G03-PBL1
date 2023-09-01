#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "./helpers/helpers.h"

#define SIMULATE_UART

char dialog(char *dialogMessage)
{
    printf("%s", dialogMessage);
    char exit = input_char();
    while (exit != 's' && exit != 'n')
    {
        printf("\nOpção inválida! Insira 's' para sim ou 'n' para não: ");
        exit = input_char();
    }
    return exit;
}

#ifdef SIMULATE_UART

void sent_data(char *data)
{
    printf("\n(Simulando envio de dados)");
}

int receive_data(char *buffer)
{
    printf("\n(Simulando recebimento de dados)");
}

void configure_uart()
{
    printf("\n(Simulando configuração de UART");
}

void disconnect_uart()
{
    printf("\n(Simulando desconexão de UART)");
}

#else

#include <termios.h>

int fd;

void configureSerialPort(speed_t baud_rate)
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

void sent_data(char *data)
{
    // Enviando sinal de start
    //  Valor estático a ser enviado como start bit (0)
    unsigned char start_bit = 0;

    // Enviar o valor estático como start bit
    write(fd, &start_bit, 1);

    // Enviar os dados para a porta serial
    write(fd, data, strlen(data));
}

int receive_data(char *buffer)
{
    // Receber dados da porta serial
    int bytes_qty = read(fd, buffer, sizeof(buffer));
    return bytes_qty;
}

void configure_uart()
{
    fd = open("/dev/ttyS0", O_RDWR); // Substitua pelo dispositivo correto (ex: /dev/ttyUSB0)

    if (fd == -1)
    {
        perror("\nErro ao abrir a porta serial");
        return 1;
    }

    // Configurar a porta serial com baud rate de 9600
    configureSerialPort(fd, B9600);
}

void disconnect_uart()
{
    close(fd);
}

#endif

int main()
{
    char data[2];
    char buffer[32];
    char can_read_or_write;
    char exit;
    int bytes_qty;

    printf("\nConfigurando UART...");
    configure_uart();
    printf("\nUART configurada!");

    do
    {
        can_read_or_write = dialog("\nDeseja enviar algum caractere para a FPGA, 's' ou 'n'? ");
        if (can_read_or_write == 's')
        {
            printf("\nInsira o caractere a ser enviado para a FPGA: ");
            data[0] = input_char();
            printf("\nEnviando '%s' para a FPGA...", data);
            sent_data(data);
            printf("\nCaractere '%s' enviado com sucesso!", data);
        }
        can_read_or_write = dialog("\n\nDeseja ler algum dado que a FPGA possa enviar, 's' ou 'n'? ");
        if (can_read_or_write == 's')
        {
            printf("\nAguardando dados da FPGA...");
            bytes_qty = receive_data(buffer);
            if (bytes_qty > 0)
            {
                buffer[bytes_qty] = '\0';
                printf("\nDados recebidos: %s", buffer);
            }
            else
            {
                printf("\nNenhum foi recebido!");
            }
        }
        exit = dialog("\n\n\nRealizar outra ação, 's' ou 'n'? ");
    } while (exit == 's');

    printf("\nEncerrando conexão... Pressione Enter para sair do programa...\n");
    while (input_char() != 13)
    {
    };

    // Fechar a porta serial
    disconnect_uart();

    return 0;
}
