#ifndef MODELS_H
#define MODELS_H

struct Sensor {
    int address;
    int command;
    short int working;
    float temperature;
    float humidity;
    short int in_use;
};

extern struct Sensor sensors[32];

extern short int in_protocol;

int select_sensor_value(struct Sensor sensor);
void manage_sensor(unsigned int address, unsigned int command);
void open_connection();
void close_connection();
char *rx_char();
void tx_char(char *data);
void tx_hex(unsigned int hex_value);
int can_start_protocol();
#endif
