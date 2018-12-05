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

int main(void) {

	uart_init();
    stdout = &uart_output;
    stdin  = &uart_input;
                
    char input;

    while(1) {
        puts("Hello world!");
        input = getchar();
        printf("You wrote %c\n", input);        
	}
	return 0; 
}
