#include "./helpers/helpers.h"
#include "./models/models.h"

#include <stdio.h>
#include <unistd.h>
#include <locale.h>

#define H 0x48 // 72 em decimal
#define h 0x68 // 104 em decimal

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
}

int manage_sensor_menu()
{
    unsigned int sensor_address;
    unsigned int sensor_command = 0;
    // input("Informe o endereço do sensor (entre 1 e 32):", "%x", &sensor_address);
    input_x("Informe o endereço do sensor (entre 1 e 32):", &sensor_address, 16);
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
        input_x("Informe o comando a ser enviado ao sensor (ou, insira '0x48' sem aspas para visualizar a lista de comandos disponíveis):", &sensor_command, 16);
        if (sensor_command != H && sensor_command != h && (sensor_command < 0x01 || sensor_command > 0x06))
        {
            printf("Selecionou: %x\n\n", sensor_command);
            fprintf(stderr, "Opção inválida! Retornando ao menu anterior...");
            skip_lines(8, 0, "");
            return 1;
        }
    } while (sensor_command == H || sensor_command == h);

    register_command(sensor_address, sensor_command);

    switch (sensor_command)
    {
        case 0x00:
            get_sensor_situation(sensors[sensor_address]);
            if (sensors[sensor_address].working)
                printf("Situação do sensor #%d: FUNCIONANDO NORMALMENTE :)", sensors[sensor_address].address);
            else
                printf("Situação do sensor #%d: COM PROBLEMAS :(", sensors[sensor_address].address);
            break;
        case 0x01:
            get_sensor_temperature(sensors[sensor_address]);
            printf("Temperatura do sensor #%d: %.3f°C", sensors[sensor_address].address, sensors[sensor_address].temperature);
            break;
        case 0x02:
            get_sensor_humidity(sensors[sensor_address]);
            printf("Umidade do sensor #%d: %.2f%", sensors[sensor_address].address, sensors[sensor_address].humidity);
            break;
        case 0x03:
            break;
    }
    return 0;
}

void menu()
{
    setlocale(LC_ALL, "Portuguese"); // Configuração regional para o português brasileiro em UTF-8
    int choice = 0;
    do
    {
        printf("Insira o valor correspondente à opção que deseja selecionar:");
        skip_lines(4, 0, "");
        slowed_printf("1 - Enviar comando para sensor\n\n", 0.1);
        slowed_printf("2 - Sair do programa\n\n", 0.1);
        // input("Opção escolhida: ", "%d", &choice);
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
    open_connection();
    menu();
    close_connection();
    return 0;
}