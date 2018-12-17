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
#define MEM_CAP 0xFF
#define EXT_MEM 0xA0
#define WRITE 0
#define READ 1

int main(void) {

	uart_init();
	set_output_signals();
	controller_init();
	i2c_init();
    stdout = &uart_output;
    stdin  = &uart_input;

	int Debug1 = 1;
                
    char cmdServer[6]={};			// Serial communication array 2 bits ID and 4 bits message
    uint8_t i,val;
    uint8_t addr[256];
	uint8_t write_i2c_enable = 1;
	uint8_t controller_enable = 0;

    while(1) {

		if(Debug1 == 1){
			//Debug Code for Java serial reader
			printf("Hello, ");
			printf("World, ");
			printf("ATMega328p \n");
			_delay_ms(10000);
		}
		
		if(Debug1 == 0){
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

			//byte_write_eeprom(EXT_MEM, 0x00, cmdServer[0]);
			//byte_write_eeprom(EXT_MEM, 0x01, cmdServer[1]);
			//byte_write_eeprom(EXT_MEM, 0x50, 11);
			//byte_write_eeprom(EXT_MEM, 0xF8, 11);

			//prints whole eeprom memory with corresponding values written to addresses
			read_eeprom(EXT_MEM, &addr[0]);
			printf("ADDRESS --- VALUE STORED\n\n");
			for(i=0;i<MEM_CAP+1;i++){
				printf("%d --- %d\n", i, addr[i]);
				write_i2c_enable = 0;
			}

		}
		//I2C COMMUNICATION CODE ENDS HERE
		/////////////////////////////////////////////////////////////////////////////////////////////////

		//CONTROLLER CODE STARTS HERE
		val = 0x00;
		if(controller_enable == 1){
			if(cmdServer[2] == 0 && cmdServer[3] == 0 && cmdServer[4] == 0 && cmdServer[5] == 0){
				light_red_on();
				byte_write_eeprom(EXT_MEM,val,'R');
				light_green_off();
				controller_enable = 0;
				if(val<MEM_CAP){
					val++;
				}
				else val=0x00;
			}
			else if (cmdServer[2] == 1 && cmdServer[3] == 1 && cmdServer[4] == 1 && cmdServer[5] == 1){
				light_red_off();
				light_green_on();
				byte_write_eeprom(EXT_MEM,val,'G');
				controller_enable = 0;
				if(val<MEM_CAP){
					val++;
				}
				else val=0x00;
			}
			else
				light_red_on();
				byte_write_eeprom(EXT_MEM,val,'R');
				light_green_off();
				controller_enable = 0;
				if(val<MEM_CAP){
					val++;
				}
				else val=0x00;
		}
		//CONTROLLER CODE ENDS HERE
		/////////////////////////////////////////////////////////////////////////////////////////////////
		}//End if Debug == 0
    } //End While(1)
	return 0; 
}