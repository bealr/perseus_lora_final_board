#include <xc.h>
#include "I2C_d.h"

void I2C_init(void)
{   
    SSPCON1 = 0b00101000;
    SSPCON2 = 0;
    SSPADD  = 254;
    SSPSTAT = 0b10000000;
    TRISAbits.TRISA1 = 0;
    TRISAbits.TRISA2 = 0;
}

void I2C_write_single_byte(char i2c_addr, char data)
{
    I2C_busy();
    SSPCON2bits.SEN = 1; // start bit
    while(SSPCON2bits.SEN);
    
    I2C_busy();
    SSPBUF = (i2c_addr<<1) & 0xFE; // write mode
    
    wait_ack();
    
    I2C_busy();
    SSPBUF = data; // data
    
    wait_ack();
    
    I2C_busy();
    SSPCON2bits.PEN = 1; // stop bit
    while(SSPCON2bits.PEN);
    
    __delay_ms(1);
}


void I2C_write_multiple_bytes(char i2c_addr, char* data, char nb_bytes)
{
    char i;
    I2C_busy();
    SSPCON2bits.SEN = 1; // start bit
    while(SSPCON2bits.SEN);
    
    I2C_busy();
    SSPBUF = (i2c_addr<<1) & 0xFE; // write mode
    
    wait_ack();
    
    for(i = 0; i < nb_bytes; i++) {
        I2C_busy();
        SSPBUF = data[i]; // data
        I2C_busy();
        wait_ack();
    }
    
    I2C_busy();
    SSPCON2bits.PEN = 1; // stop bit
    while(SSPCON2bits.PEN);
    
    __delay_ms(1);
}

void I2C_read_single_byte(char i2c_addr, char* data)
{
    I2C_busy();
    SSPCON2bits.SEN = 1; // start bit
    while(SSPCON2bits.SEN);
    
    I2C_busy();
    SSPBUF = (i2c_addr<<1) & 0xFF; // read mode
    
    wait_ack();
    
    I2C_busy();
    SSPCON2bits.RCEN = 1;
    
    I2C_busy();
    *data = SSPBUF;
    
    I2C_busy();
    SSPCON2bits.ACKDT = 1;
    SSPCON2bits.ACKEN = 1;
    
    I2C_busy();
    SSPCON2bits.PEN = 1; // stop bit
    while(SSPCON2bits.PEN);
    
    __delay_ms(1);
}

void I2C_read_multiple_bytes(char i2c_addr, char* data, char nb_bytes)
{
    char i;
    I2C_busy();
    SSPCON2bits.SEN = 1; // start bit
    while(SSPCON2bits.SEN);
    
    I2C_busy();
    SSPBUF = (i2c_addr<<1) & 0xFF; // read mode
    
    wait_ack();
    
    for(i = 0; i < nb_bytes; i++) {
        I2C_busy();
        SSPCON2bits.RCEN = 1;
    
        I2C_busy();
        data[i] = SSPBUF;

        I2C_busy();
        
        SSPCON2bits.ACKDT = (i < nb_bytes - 1) ? 0 : 1;
        SSPCON2bits.ACKEN = 1;
    }
    
    I2C_busy();
    SSPCON2bits.PEN = 1; // stop bit
    while(SSPCON2bits.PEN);
    
    __delay_ms(1);
}

void I2C_busy()
{
    while((SSPCON2 & 0x1F) || (SSPSTAT & 0x04));
}

void wait_ack()
{
    char timeout_manouche=100;
    
    while (!SSPCON2bits.ACKSTAT && timeout_manouche)
    {
        timeout_manouche--;
        __delay_us(1);
    }
}