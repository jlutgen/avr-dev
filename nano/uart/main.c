/*
 * UART
 */

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
        // Put data into buffer (sends the data)
        UDR0 = *s++;
    }
}

void config_uart(void) {
    UBRR0 = 8;                              // Baud rate 115200
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0);  // TX/RX enable
}

void config_gpio(void) {
    DDRB = 0b00100000;  // PB5: output pin
}

int main(void) {
    config_uart();
    config_gpio();
    uart_write("Hello from Nano!\r\n");
    while (1) {
        uart_read(msg, 80);
        uart_write("received:");
        uart_write(msg);
        uart_write("\r\n");
        PORTB ^= 0b00100000;  // Toggle LED
    }
}
