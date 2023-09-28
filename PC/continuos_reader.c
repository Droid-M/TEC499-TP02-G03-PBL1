#include "./helpers/helpers.h"
#include "./models/models.h"
#include <stdio.h>
#include <unistd.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>

struct Sensor sensors[32];

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Quantidade de argumentos é insuficiente para prosseguir!");
        pause_program("pressione Enter para sair...");
        return 1;
    }
    init_shared_memory();
    int sensor_address = atoi(argv[1]);
    int sensor_command = atoi(argv[2]);
    time_t currentTime;
    struct tm *localTime;

    // Registra o comando recebido para um sensor específico
    register_command(sensor_address, sensor_command);

    if (sensor_command == 0x07)
    {
        while (!key_has_pressed())
        {
            sleep(1);
            time(&currentTime); // Obtém o tempo atual
            localTime = localtime(&currentTime);
            
            // Obtém a temperatura do sensor e exibe
            get_sensor_temperature(&sensors[sensor_address - 1]);
            printf("\nInstante %02d:%02d:%02d ----- ", localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
            printf("Temperatura lida no sensor #%d: %.3f °C\n\n", sensors[sensor_address - 1].address, sensors[sensor_address - 1].temperature);
        }
    }
    else if (sensor_command == 0x08)
    {
        while (!key_has_pressed())
        {
            sleep(1);
            time(&currentTime); // Obtém o tempo atual
            localTime = localtime(&currentTime);

            // Obtém a umidade do sensor e exibe
            get_sensor_humidity(&sensors[sensor_address - 1]);
            printf("\nInstante %02d:%02d:%02d ----- ", localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
            printf("Umidade lida no sensor #%d: %.3f%%\n\n", sensors[sensor_address - 1].address, sensors[sensor_address - 1].humidity);
        }
    }
    else
    {
        fprintf(stderr, "O comando enviado não é reconhecido!");
        pause_program("pressione Enter para sair...");
        return 1;
    }

    // Aguarda a entrada do usuário para sair
    pause_program("pressione Enter para sair...");

    // Fecha a memória compartilhada antes de sair
    close_shared_memory();
    return 0;
}
