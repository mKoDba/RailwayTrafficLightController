/*********************************************************************************
 *                                                                               *
 *               Reliable Light System Controller For Train Line                 *
 *                                                                               *
 *                                                                               *
 *               Code guide: NASA C style guide 1994                             *
 *                                                                               *
 *               Authors: Joel Matos Vaz (joelmatosvaz@gmail.com)                *
 *                        Mario Kodba (up201802785@fe.up.pt)                      *
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
#include "config.h"
#include "serial_comms.h"
#include "i2c_comms.h"
#include "controller.h"

int main(void) {

	uart_init();
	set_output_signals();
	controller_init();
	i2c_init();
    stdout = &uart_output;
    stdin  = &uart_input;
                
    char cmdServer[6]={};			// Serial communication array 2 bits ID and 4 bits message
    uint8_t ret;
	uint8_t write_i2c_enable = 0;
	uint8_t controller_enable = 0;

    while(1) {

		//SERIAL COMMUNICATION CODE STARTS HERE
        puts("Receive Mode enabled! \n");
        uart_getstring(cmdServer);
		printf("ID = %c %c Message = ", cmdServer[0], cmdServer[1]);
		for(int i = 0; i < 6; i++){
        	printf("%c ", cmdServer[i]);    
		}
		printf("\n");
		if(cmdServer[0] == '0' && cmdServer[1] == '1'){
			printf("\nI'm micro controller with id 01 and got Message: \n");
			printf("Slave id %c %c Message: ",cmdServer[0], cmdServer[1]);
			//write_i2c_enable = 1;
			controller_enable = 1; //enables controller logic
			for(int i = 0; i < 4; i++){
				printf("%c ",cmdServer[i+2]);
			}
		printf("\n \n");
		}
		else{
			printf("Incorrect slave id: ");
			//write_i2c_enable = 1;
			controller_enable = 0; //Safety redundancy
			for(int i = 0; i < 2; i++){
				printf("%c", cmdServer[i]);
			}
			printf("\n \n");
		}
		//SERIAL COMMUNICATION CODE ENDS HERE
		/////////////////////////////////////////////////////////////////////////////////////////////////

		//I2C COMMUNICATION CODE STARTS HERE
		if(write_i2c_enable == 1){
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
				i2c_stop();     
				write_i2c_enable = 0;                         							 /* release bus */
			}
		}
		//I2C COMMUNICATION CODE ENDS HERE
		/////////////////////////////////////////////////////////////////////////////////////////////////

		//CONTROLLER CODE STARTS HERE
		if(controller_enable == 1){
			if(cmdServer[2] == 0 && cmdServer[3] == 0 && cmdServer[4] == 0 && cmdServer[5] == 0){
				light_red_on();
				light_green_off();
				controller_enable = 0;
			}
			else if (cmdServer[2] == 1 && cmdServer[3] == 1 && cmdServer[4] == 1 && cmdServer[5] == 1){
				light_red_off();
				light_green_on();
				controller_enable = 0;
			}
			else
				light_red_on();
				light_green_off();
				controller_enable = 0;
		}
		//CONTROLLER CODE ENDS HERE
		/////////////////////////////////////////////////////////////////////////////////////////////////
    } //End While(1)
	return 0; 
}
