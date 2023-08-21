#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

void configureSerialPort(int fd, speed_t baud_rate)
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

int main()
{
    int fd;
    fd = open("/dev/ttyS0", O_RDWR); // Substitua pelo dispositivo correto (ex: /dev/ttyUSB0)

    if (fd == -1)
    {
        perror("Erro ao abrir a porta serial");
        return 1;
    }

    // Configurar a porta serial com baud rate de 9600
    configureSerialPort(fd, B9600);

    //Enviando sinal de start
    // Valor estático a ser enviado como start bit (0)
    unsigned char start_bit = 0;

    // Enviar o valor estático como start bit
    write(fd, &start_bit, 1);

    char data[] = "H";

    // Enviar os dados para a porta serial
    write(fd, data, strlen(data));

    // Aguardar um atraso antes de ler a resposta
    usleep(100000); // Atraso de 100 milissegundos

    // Receber dados da porta serial
    char buffer[32];
    int numBytes = read(fd, buffer, sizeof(buffer));
    if (numBytes > 0) {
        buffer[numBytes] = '\0';
        printf("Dados recebidos: %s\n", buffer);
    }

    // Fechar a porta serial
    close(fd);

    return 0;
}
