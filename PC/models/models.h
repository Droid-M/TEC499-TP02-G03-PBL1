#ifndef MODELS_H
#define MODELS_H

struct Sensor {
    int address;
    int command;
    short int working;
    float temperature;
    float humidity;
};

int select_sensor_value(struct Sensor sensor);
void manage_sensor(unsigned int address, unsigned int command);
#endif
