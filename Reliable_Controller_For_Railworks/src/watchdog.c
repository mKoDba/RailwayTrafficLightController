#include"config.h"
#include"watchdog.h"

void wdt_initiate(void){
    //disable interrupts
    cli();
    //reset watch dog
    wdt_reset();
    //set up watch dog timer interrupt
    WDTCSR = (1<<WDCE) | (1<<WDE);
    //start watchdog timer with 4second prescaler
    WDTCSR = (1<<WDIE) | (1<<WDE) | (1<<WDP3);
    //enable global interrupts
    sei();
}