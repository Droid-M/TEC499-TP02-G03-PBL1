#include <stdio.h>
#include <unistd.h>

#include "./helpers/helpers.h"
#include "./models/models.h"

void menu()
{
    struct Sensor sensors[32];
    slowed_printf("Sistema de gestão de sensoriamento - Versão 1.0", 0.2);
    while(!kbhit()) {
        for(int i = 0; i < sizeof(sensors); i++) {
            printf("Informações do sensor %d\n", i);
            printf(
                "Endereço: %d ---- Comando enviado: %d ---- Valor recebido: %d", 
                sensors[i].address,
                sensors[i].command,
                select_sensor_value(sensors[i])
            );
        }
    }
}

int main()
{
    menu();
    return 0;
}