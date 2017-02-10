#include <xc.h>
#include "I2C_d.h"

/* --- ADDRESS ---
 * LCD  : 0x3F
 * ALIM : 0x14
 * SELE : 0x28
 
 */


void I2C_init(void)
{   
    /*
    TRISCbits.RC3 = 1;
    TRISCbits.RC4 = 1;
    ANSELCbits.ANSC3 = 0;
    ANSELCbits.ANSC4 = 0;
    SSP2CON1bits.SSPEN = 1;
     */
    SSP1CON1 = 0b00101000;
    SSP1CON2 = 0;
    SSP1ADD  = 254;
    SSP1STAT = 0b10000000;
    TRISAbits.TRISA1 = 0;
    TRISAbits.TRISA2 = 0;
}

void I2C_write(char i2c_addr, char data)
{
    I2C_busy();
    SSPCON2bits.SEN = 1; // start bit
    while(SSPCON2bits.SEN);
    
    I2C_busy();
    SSP1BUF = (i2c_addr<<1) & 0xFE; // write mode
    
    wait_ack();
    
    I2C_busy();
    SSP1BUF = data; // data
    
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
    SSP1BUF = (i2c_addr<<1) & 0xFE; // write mode
    
    wait_ack();
    
    for(i = 0; i < nb_bytes; i++) {
        I2C_busy();
        SSP1BUF = data[i]; // data
        I2C_busy();
        wait_ack();
    }
    
    I2C_busy();
    SSPCON2bits.PEN = 1; // stop bit
    while(SSPCON2bits.PEN);
    
    __delay_ms(1);
}

void I2C_read(char i2c_addr, char* data)
{
    I2C_busy();
    SSPCON2bits.SEN = 1; // start bit
    while(SSPCON2bits.SEN);
    
    I2C_busy();
    SSP1BUF = (i2c_addr<<1) & 0xFF; // read mode
    
    wait_ack();
    
    I2C_busy();
    SSP1BUF = data; // data
    
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
    SSP1BUF = (i2c_addr<<1) & 0xFF; // read mode
    
    wait_ack();
    
    for(i = 0; i < nb_bytes; i++) {
        I2C_busy();
        SSP1BUF = data[i]; // data
        I2C_busy();
        SSPCON2bits.ACKDT = 1;
        //wait end of ack -> SSPCON2bits.ACKEN
    }
    
    I2C_busy();
    SSPCON2bits.PEN = 1; // stop bit
    while(SSPCON2bits.PEN);
    
    __delay_ms(1);
}


void I2C_busy(void)
{
    while((SSP1CON2 & 0x1F) || (SSP1STAT & 0x04));
}

void wait_ack(void)
{
    char timeout_manouche=100;
    
    while (!SSPCON2bits.ACKSTAT && timeout_manouche)
    {
        timeout_manouche--;
        __delay_us(1);
    }
}