#include <msp430.h>

void uart_init(void);
void uart_send_byte(unsigned char byte);
void uart_send_data(unsigned char *data, unsigned int size);
void uart_send_string(const char *str);
