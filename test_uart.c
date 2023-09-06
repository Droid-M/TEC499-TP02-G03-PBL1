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

    char data = (char) 0x41;
    
    printf("\nSe preparando para enviar '%c'...\n", data);
    fflush(stdout);

    // Enviar os dados para a porta serial
    write(fd, &data, 1);
    
    printf("\nValor '%c' enviado! Aguardando resposta...\n", data);
    fflush(stdout);

    // Aguardar um atraso antes de ler a resposta
    sleep(1); // Atraso 1 segundo
    
    // Receber dados da porta serial
    char buffer[32];
    int numBytes = read(fd, buffer, sizeof(buffer));
    // int numBytes = 0x01; //<-- apenas para testar a executação
    if (numBytes > 0) {
        buffer[numBytes] = '\0';
        printf("\nDados recebidos: %s\n", buffer);
        fflush(stdout);
    } else {
	printf("\nNenhum dado foi recebido!\n");
	fflush(stdout);
    }

    // Fechar a porta serial
    close(fd);
    
    printf("\nConexão fechada. Pression Enter para sair...");

    scanf("%c", &data);
    return 0;
}
