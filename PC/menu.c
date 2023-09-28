#include "./helpers/helpers.h"
#include "./models/models.h"
#include <stdio.h>
#include <unistd.h>
#include <locale.h>
#include <stdlib.h>

#define H 0x48 // 72 em decimal
#define h 0x68 // 104 em decimal
#define UART_HAS_FREE "UART_HAS_FREE"
#define UART_HAS_BUSY "UART_HAS_BUSY"

struct Sensor sensors[32];

void show_sensor_commands()
{
    printf("Código: 0x00 ---- Comando: Situação atual do sensor\n");
    printf("Código: 0x01 ---- Comando: Temperatura atual\n");
    printf("Código: 0x02 ---- Comando: Umidade atual\n");
    printf("Código: 0x03 ---- Comando: Ativa o sensoriamento contínuo de temperatura\n");
    printf("Código: 0x04 ---- Comando: Ativa o sensoriamento contínuo de umidade\n");
    printf("Código: 0x05 ---- Comando: Desativa o sensoriamento contínuo de temperatura\n");
    printf("Código: 0x06 ---- Comando: Desativa o sensoriamento contínuo de umidade\n");
    printf("Código: 0x07 ---- Comando: Exibe o sensoriamento contínuo de temperatura\n");
    printf("Código: 0x08 ---- Comando: Exibe o sensoriamento contínuo de umidade\n");
}

int manage_sensor_menu()
{
    unsigned int sensor_address;
    unsigned int sensor_command = 0;
    char command[100];
    const char *continuos_reader_exec_path = env("CONTINUOS_READER_EXECUTABLE_PATH");
    const char *linux_terminal = env("LINUX_TERMINAL");
    char *arguments[4] = {(char *) linux_terminal, "-e", "", NULL};

    // Solicita o endereço do sensor e verifica se está dentro do intervalo permitido
    input_x("Informe o endereço do sensor (entre 0x01 e 0x20): ", &sensor_address, 16);
    if (sensor_address < 0x01 || sensor_address > 0x20)
    {
        fprintf(stderr, "Opção inválida! Retornando ao menu anterior...");
        skip_lines(8, 0, "");
        return 1;
    }

    do
    {
        if (sensor_command == H || sensor_command == h)
            show_sensor_commands();

        // Solicita o comando a ser enviado ao sensor e verifica sua validade
        input_x("Informe o comando a ser enviado ao sensor (ou '0x48' para visualizar a lista de comandos disponíveis): ", &sensor_command, 16);
        if (sensor_command != H && sensor_command != h && (sensor_command < 0x00 || sensor_command > 0x08))
        {
            printf("Selecionou: %x\n\n", sensor_command);
            fprintf(stderr, "Opção inválida! Retornando ao menu anterior...");
            skip_lines(8, 0, "");
            return 1;
        }
    } while (sensor_command == H || sensor_command == h);

    // Registra o comando para o sensor selecionado
    register_command(sensor_address, sensor_command);

    switch (sensor_command)
    {
    case 0x00:
        // Obtém a situação do sensor
        get_sensor_situation(&sensors[sensor_address - 1]);
        if (sensors[sensor_address - 1].working)
            printf("Situação do sensor #%d: FUNCIONANDO NORMALMENTE :)", sensors[sensor_address - 1].address);
        else
            printf("Situação do sensor #%d: COM PROBLEMAS :(", sensors[sensor_address - 1].address);
        break;
    case 0x01:
        // Obtém a temperatura do sensor
        get_sensor_temperature(&sensors[sensor_address - 1]);
        printf("Temperatura lida no sensor #%d: %.3f °C", sensors[sensor_address - 1].address, sensors[sensor_address - 1].temperature);
        break;
    case 0x02:
        // Obtém a umidade do sensor
        get_sensor_humidity(&sensors[sensor_address - 1]);
        printf("Umidade lida no sensor #%d: %.2f%%", sensors[sensor_address - 1].address, sensors[sensor_address - 1].humidity);
        break;
    case 0x03:
    case 0x04:
    case 0x05:
    case 0x06:
        // Ativa ou desativa o sensoriamento contínuo de temperatura ou umidade
        toggle_continuos_monitoring(&sensors[sensor_address - 1]);
        if (sensor_command == 0x03)
            printf("Monitoramento contínuo de temperatura ativado para o sensor #%d", sensors[sensor_address - 1].address);
        else if (sensor_command == 0x04)
            printf("Monitoramento contínuo de umidade ativado para o sensor #%d", sensors[sensor_address - 1].address);
        else if (sensor_command == 0x05)
            printf("Monitoramento contínuo de temperatura desativado para o sensor #%d", sensors[sensor_address - 1].address);
        else if (sensor_command == 0x06)
            printf("Monitoramento contínuo de umidade desativado para o sensor #%d", sensors[sensor_address - 1].address);
        break;
    case 0x07:
    case 0x08:
        // Inicia um programa externo para exibir o sensoriamento contínuo de temperatura ou umidade
        #ifdef _WIN32
        sprintf(command, "start %s %d %d", continuos_reader_exec_path, sensor_address, sensor_command);
        system(command);
        #else
        sprintf(command, "./%s %d %d", continuos_reader_exec_path, sensor_address, sensor_command);
        arguments[2] = command;
        execute(linux_terminal, arguments);
        #endif
        break;
    default:
        printf("Comando '%d' inválido!", sensor_command);
        skip_lines(8, 0, "");
        return 1;
    }
    skip_lines(8, 0, "");
    return 0;
}

void menu()
{
    setlocale(LC_ALL, "Portuguese"); // Configuração regional para o português brasileiro em UTF-8
    int choice = 0;
    do
    {
        printf("Insira o valor correspondente à opção que deseja selecionar: ");
        skip_lines(4, 0, "");
        slowed_printf("1 - Enviar comando para sensor\n\n", 0.1);
        slowed_printf("2 - Sair do programa\n\n", 0.1);
        input_d("Opção escolhida: ", &choice, 5);
        if (choice == 1)
        {
            manage_sensor_menu();
        }
        else if (choice != 2)
        {
            fprintf(stderr, "Opção inválida! Insira apenas 1 ou 2!");
            skip_lines(4, 0, "");
        }
    } while (choice != 2);
}

int main()
{
    init_shared_memory();
    write_in_shared_memory(UART_HAS_FREE);
    open_connection();
    menu();
    close_connection();
    close_shared_memory();
    return 0;
}
