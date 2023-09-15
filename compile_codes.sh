#!/bin/bash

source config.env

# Remove antigas instâncias executáveis para evitar trava de sobrescrita
echo "Removendo antigos programas compilados..."
rm PC/Executables/menu PC/Executables/continuos_reader PC/Executables/simple_communication

# Compilar funções ajudantes
echo "Compilando funções ajudantes..."
gcc -c PC/helpers/time_helpers.c -o time_helpers.o
gcc -c PC/helpers/number_helpers.c -o number_helpers.o
gcc -c PC/helpers/file_helpers.c -o file_helpers.o

# Compilar funções de entidades/modelos
echo "Compilando funções de entidades/modelos e controle..."
gcc -c PC/models/sensor.c -o sensor.o

echo "Compilando programa principal e auxiliares..."

# Compilar programa principal
gcc -c PC/menu.c -o menu.o
# Compilar os programas auxiliares
gcc -c PC/continuos_reader.c -o continuos_reader.o
gcc -c PC/simple_communication.c -o simple_communication.o

echo "Ligando arquivos de objeto e criado executáveis..."

if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then #Se o sistema operacional for Windows:
    # Compilar funções ajudantes
    gcc -c PC/helpers/windows/input_helpers.c -o input_helpers.o
    gcc -c PC/helpers/windows/output_helpers.c -o output_helpers.o
    # Compilar funções de entidades/modelos
    gcc -c PC/models/windows/semaphore.c -o semaphore.o
    gcc -c PC/models/windows/uart.c -o uart.o
    # Vincular os arquivos objetos e criar o executável
    gcc time_helpers.o file_helpers.o input_helpers.o output_helpers.o uart.o sensor.o number_helpers.o semaphore.o menu.o -o PC/Executables/menu
    gcc time_helpers.o file_helpers.o input_helpers.o output_helpers.o uart.o sensor.o number_helpers.o semaphore.o continuos_reader.o -o PC/Executables/continuos_reader
    gcc time_helpers.o file_helpers.o input_helpers.o output_helpers.o number_helpers.o simple_communication.o -o PC/Executables/simple_communication
else #Se o sistema operacional não for Windows:
    # Compilar funções ajudantes
    gcc -c PC/helpers/linux/input_helpers.c -o input_helpers.o
    gcc -c PC/helpers/linux/output_helpers.c -o output_helpers.o
    # Compilar funções de entidades/modelos
    gcc -c PC/models/linux/semaphore.c -o semaphore.o
    if [ "$IN_UART_SIMULATION" == "0" ]; then
        gcc -c PC/models/linux/fake_uart.c -o uart.o
    else
        gcc -c PC/models/linux/uart.c -o uart.o
    fi
    # Vincular os arquivos objetos e criar o executável
    gcc time_helpers.o file_helpers.o input_helpers.o output_helpers.o uart.o sensor.o number_helpers.o semaphore.o menu.o -o PC/Executables/menu -lm -lncurses
    gcc time_helpers.o file_helpers.o input_helpers.o output_helpers.o uart.o sensor.o number_helpers.o semaphore.o continuos_reader.o -o PC/Executables/continuos_reader -lm -lncurses
    gcc time_helpers.o file_helpers.o input_helpers.o output_helpers.o number_helpers.o simple_communication.o -o PC/Executables/simple_communication -lm -lncurses
fi

echo "Removendo arquivos de sobras..."

# Limpa os arquivos de objetos executáveis
rm input_helpers.o menu.o output_helpers.o uart.o sensor.o number_helpers.o time_helpers.o continuos_reader.o semaphore.o file_helpers.o  #("descomente" para permitir a reciclagem automática)
rm simple_communication.o

echo "Programa compilado com sucesso!"
sleep 1


# echo "Executando programa..."

# Executar o executável
# ./menu #("descomente" esta linha para executar o programa automaticamente)

echo "Pressione ENTER para continuar..."
read