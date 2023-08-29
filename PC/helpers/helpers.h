#ifndef HELPERS // Aplicando "include guards"
#define HELPERS

// time_helpers.c
int sleep_nanos(unsigned long long ns);
int sleep_micros(unsigned long long us);

// number_helpers.c
int hex_to_decimal(int hex_value);
float build_float(int integer, int decimal);
int random_decimal(int max_value);
int random_hexa(int hexa_digits_qty);

// input_helpers.c
int key_has_pressed();
void pause_program(char *message);
void input(char *message, const char *format, ...);
void input_d(char *message, int *buffer, int max_size);
void input_x(char *message, int *buffer, int max_size);

// output_helpers.c
void slowed_printf(char *to_print, float delay_seconds);
void skip_lines(int qty, int delay_per_line, char *line_content);
// void c_log(const char *to_output, ...);
void clear_input_buffer();
void clear_console();

#endif