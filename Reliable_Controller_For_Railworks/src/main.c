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
#include "ram_test.h"
#include "flash_test.h"
#include "controller.h"
#include <string.h>
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
                
    char cmdServer[6]={}; // Serial communication array 2 bits ID and 4 bits message

	uint16_t totalCmd = 1;			
    uint8_t i = 0;
    //byte_write_eeprom(EXT_MEM,0x00,0x01);
    uint8_t addr[256] = {};
	char write_i2c_enable[1]={};
	uint8_t controller_enable = 0;

    while(1) {
    	uint8_t lastaddr = read_last_address(EXT_MEM);
    	memset(cmdServer,0,sizeof(cmdServer));
		//SERIAL COMMUNICATION CODE STARTS HERE
		printf("mem address:%d\n",lastaddr);
        printf("Slave id = 01\nCommand Number = %d \nReceive Mode enabled!\n", totalCmd);
        uart_getstring(cmdServer);
		printf("ID = %c %c - - - - - - - - - - Full Command = ", cmdServer[0], cmdServer[1]);
		totalCmd = totalCmd + 1;
		for(i = 0; i < 6; i++){
        	printf("%c ", cmdServer[i]);    
		}
		printf("\n");
		if(cmdServer[0] == '0' && cmdServer[1] == '1'){
			printf("\nMicro controller with id 01 Received: \n");
			printf("Message: ");
			controller_enable = 1; //enables controller logic
			for(i = 0; i < 4; i++){
				printf("%c ",cmdServer[i+2]);
			}
		printf("\n \n");
		}
		else{
			printf("\n");
			printf("Incorrect slave id: ");
			controller_enable = 0; //Safety redundancy
			for(i = 0; i < 2; i++){
				printf("%c", cmdServer[i]);
			}
			printf("\n \n");
		} 
		//SERIAL COMMUNICATION CODE ENDS HERE
		/////////////////////////////////////////////////////////////////////////////////////////////////
		
		/////////////////////////////////////////////////////////////////////////////////////////////////
		//CONTROLLER CODE STARTS HERE
		if(controller_enable == 1){
			if(cmdServer[2] == 48 && cmdServer[3] == 48 && cmdServer[4] == 48 && cmdServer[5] == 48){
				light_red_on();
				light_green_off();
				printf("Red On\n");
				byte_write_eeprom(EXT_MEM,lastaddr,0);		//if RED, writes 0 to eeprom
				controller_enable = 0;
				if(lastaddr<MEM_CAP){
					lastaddr++;
				}
				else lastaddr=0x01;
			}
			else if (cmdServer[2] == 49 && cmdServer[3] == 49 && cmdServer[4] == 49 && cmdServer[5] == 49){
				light_red_off();
				light_green_on();
				printf("Green On\n");
				byte_write_eeprom(EXT_MEM,lastaddr,1);		//if GREEN, writes 1 to eeprom
				controller_enable = 0;
				if(lastaddr<MEM_CAP){
					lastaddr++;
				}
				else lastaddr=0x01;
			}
			//Memory Testing mode
			else if (cmdServer[2] == 48 && cmdServer[3] == 49 && cmdServer[4] == 48 && cmdServer[5] == 49){
				////////////////// MEMORY TESTING ///////////
				printf("Testing Mode\n");
				//wdt_initiate();
    			printf("RAM memory test showed %d errors.\n", ram_test());
    			if(flash_test()) 
					printf("FLASH memory test detected errors. \n");
    			else 
					printf("FLASH memory ok.");
    			////////////////////////////////////////////////////////
			}
			//Unkown Command
			else{
				printf("Not Recognizable Command\n");
				byte_write_eeprom(EXT_MEM,lastaddr,100);		//put value 100 when error detected, for easy detection
				controller_enable = 0;
				if(lastaddr<MEM_CAP){
					lastaddr++;
				}
				else lastaddr=0x01;
			}//end else if
		}//end control enable logic
		//CONTROLLER CODE ENDS HERE
		/////////////////////////////////////////////////////////////////////////////////////////////////

		//EEPROM CONTROL CODE STARTS HERE
		/////////////////////////////////////////////////////////////////////////////////////////////////
		printf("Print EEPROM memory values? (0-No / 1-Yes): \n");
		uart_getstring(write_i2c_enable);
		printf("%c\n", write_i2c_enable[0]);
		if(write_i2c_enable[0] == '1'){
			//prints whole eeprom memory with corresponding values written to addresses
			//wdt_initiate();
			read_eeprom(EXT_MEM, &addr[0]);
			printf("ADDRESS --- VALUE STORED\n\n");
			for(i=0;i<MEM_CAP;i++){
				printf("%d --- %d\n", i, addr[i]);
			}
			write_i2c_enable[0] = 0;
		}
		//uses address 0x00 of eeprom to save address value of last writing, in case of power off
		byte_write_eeprom(EXT_MEM,0x00,lastaddr);
		//EEPROM CONTROL CODE ENDS HERE
		/////////////////////////////////////////////////////////////////////////////////////////////////
		printf("\n-------------------------------------------------\n");
    } //End While(1)
	return 0; 
}//end Main
