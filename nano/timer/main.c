/*
 *  Use a timer interrupt to toggle LED
 */

#define F_CPU 16000000UL

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void config_timer1(void) {
    // Clock source: clk_i/o / 1024
    TCCR1B |= _BV(CS10) | _BV(CS12);
    // Mode 4: clear timer on compare match to OCR1A
    TCCR1B |= _BV(WGM12);
    OCR1A = 7813;           // TOP value for 2 Hz interrupt freq
    TIMSK1 |= _BV(OCIE1A);  // Enable interrupt on match
}

void config_gpio(void) {
    DDRB = 0b00100000;  // PB5: output pin
}

ISR(TIMER1_COMPA_vect) {
    PORTB ^= _BV(PB5);  // Toggle LED
}

int main(void) {
    config_timer1();
    config_gpio();
    sei();  // Enable interrupts
    while (1) {
    }
    return 0;
}
