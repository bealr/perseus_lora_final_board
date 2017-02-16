#include <xc.h>
#include "I2C_d.h"

void I2C_init(void)
{   
    
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA2 = 1;
    
    SSPCON1 = 0b00001000; // I2C = master mode
    SSPCON2 = 0;
    SSPADD  = 128;
    SSPSTAT = 0b00000000;
    
    SSPCON1bits.SSPEN = 1;
    
}

void pouet_send_byte(char device_addr, char addr, char data) {

    SSPCON2bits.SEN = 1; // start bit
    I2C_busy();
    
    SSPBUF = device_addr | 0x00; // write mode
    I2C_busy();
    wait_ack();
    
    SSPBUF = addr; // addr
    I2C_busy();
    wait_ack();
    
    SSPBUF = data; // data
    I2C_busy();
    wait_ack();
    
    SSPCON2bits.PEN = 1; // stop bit
    I2C_busy();
}

char pouet_receive_byte(char device_addr, char addr) {

    char tmp;
    
    SSPCON2bits.SEN = 1; // start bit
    I2C_busy();
    
    SSPBUF = device_addr | 0x00; // write mode
    I2C_busy();
    wait_ack();
    
    SSPBUF = addr; // addr
    I2C_busy();
    wait_ack();
    
    SSPCON2bits.PEN = 1; // stop bit
    I2C_busy();
    
    SSPCON2bits.RSEN = 1; // Restart bit
    I2C_busy();
    
    
    SSPBUF = device_addr | 0x01; // read mode
    I2C_busy();
    wait_ack();
    
    SSPCON2bits.RCEN = 1;
    I2C_busy();
    
    tmp = SSPBUF;
    ACKDT = 1;
    
    SSPCON2bits.PEN = 1; // stop bit
    I2C_busy();
    
    return tmp;
    
}

void I2C_write_single_byte(char device_addr, char addr, char data)
{
    SSPCON2bits.SEN = 1; // start bit
    I2C_busy();
    
    SSPBUF = device_addr | 0x00; // write mode
    I2C_busy();
    wait_ack();
    
    SSPBUF = addr; // addr
    I2C_busy();
    wait_ack();
    
    SSPBUF = data; // data
    I2C_busy();
    wait_ack();
    
    SSPCON2bits.PEN = 1; // stop bit
    I2C_busy();
}


void I2C_write_multiple_bytes(char device_addr, char addr, char* data, char nb_bytes)
{
    char i;
    
    SSPCON2bits.SEN = 1; // start bit
    I2C_busy();
    
    SSPBUF = device_addr | 0x00; // write mode
    I2C_busy();
    wait_ack();
    
    SSPBUF = addr; // addr
    I2C_busy();
    wait_ack();
    
    for(i = 0; i < nb_bytes; i++) {
        SSPBUF = *(data + i); // data(s)
        I2C_busy();
        wait_ack();
    }
    
    SSPCON2bits.PEN = 1; // stop bit
    I2C_busy();
}

char I2C_read_single_byte(char device_addr, char addr)
{
    char tmp;
    
    SSPCON2bits.SEN = 1; // start bit
    I2C_busy();
    
    SSPBUF = device_addr | 0x00; // write mode
    I2C_busy();
    wait_ack();
    
    SSPBUF = addr; // addr
    I2C_busy();
    wait_ack();
    
    SSPCON2bits.PEN = 1; // stop bit
    I2C_busy();
    
    SSPCON2bits.RSEN = 1; // Restart bit
    I2C_busy();
    
    
    SSPBUF = device_addr | 0x01; // read mode
    I2C_busy();
    wait_ack();
    
    SSPCON2bits.RCEN = 1;
    I2C_busy();
    
    tmp = SSPBUF;
    ACKDT = 1;
    
    SSPCON2bits.PEN = 1; // stop bit
    I2C_busy();
    
    return tmp;
}

void I2C_read_multiple_bytes(char device_addr, char addr, char* data, char nb_bytes)
{
    SSPCON2bits.SEN = 1; // start bit
    I2C_busy();
    
    SSPBUF = device_addr | 0x00; // write mode
    I2C_busy();
    wait_ack();
    
    SSPBUF = addr; // addr
    I2C_busy();
    wait_ack();
    
    SSPCON2bits.PEN = 1; // stop bit
    I2C_busy();
    
    SSPCON2bits.RSEN = 1; // Restart bit
    I2C_busy();
    
    
    SSPBUF = device_addr | 0x01; // read mode
    I2C_busy();
    wait_ack();
    
    for(i = 0; i < nb_bytes; i++) {
        
        SSPCON2bits.RCEN = 1;
        I2C_busy();

        *(data + i) = SSPBUF;
        SSPCON2bits.ACKDT = (i < nb_bytes - 1) ? 0 : 1;
    }
    
    SSPCON2bits.PEN = 1; // stop bit
    I2C_busy();
}

void I2C_busy()
{
    while((SSPCON2 & 0x1F) || (SSPSTAT & 0x04));
}

void wait_ack()
{
    char timeout_manouche=100;
    
    while (SSPCON2bits.ACKSTAT && timeout_manouche)
    {
        timeout_manouche--;
        __delay_us(1);
    }
}