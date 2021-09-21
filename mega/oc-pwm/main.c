/*
 *  Output compare / PWM
 *
 *  Pulse the onboard LED.
 */

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

void config_timer0(void) {
    // With timer clock source freq = 16 MHz / 8,
    // our PWM frequency (which is the timer rollover
    // frequency) is 2 MHz / 256 = 19.5 KHz
    
    // Clock source: clk_i/o / 8
    TCCR0B |= _BV(CS01);
    // Mode 3: Fast PWM, TOP=0xff
    TCCR0A |= _BV(WGM00) | _BV(WGM01);
    // Non-inverting mode: clear OC0A on compare match,
    // set OC0A at BOTTOM.
    TCCR0A |= _BV(COM0A1);
    OCR0A = 0;           // Initial value
}

void config_gpio(void) {
    DDRB |= _BV(PB7);  // LED/OC0A: output pin
}

int main(void) {
    uint8_t up = 1;

    config_timer0();
    config_gpio();
    while (1) {
        if (OCR0A == 0xff)
            up = 0;
        else if (OCR0A == 0) 
            up = 1;

        if (up)
            OCR0A++;
        else
            OCR0A--;
        _delay_ms(3); 
    }
    return 0;
}
