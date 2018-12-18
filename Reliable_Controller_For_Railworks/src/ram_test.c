/*********************************************************************************
 *                                                                               *
 *               EEPROM Memory Testing for ATMEGA328p                            *
 *                                                                               *
 *                                                                               *
 *               Code guide: NASA C style guide 1994                             *
 *                                                                               *
 *               Authors: Joel Matos Vaz (joelmatosvaz@gmail.com)                *
 *                        Mario Kodba (Mario's email (here)                      *
 *               Institution: @Faculdade de Engenharia Universidade do Porto     *
 *                                                                               *
 ********************************************************************************/

#include"config.h"
#include <stdlib.h>

#define RAM_BYTES 1260			// depending on mcu used
								// more than this means collision with stack memory

//////////////////////////////////////////////////////////////////////////////////

uint16_t ram_test(void) {

	  // allocate heap
	  volatile char* test = (char*)malloc(RAM_BYTES);
	  char currentByte;
	  uint16_t i,errors=0;

	  // test heap
	  for(i=0; i<RAM_BYTES; i++) {
	    currentByte = test[i];
	    test[i] = 0x55;
	    if(test[i] != 0x55) errors++;
	    test[i] = 0xAA;
	    if(test[i] != 0xAA) errors++;
	    test[i] = currentByte;
	  }
	  free(test);
	  return errors;
}
/*
	uint16_t* ptr = 0;
	//using 16bits or 2bytes
	for(ptr = (uint16_t*)0x0000; ptr <= (uint16_t*)(EEPROM_KB*512); ptr += 2){
		eeprom_busy_wait();
		//wait for eeprom to be ready
		eeprom_write_word(ptr, pattern);
		//write a given pattern
	}
	_delay_ms(WAIT);
	for(ptr = (uint16_t*)0x0000; ptr <= (uint16_t*)(EEPROM_KB*512); ptr += 2){
		eeprom_busy_wait();
		if(eeprom_read_word(ptr) != pattern) {
			return 1;
		}
		//check if a given pattern is at correct memory location
	}
	return 0;
}
*/
//////////////////////////////////////////////////////////////////////////////////
/*
uint16_t sram_test(void) {
	uint16_t errors=0;
	if(!pattern_test(0xFF)) errors++;
	if(!pattern_test(0x00)) errors++;
	if(!pattern_test(0xAA)) errors++;
	if(!pattern_test(0x55)) errors++;

	//check memory using typical patterns,
	//using all the patterns possible would take too much time
	return errors;
}

//////////////////////////////////////////////////////////////////////////////////
*/
