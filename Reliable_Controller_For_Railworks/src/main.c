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

#include"/home/sobaca/CommsForTrainLight/Reliable_Controller_For_Railworks/inc/config.h" /*Configuration file with the needed includes and I/O ports set*/
#include"/home/sobaca/CommsForTrainLight/Reliable_Controller_For_Railworks/inc/serial_comms.h"
#include"/home/sobaca/CommsForTrainLight/Reliable_Controller_For_Railworks/inc/i2c_comms.h"
#include"/home/sobaca/CommsForTrainLight/Reliable_Controller_For_Railworks/inc/controller.h"


#define DEBUG		/* defined for debugging purposes */
#define EXT_MEM 100	/* address of external memory has to be set here */

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

		//Controller Code

		//I2C COMMUNICATION CODE STARTS HERE
		//uint8_t ret;
		//ret = i2c_start(EXT_MEM);
		/* check if failed to issue start condition, could be device not found*/
		//if(ret){
		//	i2c_stop();
			/* print on terminal return value */
		//	#ifdef DEBUG
		//		printf("%d", ret);
		//	#endif
		//}

		//else {
		//	i2c_write(0x05);	/* write address = 0x05 */
		//	i2c_write(0x75);	/* write data to address 0x05 */
		//	ret = i2c_read_nack(); /* read one byte from address */
		//	i2c_stop();
		//	#ifdef DEBUG
		//		printf("%d", ret);
		//	#endif
		//}
		//I2C COMMUNICATION CODE STARTS HERE
		/////////////////////////////////////////////////////////////////////////////////////////////////
    } //End While(1)
	return 0; 
}