//i2c implemented using i2c_ref.pdf in this repo

#include <msp430.h>

#define MODE_INPUT 0
#define MODE_OUTPUT 1

#define I2C_CLK 3
#define I2C_SDA 4

#define TM_ADDR 0x44

void clear_pins() {
    P1DIR = 0x00;
}
void init_pin(unsigned char pin, unsigned char mode) {
    P1DIR &= 0xFF ^ (!mode << pin);
    P1DIR |= 1<<pin;
}
void write_pin(unsigned char pin, unsigned char val) {
    P1DIR &= 0xFF ^ (!val << pin);
    P1DIR |= 1<<pin;
}

void init_i2c() {
    // select 3.0/3.1 for i2c
    P3SEL |= 0x03;

    // UCB0CTL1 -> Control register 1 (see 1.4.2 in doc)
    // UCSWRST -> Software reset enable (see 1.4.2, 1.3.1 in doc)
    // UCSSEL_2 -> use SMCLK (high-frequency clock, as before see 1.4.2)
    UCB0CTL1 |= UCSWRST | UCSSEL_2;

    // UCB0CTL0 -> Control register 0 (see 1.4.1 in doc)
    // UCMST -> Master mode select
    // UC_MODE_3 -> I2C mode select
    // UCSYNC -> Synchronous mode select
    UCB0CTL0 = UCMST | UCMODE_3 | UCSYNC;

    // UCB0BR0/1 -> Bit rate control low/high byte (see 1.4.3/1.4.4 in doc)
    UCB0BR0 = 12;
    UCB0BR1 = 0;

    // USB0I2CSA -> slave address (1.4.9 in doc)
    UCB0I2CSA = TM_ADDR;

    // Clear the software reset (1.3.1 in doc)
    UCB0CTL1 &= ~UCSWRST;

    // Enable TX interrupt
    UCB0IE |= UCTXIE;
}

// Nonblocking write
inline void write_i2c_n(unsigned char data) {
    // Set transmitter mode and transmit start condition (1.4.2)
    UCB0CTL1 |= UCTR | UCTXSTT;
    // Transmit buffer for data (1.4.7 in doc)
    UCB0TXBUF = data;
    
}

// Blocking write
inline void write_i2c(unsigned char data) {
    UCB0CTL1 |= UCTR | UCTXSTT;
    UCB0TXBUF = data;

    // see in430.h for more details on setting registers
    // bit set on status register (SR), set Low-Power-mode
    // and global interrupt enable
    __bis_SR_register(LPM0_bits | GIE);
    // nop for 1 cycle delay
    __nop();
}

data_len = 0;

inline void write_i2c_arr(unsigned char *data) {
    UCB0CTL1 |= UCTR | UCTXSTT;
    UCB0TXBUF = data;

    // see in430.h for more details on setting registers
    // bit set on status register (SR), set Low-Power-mode
    // and global interrupt enable
    __bis_SR_register(LPM0_bits | GIE);
    // nop for 1 cycle delay
    __nop();
}

// Compiler attribute (mark as the interrupt address)
void __attribute__ ((interrupt(USCI_B0_VECTOR))) USCI_B0_ISR() {
    // If interrupt isn't TX empty, then don't handle it (38.4.12)
    if(UCB0IV != UCTXIFG) return;
    // Stop condition (see 1.4.2)
    UCB0CTL1 |= UCTXSTT;
    // Clear interrupt flag
    UCB0IFG &= ~UCTXIFG;

    // Again see in430.h for more details
    // exits LPM0 modes.
    __low_power_mode_off_on_exit();
}

void main() {
    // Turn off the watchdog
    WDTCTL = WDTPW | WDTHOLD;

    // Buffer to hold output
    unsigned char arr[5] = {}

    init_i2c();
    data_len = 2;
    write_i2c(arr);
}