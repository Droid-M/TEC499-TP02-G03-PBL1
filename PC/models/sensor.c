#include "models.h"
#include <stdio.h>
#include <unistd.h>

int select_sensor_value(struct Sensor sensor)
{
    switch (sensor.command)
    {
    case 0x00:
        return 0;
        break;
    }
}

void manage_sensor(unsigned int address, unsigned int command)
{
    printf(address, command);
}