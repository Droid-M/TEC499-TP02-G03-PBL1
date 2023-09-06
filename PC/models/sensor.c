#include "models.h"
#include "../helpers/helpers.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define PROTOCOL_START_CODE 0xff
#define PROTOCOL_END_CODE 0x00
#define READ_SITUATION_COMMAND 0x00
#define READ_TEMPERATURE_COMMAND 0x01
#define READ_HUMIDITY_COMMAND 0x02
#define UART_HAS_FREE "UART_HAS_FREE"
#define UART_HAS_BUSY "UART_HAS_BUSY"

short int in_protocol = 0;

void check_response(struct Sensor *sensor, int header, int status, int command, int footer, int address)
{
    sensor->error_in_response = (int)(header != 0xFF || status != 0x01 || footer != 0x7F || sensor->command != command || sensor->address != address);
}

int can_start_protocol()
{
    return in_protocol;
}

void start_protocol(struct Sensor *sensor)
{
    while (strcmp(get_shared_value(), UART_HAS_BUSY) == 0)
    {
        // aguarda a liberação da UART por outros programas
    }
    write_in_shared_memory(UART_HAS_BUSY);
    tx_hex(PROTOCOL_START_CODE);
    in_protocol = 1;
    sensor->in_use = 1;
}

void end_protocol(struct Sensor *sensor)
{
    tx_hex(PROTOCOL_END_CODE);
    in_protocol = 0;
    sensor->in_use = 0;
    if (strcmp(get_shared_value(), UART_HAS_BUSY) == 0)
    {
        write_in_shared_memory(UART_HAS_FREE);
    }
}

int select_sensor_value(struct Sensor *sensor)
{
    switch (sensor->command)
    {
    case 0x00:
        return 0;
        break;
    }
}

struct Sensor *get_sensors()
{
    return sensors;
}

void sent_request_package(struct Sensor *sensor)
{
    /* -------------------------- Início do Pacote de requisição -------------------------- */
    start_protocol(sensor);
    tx_hex(sensor->address);
    tx_hex(sensor->command);
    end_protocol(sensor);
    /* ------------------------ Fim do Pacote requisição ------------------------ */
}

int *receive_response_package(struct Sensor *sensor)
{
    int header;
    int status;
    int address;
    int command;
    int *data = (int *)malloc(3 * sizeof(int)); // Aloca memória para o array "data"
    int footer;
    /* ---------------------- Início do Pacote de resposta ---------------------- */
    header = rx_int();
    status = rx_int();
    address = rx_int();
    command = rx_int();
    data[0] = rx_int();
    data[1] = rx_int();
    data[2] = rx_int(); // situation
    footer = rx_int();
    /* ------------------------ Fim do pacote de resposta ----------------------- */
    check_response(sensor, header, status, command, footer, address);
    return data;
}

short int has_integrity(struct Sensor *sensor)
{
    // IMPORTANT - Calcular Checksum ou CRC aqui caso seja implantado no protocolo...
    return (int)!sensor->error_in_request;
    // return (int)!sensor->error_in_request && sensor->working;
}

void get_sensor_temperature(struct Sensor *sensor)
{
    sent_request_package(sensor);
    int *data = receive_response_package(sensor);
    if (has_integrity(sensor))
        sensor->temperature = build_float(data[0], data[1]);
}

void get_sensor_humidity(struct Sensor *sensor)
{
    sent_request_package(sensor);
    int *data = receive_response_package(sensor);
    if (has_integrity(sensor))
        sensor->humidity = build_float(data[0], data[1]);
}

void get_sensor_situation(struct Sensor *sensor)
{
    sent_request_package(sensor);
    int *data = receive_response_package(sensor);
    if (has_integrity(sensor))
        sensor->working = data[0] == 0x07;
}

short int toggle_continuos_monitoring(struct Sensor *sensor)
{
    sent_request_package(sensor);
    receive_response_package(sensor);
    return has_integrity(sensor);
}

void register_command(unsigned int address, unsigned int command)
{
    sensors[address - 1].in_use = 1;
    sensors[address - 1].address = address;
    sensors[address - 1].command = command;
    sensors[address - 1].error_in_request = 0;
    sensors[address - 1].error_in_response = 0;
}