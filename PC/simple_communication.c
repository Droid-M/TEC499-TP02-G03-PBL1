#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "./helpers/helpers.h"
#include "./models/models.h"

int main()
{
    int tx_bytes;
    char data[2];
    char buffer[1];
    char choice;
    char exit;
    int bytes_read;
    int count_bytes_seq_tx = 0; // Registra a quantidade de caracteres enviados em sequencia
    int count_bytes_seq_rx = 0; // Registra a quantidade de caracteres recebidos em sequencia
    char message[100];

    printf("\nConfigurando UART...");
    if (open_connection())
        printf("\nUART configurada!");
    else {
        printf("\nFalha ao configurar UART!");
        return 1;
    }

    do {
        choice = dialog("\nInsira 't' para enviar algum byte ou 'r' para receber algum byte: ", 't', 'r');
        if (choice == 't') {
            count_bytes_seq_rx = 0;
            // Solicita o byte a ser enviado em formato hexadecimal
            sprintf(message, "\nInsira o %dº caractere (em HEXADECIMAL) a ser enviado para a FPGA: ", ++count_bytes_seq_tx);
            input_x(message, &tx_bytes, 8);
            printf("\nEnviando '0x%X' (%d em decimal) para a FPGA...", tx_bytes, tx_bytes);
            tx_hex(tx_bytes);
            printf("\nCaractere '%c' enviado com sucesso!", (char) tx_bytes);
        } else if (choice == 'r') {
            printf("\nAguardando resposta da FPGA...");
            bytes_read = rx_int();
            if (bytes_read != -1)
            {
                printf("\n%dº byte recebido (em decimal): %d", ++count_bytes_seq_rx, bytes_read);
            }
            else
            {
                printf("\nNenhum byte foi recebido!");
            }
        }
        exit = dialog("\nInsira 's' para realizar outra ação ou 'n' para finalizar o programa: ", 's', 'n');
    } while (exit == 's');

    printf("\nEncerrando conexão... Pressione Enter para sair do programa...\n");
    while (input_char() != 13)
    {
    };

    // Fechar a porta serial
    close_connection();

    return 0;
}