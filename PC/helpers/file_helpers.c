#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *env(const char *key)
{
    FILE *file;
    char line[1000]; // Tamanho máximo da linha aumentado para 1000 caracteres

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
        // Remove espaços em branco no início e no final da linha
        char *trimmed_line = strtok(line, " \t\n\r");

        // Verifica se a linha não está vazia
        if (trimmed_line != NULL && trimmed_line[0] != '#')
        { // Ignora linhas de comentário

            // Divide a linha em nome e valor da variável
            char *name = strtok(trimmed_line, "=");
            char *value = strtok(NULL, "\n");

            if (name != NULL && value != NULL)
            {
                // Verifica se o nome da variável corresponde ao nome procurado
                if (strcmp(name, key) == 0)
                {
                    // Fecha o arquivo e retorna o valor da variável
                    fclose(file);
                    return value;
                }
            }
        }
    }

    // Fecha o arquivo
    fclose(file);

    // Retorna NULL se a variável não for encontrada
    return NULL;
}