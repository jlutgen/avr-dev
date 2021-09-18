/*
 *  Blinky
 */

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    DDRB = 0b00100000;  // PB5: output pin
    while (1) {
        PORTB ^= 0b00100000;  // Toggle LED on PB5
        _delay_ms(500);
    }
}
