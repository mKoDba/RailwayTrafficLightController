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
/*
*
*Code for controlling the lights
*Emergence mode red and green both on and blinking
*
*/
#include "config.h"
#include "controller.h"

void set_output_signals(void){
    DDRB = DDRB | (1<<Red_light) | (1<<Green_light);
}

//////////////////////////////////////////////////////////////////////////////////

void controller_init(void){
        PORTB = (1<<Red_light);
        PORTB = (0<<Green_light);
}

//////////////////////////////////////////////////////////////////////////////////

void light_red_on(void){
        PORTB = (0<<Red_light);
}

//////////////////////////////////////////////////////////////////////////////////

void light_red_off(void){
    PORTB = (1<<Red_light);
}

/////////////////////////////////////////////////////////////////////////////////

void light_green_on(void){
    PORTB = (1<<Green_light);
}

//////////////////////////////////////////////////////////////////////////////////

void light_green_off(void){
    PORTB = (0<<Green_light);
}

//////////////////////////////////////////////////////////////////////////////////

void emergence_mode(void){
    PORTB = (1<<Red_light) | (0<<Green_light); 

}

//////////////////////////////////////////////////////////////////////////////////
