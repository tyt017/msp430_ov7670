#include <msp430.h>

unsigned int i;

void uart_init(void) {
    //¡]P2.0 -> TX, P2.1 -> RX¡^
    P2SEL1 |= BIT0 | BIT1;  // set P2.0 and P2.1 as UART
    P2SEL0 &= ~(BIT0 | BIT1);

    // initialize eUSCI_A0 as UART mode
    UCA0CTLW0 = UCSWRST;          // reset mode
    UCA0CTLW0 |= UCSSEL_2;        // use SMCLK as clock
    UCA0BRW = 8;                  // set 115200 (assume SMCLK = 1 MHz)
    UCA0MCTLW = 0xD600;
    UCA0CTLW0 &= ~UCSWRST;        // release eUSCI_A0
}

void uart_send_byte(unsigned char byte) {
    while (!(UCA0IFG & UCTXIFG)); // waiting for transfer buffer
    UCA0TXBUF = byte;             // sending a byte
}

void uart_send_data(unsigned char *data, unsigned int size) {
    for (i = 0; i < size; i++) {
        uart_send_byte(data[i]); // sending a byte for each time
    }
}

void uart_send_string(const char *str) {
    while (*str) {
        while (!(UCA0IFG & UCTXIFG));
        UCA0TXBUF = *str++;
    }
}
