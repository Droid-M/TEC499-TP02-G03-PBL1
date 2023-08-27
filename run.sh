#!/bin/bash

# Compilar funções ajudantes
gcc -c C/helpers/input_helpers.c -o input_helpers.o
gcc -c C/helpers/output_helpers.c -o output_helpers.o

# Compilar programa principal
gcc -c C/menu.c -o menu.o

# Vincular os arquivos objetos e criar o executavel
gcc input_helpers.o output_helpers.o  menu.o -o menu

# Executar o executavel
./menu

# Limpar os arquivos objetos e executavel
rm input_helpers.o menu.o menu
