/*********************************************************************************
 *                                                                               *
 *               Serial Communication                                            *
 *                                                                               *
 *                                                                               *
 *               Code guide: NASA C style guide 1994                             *
 *                                                                               *
 *               Authors: Joel Matos Vaz (joelmatosvaz@gmail.com)                *
 *                        Mario Kodba (Mario's email (here)                      *
 *               Institution: @Faculdade de Engenharia Universidade do Porto     *
 *                                                                               *
 ********************************************************************************/

/*
*
* Asynchronous communication between master (computer) and slaves (controller-ID)
* First the function for communication and then the functions for information transfer
*
* The message needs the content to send and the address (ID) of the slave (receiver)
* Also important to record the time stamp of the message
*
* 
*/

#include "../includes/config.h"
#include <util/setbaud.h>

void uart_init(void) {
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
    
#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~(_BV(U2X0));
#endif

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */ 
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */    
}

//////////////////////////////////////////////////////////////////////////////////

void uart_putchar(char c, FILE *stream) {
    if (c == '\n') {
        uart_putchar('\r', stream);
    }
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
}

//////////////////////////////////////////////////////////////////////////////////

char uart_getchar(void) {
    loop_until_bit_is_set(UCSR0A, RXC0);
    return UDR0;
}

//////////////////////////////////////////////////////////////////////////////////

void uart_getstring(char * name) {
 char c;
 do {
  c = uart_getchar();
  if (c != '\n') {
    *name++ = c;
  }
 } while (c != '\n');
 *name = 0;
}

//////////////////////////////////////////////////////////////////////////////////
