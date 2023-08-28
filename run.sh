#!/bin/bash

# Remove instância executável antiga para evitar trava de sobrescrita
rm menu

# Compilar funções ajudantes
gcc -c PC/helpers/input_helpers.c -o input_helpers.o
gcc -c PC/helpers/output_helpers.c -o output_helpers.o
gcc -c PC/helpers/number_helpers.c -o number_helpers.o

# Compilar funções de entidades/modelos
gcc -c PC/models/sensor.c -o sensor.o

# Compilar programa principal
gcc -c PC/menu.c -o menu.o

# Vincular os arquivos objetos e criar o executável
gcc input_helpers.o output_helpers.o sensor.o number_helpers.o menu.o -o menu

# Executar o executável
# ./menu ("descomente" esta linha para executar o programa automaticamente)

# Limpar os arquivos objetos e executável
# rm input_helpers.o menu.o output_helpers.o sensor.o number_helpers.o ("descomente" para permitir a reciclagem automática)
