#include <msp430.h>

void main() {
    WDTCTL = WDTPW | WDTHOLD; // Disable the watchdog timer
    P1DIR = 0x01; // Set the pin to be an output

    while(1) {
        volatile unsigned long i = 50000;
        P1OUT ^= 0x01; // Switch pin's level
        while(i--);
    }
}
