#!/bin/bash

source config.env

echo "$IN_UART_SIMULATION"

echo "Removendo antigos programas compilados..."

# Remove antigas instâncias executáveis para evitar trava de sobrescrita
rm menu continuos_reader
rm simple_communication

echo "Compilando funções ajudantes..."

# Compilar funções ajudantes
gcc -c PC/helpers/time_helpers.c -o time_helpers.o
gcc -c PC/helpers/number_helpers.c -o number_helpers.o
gcc -c PC/helpers/file_helpers.c -o file_helpers.o
echo "Compilando funções de entidades/modelos e controle..."

# Compilar funções de entidades/modelos
gcc -c PC/models/uart/fake_uart.c -o fake_uart.o
gcc -c PC/models/uart/fake_uart.c -o linux_uart.o
gcc -c PC/models/uart/fake_uart.c -o windows_uart.o
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
if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then #Se o sistema operacional for Windows:
    if [ "$IN_UART_SIMULATION" == "0" ]; then
        gcc time_helpers.o file_helpers.o input_helpers.o output_helpers.o fake_uart.o sensor.o number_helpers.o semaphore.o menu.o -o menu
        gcc time_helpers.o file_helpers.o input_helpers.o output_helpers.o fake_uart.o sensor.o number_helpers.o semaphore.o menu.o -o menu
        gcc time_helpers.o file_helpers.o input_helpers.o output_helpers.o fake_uart.o sensor.o number_helpers.o semaphore.o continuos_reader.o -o continuos_reader
        gcc time_helpers.o file_helpers.o input_helpers.o output_helpers.o number_helpers.o simple_communication.o -o simple_communication
    else
        gcc time_helpers.o file_helpers.o input_helpers.o output_helpers.o windows_uart.o sensor.o number_helpers.o semaphore.o menu.o -o menu
        gcc time_helpers.o file_helpers.o input_helpers.o output_helpers.o windows_uart.o sensor.o number_helpers.o semaphore.o menu.o -o menu
        gcc time_helpers.o file_helpers.o input_helpers.o output_helpers.o windows_uart.o sensor.o number_helpers.o semaphore.o continuos_reader.o -o continuos_reader
        gcc time_helpers.o file_helpers.o input_helpers.o output_helpers.o number_helpers.o simple_communication.o -o simple_communication
    fi
else #Se o sistema operacional não for Windows:
    gcc -c PC/helpers/linux/input_helpers.c -o input_helpers.o
    gcc -c PC/helpers/linux/output_helpers.c -o output_helpers.o
    if [ "$IN_UART_SIMULATION" == "0" ]; then
        gcc time_helpers.o file_helpers.o input_helpers.o output_helpers.o fake_uart.o sensor.o number_helpers.o semaphore.o menu.o -o menu -lm -lncurses
        gcc time_helpers.o file_helpers.o input_helpers.o output_helpers.o fake_uart.o sensor.o number_helpers.o semaphore.o menu.o -o menu -lm -lncurses
        gcc time_helpers.o file_helpers.o input_helpers.o output_helpers.o fake_uart.o sensor.o number_helpers.o semaphore.o continuos_reader.o -o continuos_reader -lm -lncurses
        gcc time_helpers.o file_helpers.o input_helpers.o output_helpers.o number_helpers.o simple_communication.o -o simple_communication -lm -lncurses
    else
        gcc time_helpers.o file_helpers.o input_helpers.o output_helpers.o linux_uart.o sensor.o number_helpers.o semaphore.o menu.o -o menu -lm -lncurses
        gcc time_helpers.o file_helpers.o input_helpers.o output_helpers.o linux_uart.o sensor.o number_helpers.o semaphore.o menu.o -o menu -lm -lncurses
        gcc time_helpers.o file_helpers.o input_helpers.o output_helpers.o linux_uart.o sensor.o number_helpers.o semaphore.o continuos_reader.o -o continuos_reader -lm -lncurses
        gcc time_helpers.o file_helpers.o input_helpers.o output_helpers.o number_helpers.o simple_communication.o -o simple_communication -lm -lncurses
    fi
fi

echo "Removendo arquivos de sobras..."

# Limpa os arquivos de objetos executáveis
rm input_helpers.o menu.o output_helpers.o fake_uart.o linux_uart.o windows_uart.o sensor.o number_helpers.o time_helpers.o continuos_reader.o semaphore.o  #("descomente" para permitir a reciclagem automática)
rm simple_communication.o

echo "Programa compilado com sucesso!"
sleep 1


# echo "Executando programa..."

# Executar o executável
# ./menu #("descomente" esta linha para executar o programa automaticamente)

echo "Pressione ENTER para continuar..."
read