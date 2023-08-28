#ifndef HELPERS // Aplicando "include guards"
#define HELPERS

int key_has_pressed();
void slowed_printf(char *to_print, float delay_seconds);
void skip_lines(int qty, int delay_per_line, char *line_content);
// void c_log(const char *to_output, ...);
void input(char *message, const char *format, ...);
void input_d(char *message, int *buffer, int max_size);
void input_x(char *message, int *buffer, int max_size);
void clear_input_buffer();
void clear_console();
int hex_to_decimal(int hex_value);
#endif