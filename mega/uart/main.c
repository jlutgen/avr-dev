/*
 *  UART
 */

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>

static char msg[80];

void uart_read(char* s, int len) {
    for (int i = 0; i < len; i++) {
        // Wait for data to be received
        while (!(UCSR0A & (1 << RXC0)))
            ;
        // Get received data from buffer
        *s = UDR0;
        if (*s == '\r' || *s == '\n') {
            *(s + 1) = '\0';
            return;
        }
        ++s;
    }
}

void uart_write(char* s) {
    while (*s) {
        // Wait for empty transmit buffer
        while (!(UCSR0A & (1 << UDRE0)))
            ;
        // Put data into buffer, sends the data
        UDR0 = *s++;
    }
}

void config_uart(void) {
    // UBRR0 = 8;                             // baud rate 115200
    UBRR0 = 16;                             // baud rate 57600
    // UBRR0 = 103;                             // baud rate 9600
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0); // tx/rx enable
}

int main(void) {
    config_uart();
    DDRB = 0b10000000;  // PB7: output pin
    uart_write("Hello from Mega2560!\n\r");
    while (1) {
        uart_read(msg, 80);
        uart_write("\n\rreceived:");
        uart_write(msg);
        uart_write("\n\r");
        // toggle LED
        PORTB ^= 0b10000000;
    }
}
