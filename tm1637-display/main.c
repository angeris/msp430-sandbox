#include <msp430.h>

#define CLK 3
#define DAT 2
#define CLK_BIT (1<<CLK)
#define DAT_BIT (1<<DAT)

// Note that the given protocol is NOT I2C (I made this mistake and spent hours trying to figure out the address to no avail)
void init_t() {
    P1DIR |= DAT_BIT | CLK_BIT; // Set pins 2 and 3 to be outputs
    P1OUT &= ~(DAT_BIT | CLK_BIT); // Set both pins to be high
}
void delay() {
    volatile unsigned int a = 100;
    while(a--);
}
void start_t() {
    // Start of IO sequence, see datasheet page 3
    P1OUT &= ~DAT_BIT;
    delay();
    P1OUT &= ~CLK_BIT;
    delay();
    P1OUT |= DAT_BIT;
    delay();
    P1OUT |= CLK_BIT;
    delay();
}
void end_t() {
    // End of IO sequence, again see pg. 3
    P1OUT &= ~CLK_BIT;
    delay();
    P1OUT &= ~DAT_BIT;
    delay();
    P1OUT |= CLK_BIT;
    delay();
    P1OUT |= DAT_BIT;
    delay();
}
void write_byte_t(unsigned char data) {
    int i=0;
    for(i=0; i<8; i++) {
        P1OUT &= ~CLK_BIT;
        delay();
        if(data & 1) {
            P1OUT |= DAT_BIT;
        }
        else {
            P1OUT &= ~DAT_BIT;
        }
        delay();
        P1OUT |= CLK_BIT;
        delay();
        data >>= 1;
    }
    P1OUT &= ~CLK_BIT;
    delay();
    P1OUT |= DAT_BIT;
    delay();
    P1OUT |= CLK_BIT;
    delay();
    P1DIR &= ~DAT_BIT; // Set data as input (wait for ACK)
    delay();
    while(P1IN & DAT_BIT) { if(i++ > 300) break; }
    P1DIR |= DAT_BIT;
    delay();
    P1OUT &= ~DAT_BIT;
    delay();
}

void clear_display() {
    for(int i=0; i<4; i++) {
        // Begin with data command, first
        start_t();
        write_byte_t(0x40);
        end_t();

        // Write to display registers
        start_t();
        // Set first display to be written
        write_byte_t(0xc0 | i);
        write_byte_t(0x00);
        end_t();

        start_t();
        write_byte_t(0x8f);
        end_t();
    }
}

void main() {
    WDTCTL = WDTPW | WDTHOLD;
    init_t();

    clear_display();

    while(1) {
        // Begin with data command, first
        start_t();
        write_byte_t(0x40);
        end_t();

        // Write to display registers
        start_t();
        // Set first display to be written
        write_byte_t(0xc0);
        write_byte_t(0x3f);
        end_t();

        start_t();
        write_byte_t(0x8f);
        end_t();
    }
}