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
* Author: Joel Vaz @FEUP joelmatosvaz@gmail.com
* 		  Mario Kodba @FEUP (add Mario's email)
*
* 		  TODO
* 		  	  	Test the communication
* 		  	   	Read the memory
*/

#include "config.h"
#include <avr/io.h>
#include <util/twi.h>

#ifndef  F_CPU
#define F_CPU 16000000UL
#endif

#define F_SCL 100000UL  /* define SCL freq - 100kHz or 400kHz according to datasheet */
#define PRESCALER 1
#define TWBR_VAL ((((F_CPU / F_SCL) / PRESCALER) - 16 )/2)

uint8_t i2c_start(uint8_t slave_address){
	/* reset TWI control register */
	TWCR = 0;
	/* set SCL to 100kHz - value is 0x48*/
	TWBR = (uint8_t)TWBR_VAL;
	/* send START signal */
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	/* wait for end of transmission */
	while(!(TWCR & (1<<TWINT)));

	/* check if the start condition was successfully transmitted,
	 * if it's not, return 1*/
	if((TWSR & 0xF8) != TW_START){
		return 1;
	}

	/* load slave address into data register */
	TWDR = slave_address;
	/* start transmission of address */
	TWCR = (1<<TWINT) | (1<<TWEN);
	/* wait for end of transmission */
	while(!(TWCR & (1<<TWINT)));
	/* check if the device has acknowledged the READ / WRITE mode */
	if (((TWSR & 0xF8) != TW_MT_SLA_ACK) && ((TWSR & 0xF8) != TW_MR_SLA_ACK)){
		return 1;
	}

	return 0;
}

void i2c_stop(void){
	/* send STOP condition */
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}

/* Send one byte to I2C device */
uint8_t i2c_write(uint8_t data){
	/* load data into data register */
	TWDR = data;
	/* start transmission of data to previously addressed device*/
	TWCR = (1<<TWINT) | (1<<TWEN);
	/* wait for end of transmission */
	while(!(TWCR & (1<<TWINT)));
	/* check if the device has acknowledged */
	if((TWSR & 0xF8) != TW_MT_DATA_ACK){
		return 1;
	}

	return 0;
}

/* Read one byte from the I2C device, request more data from device */
uint8_t i2c_read_ack(void){
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	while(!(TWCR & (1<<TWINT)));

    return TWDR;
}

/* Read one byte from the I2C device, read is followed by a stop condition */
uint8_t i2c_read_nack(void){
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR & (1<<TWINT)));

    return TWDR;
}
