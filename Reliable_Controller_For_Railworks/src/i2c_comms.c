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
#include "config.h"
#include "i2c_comms.h"
#include <util/twi.h>

#define F_SCL 100000UL  /* define SCL freq - 100kHz or 400kHz according to datasheet */
#define PRESCALER 1
#define TWBR_VAL ((((F_CPU / F_SCL) / PRESCALER) - 16 )/2)
#define MEM_CAP 0xFF
#define START_ADDRESS 0x00
#define WRITE 0
#define READ 1

void i2c_init(void){
  TWSR = 0;
  TWBR = (uint8_t)TWBR_VAL; /* set SCL to 100kHz - value is 0x48*/
}

uint8_t i2c_start(uint8_t slave_address){
	/* reset TWI control register */
	TWCR = 0;
	/* send START signal */
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	/* wait for end of transmission */
	while(!(TWCR & (1<<TWINT)));

	/* check if the start condition was successfully transmitted,
	 * if it's not, return 1*/
	if(((TWSR & 0xF8) != TW_START) && ((TWSR & 0xF8) != TW_REP_START)){
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

//////////////////////////////////////////////////////////////////////////////////

void i2c_stop(void){
	/* send STOP condition */
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);

	/* wait until stop condition is executed and bus released */
	while(TWCR & (1<<TWSTO));
}

//////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////////

uint8_t i2c_rep_start(uint8_t slave_address){
    return i2c_start(slave_address);
}

void i2c_start_wait(uint8_t slave_address){

    while (1){
    	TWCR = 0;
	    /* send START condition */
	    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

    	/* wait until transmission completed */
    	while(!(TWCR & (1<<TWINT)));

    	/* check value of TWI Status Register, but mask prescaler bits */
    	if ( ((TWSR & 0xF8) != TW_START) && ((TWSR & 0xF8) != TW_REP_START)) continue;

    	/* send device address */
    	TWDR = slave_address;
    	TWCR = (1<<TWINT) | (1<<TWEN);

    	/* wail until transmission completed */
    	while(!(TWCR & (1<<TWINT)));

    	/* check value of TWI Status Register, but mask prescaler bits */
    	if (((TWSR & 0xF8) == TW_MT_SLA_NACK )||((TWSR & 0xF8) == TW_MR_DATA_NACK)){
    	    /* device busy, send stop condition to terminate write operation */
	        TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);

	        /* wait until stop condition is executed and bus released */
	        while(TWCR & (1<<TWSTO));

    	    continue;
    	}
    	break;
     }
}

//////////////////////////////////////////////////////////////////////////////////

/* Read one byte from the I2C device, request more data from device */
uint8_t i2c_read_ack(void){
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	while(!(TWCR & (1<<TWINT)));

    return TWDR;
}

//////////////////////////////////////////////////////////////////////////////////

/* Read one byte from the I2C device, read is followed by a stop condition */
uint8_t i2c_read_nack(void){
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	
    return TWDR;
}

//////////////////////////////////////////////////////////////////////////////////


//function that writes single byte value (val) to an eeprom address passed to function (eeprom_address)
void byte_write_eeprom(uint8_t slave_address, uint8_t eeprom_address, uint8_t val){
	i2c_start(slave_address+WRITE);
	i2c_write(eeprom_address);
	i2c_write(val);
	i2c_stop();
}
// function that reads whole EEPROM byte at the time
// and saves read values into array passed in function by pointer
void read_eeprom(uint8_t slave_address, uint8_t *val){
	uint8_t i;
	i2c_start_wait(slave_address+WRITE);
	i2c_write(START_ADDRESS);

	i2c_rep_start(slave_address+READ);
	for(i=0;i<MEM_CAP;i++){
		val[i] = i2c_read_ack();
	}
	i2c_stop();
}
uint8_t read_last_address(uint8_t slave_address){
	uint8_t a;
	i2c_start_wait(slave_address+WRITE);
	i2c_write(START_ADDRESS);
	i2c_rep_start(slave_address+READ);
	a = i2c_read_nack();
	i2c_stop();
	return a;
}
