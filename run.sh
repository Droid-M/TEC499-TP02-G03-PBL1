#!/bin/bash

echo "Removendo antigo programa compilado..."

# Remove instância executável antiga para evitar trava de sobrescrita
rm menu

echo "Compilando funções ajudantes..."

# Compilar funções ajudantes
gcc -c PC/helpers/time_helpers.c -o time_helpers.o
gcc -c PC/helpers/input_helpers.c -o input_helpers.o
gcc -c PC/helpers/output_helpers.c -o output_helpers.o
gcc -c PC/helpers/number_helpers.c -o number_helpers.o

echo "Compilando funções de entidades/modelos e controle..."

# Compilar funções de entidades/modelos
gcc -c PC/models/uart.c -o uart.o
gcc -c PC/models/sensor.c -o sensor.o

echo "Compilando programa principal..."

# Compilar programa principal
gcc -c PC/menu.c -o menu.o

echo "Linkando arquivos de objeto e criado executável..."

# Vincular os arquivos objetos e criar o executável
gcc input_helpers.o output_helpers.o sensor.o uart.o number_helpers.o time_helpers.o menu.o -o menu

# echo "Executando..."

# Executar o executável
# ./menu #("descomente" esta linha para executar o programa automaticamente)

# echo "Removendo arquivos sobras..."

# Limpar os arquivos objetos e executável
# rm input_helpers.o menu.o output_helpers.o sensor.o uart.o number_helpers.o time_helpers.o  #("descomente" para permitir a reciclagem automática)

echo "Sucesso!!!"