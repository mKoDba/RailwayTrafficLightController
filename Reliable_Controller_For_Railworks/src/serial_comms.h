
#include <avr/io.h>

void uart_putchar(char c, FILE *stream);
char uart_getchar(FILE *stream);

void uart_init(void);

FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
FILE uart_input = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);

/* i2c communication header files */
uint8_t i2c_start(uint8_t slave_address);
void i2c_stop(void);

uint8_t i2c_write(uint8_t data);

uint8_t i2c_read_ack(void);
uint8_t i2c_read_nack(void);
