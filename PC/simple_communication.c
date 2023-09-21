#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "./helpers/helpers.h"

// #define SIMULATE_UART

char dialog(char *dialogMessage, const char option1, const char option2)
{
    printf("%s", dialogMessage);
    char choice = input_char();
    while (choice != option1 && choice != option2)
    {
        printf("\nOpção inválida! %s", dialogMessage);
        choice = input_char();
    }
    return choice;
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

int configure_uart()
{
    fd = open("/dev/ttyS0", O_RDWR); // Substitua pelo dispositivo correto (ex: /dev/ttyUSB0)

    if (fd == -1)
    {
        perror("\nErro ao abrir a porta serial");
        return 0;
    }

    // Configurar a porta serial com baud rate de 9600
    configureSerialPort(B9600);
    return 1;
}

void disconnect_uart()
{
    close(fd);
}

#endif

int main()
{
    int tx_bytes;
    char char_tx;
    char data[2];
    char buffer[1];
    char choice;
    char exit;
    int bytes_qty;
    int count_bytes_seq_tx = 0; // Registra a quantidade de caracteres enviados em sequencia
    int count_bytes_seq_rx = 0; // Registra a quantidade de caracteres recebidos em sequencia
    char message[100];

    printf("\nConfigurando UART...");
    if (configure_uart())
        printf("\nUART configurada!");
    else {
        printf("\nFalha ao configurar UART!");
        return 1;
    }

    do {
        choice = dialog("\nInsira 't' para enviar algum byte ou 'r' para receber algum byte: ", 't', 'r');
        if (choice == 't') {
            count_bytes_seq_rx = 0;
            // printf("\nInsira o %dº caractere a ser enviado para a FPGA: ", ++count_bytes_seq_tx);
            // data[0] = input_char();
            sprintf(message, "\nInsira o %dº caractere (em HEXADECIMAL) a ser enviado para a FPGA: ", ++count_bytes_seq_tx);
            input_x(message, &tx_bytes, 8);
            printf("\nEnviando '0x%X' para a FPGA...", tx_bytes);
            char_tx = (char) tx_bytes;
            sent_data(&char_tx);
            printf("\nCaractere '%c' enviado com sucesso!", char_tx);
        } else if (choice == 'r') {
            printf("\nAguardando resposta da FPGA...");
            bytes_qty = receive_data(buffer);
            if (bytes_qty > 0)
            {
                buffer[bytes_qty] = '\0';
                printf("\n%dº byte recebido: %s", ++count_bytes_seq_rx, buffer);
                // printf("\nQuantidade de bytes recebidos: %d", bytes_qty);
            }
            else
            {
                printf("\nNenhum foi recebido!");
            }
        }
        exit = dialog("\nInsira 's' para realizar outra ação ou 'n' para finalizar o programa: ", 's', 'n');
    } while (exit == 's');

    printf("\nEncerrando conexão... Pressione Enter para sair do programa...\n");
    while (input_char() != 13)
    {
    };

    // Fechar a porta serial
    disconnect_uart();

    return 0;
}
