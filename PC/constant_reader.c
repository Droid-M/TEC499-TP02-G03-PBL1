#include "./helpers/helpers.h"
#include "./models/models.h"

#include <stdio.h>
#include <unistd.h>
#include <locale.h>

void reader(struct Sensor *sensors)
{
    setlocale(LC_ALL, "Portuguese"); // Configuração regional para o português brasileiro em UTF-8
    slowed_printf("Sistema de gestão de sensoriamento - Versão 1.0", 0.2);
    skip_lines(5, 1, ".");
    while(!key_has_pressed()) {
        for(int i = 0; i < sizeof(sensors); i++) {
            printf("Informações do sensor %d:\n", i);
            printf(
                "\tEndereço: %d ---- Comando enviado: %d ---- Valor recebido: %d", 
                sensors[i].address,
                sensors[i].command,
                select_sensor_value(sensors[i])
            );
            skip_lines(5, 1, ".");
        }
    }
}

int main()
{
    menu();
    return 0;
}