/*
 *  Comparator
 */

// On the Arduino Mega2560 board, the PE2/AIN0 pin of the atmega2560
// is not connected to anything. This is the positive input to the
// analog comparator. The only alternative is to set ACSR.ACBG, which
// routes the internal bandgap reference (nominally 1.1 V) to the
// positive input of the comparator.

#define F_CPU 16000000UL

#include <avr/io.h>

void config_comparator(void) {
    // Default negative input is PE3/AIN1 (Arduino pin 5)

    ACSR |= _BV(ACBG);  // Internal 1.1V ref is positive input
}

void config_gpio(void) {
    // DDRB |= _BV(PB7);  // LED: output pin
    DDRB |= _BV(PB0);  // PB0: output pin (board pin 53)
}

int main(void) {
    config_comparator();
    config_gpio();
    while (1) {
        // Poll the analog comparator output bit (ACSR.ACO)
        // and set/clear PB0 accordingly
        if (ACSR & _BV(ACO))
            PORTB |= _BV(PB0);
        else
            PORTB &= ~_BV(PB0);
    }
}
