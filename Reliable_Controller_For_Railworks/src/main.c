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

//#include"/home/sobaca/CommsForTrainLight/Reliable_Controller_For_Railworks/inc/config.h" /*Configuration file with the needed includes and I/O ports set*/
//#include"/home/sobaca/CommsForTrainLight/Reliable_Controller_For_Railworks/inc/serial_comms.h"
//#include"/home/sobaca/CommsForTrainLight/Reliable_Controller_For_Railworks/inc/i2c_comms.h"
//#include"/home/sobaca/CommsForTrainLight/Reliable_Controller_For_Railworks/inc/controller.h"
#include "config.h"
#include "serial_comms.h"
#include "i2c_comms.h"
//#include "controller.h"


#define DEBUG		/* defined for debugging purposes */
#define EXT_MEM 0xA0	/* address of external memory has to be set here */
#define WRITE 0
#define READ 1

int main(void) {

	uart_init();
    stdout = &uart_output;
    stdin  = &uart_input;
                
    char input[6];			// Serial communication array 2 bits ID and 4 bits message
	
    while(1) {

		//SERIAL COMMUNICATION CODE HERE
		//Tests the serial communication
		//if serial receive mode enabled do this code
        puts("Receive Mode enabled! \n");
        uart_getstring(input);
		printf("ID = %c %c Message = ", input[0], input[1]);
		for(int i = 0; i < 7; i++){
        	printf("%c ", input[i]);    
		}
		printf("\n");
		if(input[0] == '0' && input[1] == '1'){
			printf("\nSou micro com id 01 e recebi a mensagem: ");
			printf("Slave id %c %c Message: ",input[0], input[1]);
			for(int i = 0; i < 5; i++){
				printf("%c ",input[i+2]);
			}
		printf("\n \n");
		}
		else{
			printf("Incorrect slave id: ");
			for(int i = 0; i < 2; i++){
				printf("%c", input[i]);
			}
			printf("\n \n");
		}
		//SERIAL COMMUNICATION CODE ENDS HERE
		/////////////////////////////////////////////////////////////////////////////////////////////////

		ret = i2c_start(EXT_MEM+WRITE);
		/* check if failed to issue start condition, could be device not found*/
		if(ret){
			/* print on terminal return value */
			i2c_stop();
			printf("There was an error with issuing start condition!!!");
		}

		else {
			uint8_t start_add = 0x05;
			i2c_write(start_add);	/* write address = 0x05 */
			i2c_write(0x75);		/* write data to address 0x05 */
			i2c_stop();				/* release bus */

			i2c_start_wait(EXT_MEM+WRITE);     /* set device address and write mode */
			i2c_start(EXT_MEM+WRITE);
			i2c_write(0x05);                   /* write address = 0x05 */
			i2c_rep_start(EXT_MEM+READ);       /* set device address and read mode */
			ret = i2c_read_nack();             /* read one byte followed by stop condition */
			i2c_stop();

			printf("The value read from EEPROM address (%d): %d\n\n", start_add, ret);

			/* write 0x70,0x71,072,073 to eeprom address 0x00..0x03 (page write),
			 wait until the device is no longer busy from the previous write operation */
			i2c_start_wait(EXT_MEM+WRITE);     // set device address and write mode
			i2c_write(0x00);                        // write start address = 0
			i2c_write(0x70);                        // write data to address 0
			i2c_write(0x71);                        //    "    "   "    "    1
			i2c_write(0x72);                        //    "    "   "    "    2
			i2c_write(0x74);                        //    "    "   "    "    3
			i2c_stop();                             // set stop conditon = release bus

			 /* write ok, read value back from eeprom address 0..3 (sequential read),
			    wait until the device is no longer busy from the previous write operation */
			i2c_start_wait(EXT_MEM+WRITE);      /* set device address and write mode */
			start_add = 0x00;
			i2c_write(start_add);                   /* write address = 0 */
			i2c_rep_start(EXT_MEM+READ);        	/* set device address and read mode */
			ret = i2c_read_ack();                       					/* read one byte form address 0 */
			printf("Value written to address (%d): %d\n", start_add, ret);
			ret = i2c_read_ack();                       						 /*  "    "    "    "     "    1 */
			printf("Value written to address (%d): %d\n", start_add+1, ret);
			ret = i2c_read_ack();                      							 /* "    "    "    "     "    2 */
			printf("Value written to address (%d): %d\n", start_add+2, ret);
			ret = i2c_read_nack();                      						 /*  "    "    "    "     "    3 */
			printf("Value written to address (%d): %d\n", start_add+3, ret);
			i2c_stop();                              							 /* release bus */
		}
		//I2C COMMUNICATION CODE ENDS HERE
		/////////////////////////////////////////////////////////////////////////////////////////////////
    } //End While(1)
	return 0; 
}
