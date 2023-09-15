#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *env(const char *key)
{
    FILE *file;
    char line[100]; // Assumindo que cada linha do arquivo terá no máximo 100 caracteres

    // Abre o arquivo "config.env" para leitura
    file = fopen("config.env", "r");

    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo");
        return NULL; // Retorna NULL em caso de erro
    }

    // Lê o arquivo linha por linha
    while (fgets(line, sizeof(line), file))
    {
        // Divide a linha em nome e valor da variável
        char *name = strtok(line, "=");
        char *value = strtok(NULL, "\n");

        if (name != NULL && value != NULL)
        {
            // Remove espaços em branco extras do valor (se houver)
            char *trimmed_value = strtok(value, " \t\n\r");

            // Verifica se o nome da variável corresponde ao nome procurado
            if (strcmp(name, key) == 0)
            {
                // Fecha o arquivo e retorna o valor da variável
                fclose(file);
                return trimmed_value;
            }
        }
    }

    // Fecha o arquivo
    fclose(file);

    // Retorna NULL se a variável não for encontrada
    return NULL;
}