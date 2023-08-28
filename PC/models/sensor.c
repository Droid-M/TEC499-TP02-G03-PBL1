#include "models.h"
#include <stdio.h>
#include <unistd.h>

#define PROTOCOL_H1 0xff

int select_sensor_value(struct Sensor sensor)
{
    switch (sensor.command)
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

void get_sensor_situation(struct Sensor sensor)
{
    tx_hex(sensor.address);
    tx_hex(sensor.command);
    rx_char();
}

void manage_sensor(unsigned int address, unsigned int command)
{
    sensors[address].in_use = 1;
    sensors[address].address = address;
    sensors[address].command = command;
    switch (command)
    {
        case 0x00:
            get_sensor_situation(sensors[address]);
            break;
    }
}