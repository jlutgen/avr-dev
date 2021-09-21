/*
 *  Blinky
 */

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    DDRB = 0b10000000;  // PB7: output pin
    while (1) {
        // LED on
        PORTB = 0b10000000;
        _delay_ms(500);

        // LED off
        PORTB = 0b00000000;
        _delay_ms(500);
    }
}
