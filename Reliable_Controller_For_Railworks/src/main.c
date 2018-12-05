/*********************************************************************************
 *                                                                               *
 *               Reliable Light System Controller For Train Line                 *
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
*	Algorithm: 1 ) Computer main station sends data with slave address
*			   2 ) Micro controller receives the commands and evaluates if address is correct
*			   3 ) Sends the data to the memory I2C with time stamp
*			   4 ) Sends the data to the corresponding output LEDs
*			   5 ) If test mode enable begins testing the memory
*/

#include"config.h" /*Configuration file with the needed includes and I/O ports set*/
#include "serial_comms.h"

#define DEBUG		/* defined for debugging purposes */
//#define EXT_MEM address	---------- address of ext_memory has to be set here

int main(void) {

	uart_init();
    stdout = &uart_output;
    stdin  = &uart_input;
                
    char* input;			// added pointer?

    while(1) {
        puts("Hello world!");
        input = getchar();
        printf("You wrote %c\n", input);        

/* this part writes to eeprom memory and reads it back, checking for errors */
#ifdef DEBUG
	usart_init();
	printf_init();
	printf("\n\nHello\n");
#endif

	uint8_t ret;
	ret = i2c_start(EXT_MEM);
	/* check if failed to issue start condition, could be device not found*/
	if(ret){
		i2c_stop();
/* print on terminal return value */
#ifdef DEBUG
		printf("%d", ret);
#endif
	}

	else {
		i2c_write(0x05);	/* write address = 0x05 */
		i2c_write(0x75);	/* write data to address 0x05 */
		ret = i2c_read_nack(); /* read one byte from address */
		i2c_stop();
#ifdef DEBUG
		printf("%d", ret);
#endif
	}

    }
	return 0; 
}
