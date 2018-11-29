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
*	Algorithm: 1 ) Computer main station sends data with slave id
*			   2 ) Micro controller receives the commands and evaluates if id is correct
*			   3 ) Sends the data to the memory I2C with time stamp
*			   4 ) Sends the data to the corresponding output LEDs
*			   5 ) If test mode enable begins testing the memory
*
*/

#include"Config.h" /*Configuration file with the needed includes and I/O ports set*/

int main(void) {

	uint8_t led_state = 1;
	DDRB = DDRB | (1<<LED_BIT);

	while (1) {
		if (led_state)
			LED_HIGH;
		else
			LED_LOW;

		led_state = !led_state;
		_delay_ms(50000);
	}
	return 0;
}
