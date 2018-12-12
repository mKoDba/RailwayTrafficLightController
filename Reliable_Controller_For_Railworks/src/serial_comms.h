
#include <avr/io.h>

void uart_putchar(char c, FILE *stream);
char uart_getchar(FILE *stream);
void uart_getstring(char * name);


void uart_init(void);

FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
FILE uart_input = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);