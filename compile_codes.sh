#!/bin/bash

echo "Removendo antigos programas compilados..."

# Remove antigas instâncias executáveis para evitar trava de sobrescrita
rm menu continuos_reader
rm simple_communication

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
gcc -c PC/models/semaphore.c -o semaphore.o

echo "Compilando programa principal e auxiliares..."

# Compilar programa principal
gcc -c PC/menu.c -o menu.o
# Compilar os programas auxiliares
gcc -c PC/continuos_reader.c -o continuos_reader.o
gcc -c PC/simple_communication.c -o simple_communication.o

echo "Ligando arquivos de objeto e criado executáveis..."

# Vincular os arquivos objetos e criar o executável
gcc time_helpers.o input_helpers.o output_helpers.o sensor.o uart.o number_helpers.o semaphore.o menu.o -o menu -lm
gcc time_helpers.o input_helpers.o output_helpers.o sensor.o uart.o number_helpers.o semaphore.o continuos_reader.o -o continuos_reader -lm
gcc time_helpers.o input_helpers.o output_helpers.o number_helpers.o simple_communication.o -o simple_communication -lm

echo "Removendo arquivos de sobras..."

# Limpa os arquivos de objetos executáveis
rm input_helpers.o menu.o output_helpers.o sensor.o uart.o number_helpers.o time_helpers.o continuos_reader.o semaphore.o  #("descomente" para permitir a reciclagem automática)
rm simple_communication.o

echo "Programa compilado com sucesso!"
sleep 1


# echo "Executando programa..."

# Executar o executável
# ./menu #("descomente" esta linha para executar o programa automaticamente)

echo "Pressione ENTER para continuar..."
read