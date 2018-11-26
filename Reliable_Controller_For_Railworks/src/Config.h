/*********************************************************************************
 *                                                                               *
 *               Configuration File for Main Function                            *
 *                                                                               *
 *                                                                               *
 *               Code guide: NASA C style guide 1994                             *
 *                                                                               *
 *               Authors: Joel Matos Vaz (joelmatosvaz@gmail.com)                *
 *                        Mario Kodba (Mario's email (here)                      *
 *               Institution: @Faculdade de Engenharia Universidade do Porto     *
 *                                                                               *
 ********************************************************************************/

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define F_CPU 16000000UL /* CPU frequency at 5Volts */

#define LED_BIT PB4
#define LED_HIGH PORTB |= _BV(LED_BIT)
#define LED_LOW PORTB &= ~(_BV(LED_BIT))
