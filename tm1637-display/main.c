//i2c implemented using i2c_ref.pdf in this repo

#include <msp430.h>

#define MODE_INPUT 0
#define MODE_OUTPUT 1

#define I2C_CLK 3
#define I2C_SDA 4

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
    P3SEL |= 0x03;      // select 3.0/3.1 for i2c
    UCB0CTL1 |= USCWRST;
    UCB0CTL0 = UCMST | UC_MODE_3
}
void write_i2c(unsigned char *data, unsigned char len) {
    // Start sequence
    write_pin(I2C_SDA, 0);

}
void read_i2c() {

}

void main() {
    WDTCTL = WDTPW | WDTHOLD;

    P1DIR = 0x01;

    while(1) {
        volatile unsigned long i = 10000;
        P1OUT ^= 0x01; // Switch pin's level
        while(i--);
    }   
}