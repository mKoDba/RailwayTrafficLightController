#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#define DEBUG		/* defined for debugging purposes */
#define EXT_MEM 0xA0	/* address of external memory has to be set here */
#define WRITE 0
#define READ 1

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#ifndef BAUD
#define BAUD 9600
#endif