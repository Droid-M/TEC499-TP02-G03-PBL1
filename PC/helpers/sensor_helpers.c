#include "../models/models.h"
#include "helpers.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int in_protocol;

void check_response(struct Sensor *sensor, int header, int status, int command, int footer, int address)
{
    sensor->error_in_response = (int) (header != 0xFF || status != 0x01 || footer != 0x7F || sensor->command != command || sensor->address != address);
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