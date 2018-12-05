/*********************************************************************************
 *                                                                               *
 *               Configuration File for Main Function                            *
 *                                                                               *
 *                                                                               *
 *               Code guide: NASA C style guide 1994                             *
 *                                                                               *
 *               Authors: Joel Matos Vaz (joelmatosvaz@gmail.com)                *
 *                        Mario Kodba (up201802785@fe.up.pt)                     *
 *               Institution: @Faculdade de Engenharia Universidade do Porto     *
 *                                                                               *
 ********************************************************************************/

/*
*
* Synchronous communication between master (Micro Controller) and slave (EEPROM Memory - xxxxxx)
* Communication type is I2C
*
* The communication will have all the commands sent from the computer (main station)
* the associated times stamps and malfunctions found.
*
* 		  TODO
* 		  	  	Test the communication
* 		  	   	Read the memory
*/

#include <util/twi.h>

#include"/home/sobaca/CommsForTrainLight/Reliable_Controller_For_Railworks/inc/config.h"
#include"/home/sobaca/CommsForTrainLight/Reliable_Controller_For_Railworks/inc/i2c_comms.h"


uint8_t i2c_start(uint8_t slave_address){
	/* reset TWI control register */
	TWCR0 = 0;
	/* set SCL to 100kHz - value is 0x48*/
	TWBR0 = (uint8_t)TWBR_VAL;
	/* send START signal */
	TWCR0 = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	/* wait for end of transmission */
	while(!(TWCR0 & (1<<TWINT)));

	/* check if the start condition was successfully transmitted,
	 * if it's not, return 1*/
	if((TWSR0 & 0xF8) != TW_START){
		return 1;
	}

	/* load slave address into data register */
	TWDR0 = slave_address;
	/* start transmission of address */
	TWCR0 = (1<<TWINT) | (1<<TWEN);
	/* wait for end of transmission */
	while(!(TWCR0 & (1<<TWINT)));
	/* check if the device has acknowledged the READ / WRITE mode */
	if (((TWSR0 & 0xF8) != TW_MT_SLA_ACK) && ((TWSR0 & 0xF8) != TW_MR_SLA_ACK)){
		return 1;
	}
	else 
		return 0;
}

void i2c_stop(void){
	/* send STOP condition */
	TWCR0 = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}

/* Send one byte to I2C device */
uint8_t i2c_write(uint8_t data){
	/* load data into data register */
	TWDR0 = data;
	/* start transmission of data to previously addressed device*/
	TWCR0 = (1<<TWINT) | (1<<TWEN);
	/* wait for end of transmission */
	while(!(TWCR0 & (1<<TWINT)));
	/* check if the device has acknowledged */
	if((TWSR0 & 0xF8) != TW_MT_DATA_ACK){
		return 1;
	}
	else
		return 0;
}

/* Read one byte from the I2C device, request more data from device */
uint8_t i2c_read_ack(void){
	TWCR0 = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	while(!(TWCR0 & (1<<TWINT)));

    return TWDR0;
}

/* Read one byte from the I2C device, read is followed by a stop condition */
uint8_t i2c_read_nack(void){
	TWCR0 = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR0 & (1<<TWINT)));
	
    return TWDR0;
}