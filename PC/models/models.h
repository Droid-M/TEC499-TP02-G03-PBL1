#ifndef MODELS_H
#define MODELS_H

struct Sensor
{
    int address;
    int command;
    short int working;
    float temperature;
    float humidity;
    short int in_use;
    int error_in_request;
    int error_in_response;
};

extern struct Sensor sensors[32];

extern short int in_protocol;
// uart
int open_connection();
void close_connection();
char *rx_char();
int rx_int();
void tx_char(char *data);
void tx_hex(unsigned int hex_value);

// sensor.c
int can_start_protocol();
void register_command(unsigned int address, unsigned int command);
void get_sensor_temperature(struct Sensor *sensor);
void get_sensor_humidity(struct Sensor *sensor);
void get_sensor_situation(struct Sensor *sensor);
short int toggle_continuos_monitoring(struct Sensor *sensor);

// semaphore.c

void init_shared_memory();
void write_in_shared_memory(const char *message);
char *get_shared_value();
void close_shared_memory();
#endif
