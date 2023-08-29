#include "models.h"
#include "../helpers/helpers.h"
#include <stdio.h>
#include <unistd.h>

#define PROTOCOL_START_CODE 0xff
#define PROTOCOL_END_CODE 0x00
#define READ_SITUATION_COMMAND 0x00
#define READ_TEMPERATURE_COMMAND 0x01
#define READ_HUMIDITY_COMMAND 0x02

short int in_protocol = 0;

int can_start_protocol()
{
    return in_protocol;
}

void start_protocol(struct Sensor *sensor)
{
    tx_hex(PROTOCOL_START_CODE);
    in_protocol = 1;
    sensor->in_use = 1;
}

void end_protocol(struct Sensor *sensor)
{
    tx_hex(PROTOCOL_END_CODE);
    in_protocol = 0;
    sensor->in_use = 0;
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

void get_sensor_temperature(struct Sensor *sensor)
{
    int integer;
    int decimal;
    int situation;
    start_protocol(sensor);
    tx_hex(sensor->address);
    tx_hex(READ_TEMPERATURE_COMMAND);
    integer = rx_int();
    decimal = rx_int();
    situation = rx_int();
    end_protocol(sensor);
    sensor->temperature = build_float(integer, decimal);
    if (situation != 0x07)
        sensor->working = 0;
    else
        sensor->working = 1;
}

void get_sensor_humidity(struct Sensor *sensor)
{
    int integer;
    int decimal;
    int situation;
    start_protocol(sensor);
    tx_hex(sensor->address);
    tx_hex(READ_HUMIDITY_COMMAND);
    integer = rx_int();
    decimal = rx_int();
    end_protocol(sensor);
    situation = rx_int();
    sensor->humidity = build_float(integer, decimal);
    if (situation != 0x07)
        sensor->working = 0;
    else
        sensor->working = 1;
}

void get_sensor_situation(struct Sensor *sensor)
{
    int situation;
    start_protocol(sensor);
    tx_hex(sensor->address);
    tx_hex(READ_SITUATION_COMMAND);
    situation = rx_int();
    end_protocol(sensor);
    if (situation != 0x07)
        sensor->working = 0;
    else
        sensor->working = 1;
}

void toggle_continuos_monitoring(struct Sensor *sensor)
{
    start_protocol(sensor);
    tx_hex(sensor->address);
    tx_hex(sensor->command);
    end_protocol(sensor);
}

void register_command(unsigned int address, unsigned int command)
{
    sensors[address].in_use = 1;
    sensors[address].address = address;
    sensors[address].command = command;
}