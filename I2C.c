/* 
 * File:   I2C.c
 * Author: rbeal
 *
 * Created on August 15, 2016, 1:21 PM
 */

#include "I2C.h"
#include <xc.h>

void init_I2C()
{
    TRISB0 = 1;
    TRISB1 = 1;
    
    //SSPSTAT = 0x80;
    SSPCON1 = 0x38;
    SSPCON2 = 0x00;
    
    SSPSTATbits.CKE=1;
    SSPSTATbits.SMP=1;
    
    SSPADD = 0x13;
    
    SSPEN = 1;
}

void I2C_wait()
{
    while((SSPCON2 & 0x1F) || (SSPSTAT & 0x04))
    {
        NOP();
    }
}

void transmit_I2C(char addr, char data){
   
    I2C_wait();
    SEN = 1; // start bit
    
    I2C_wait();
    SSPBUF = 0x54; // device addr
    
    while (ACKSTAT);
    
    I2C_wait();
    SSPBUF = addr; // addr
    
    while (ACKSTAT);
    
    I2C_wait();
    SSPBUF = data; // data
    
    while (ACKSTAT);
    
    I2C_wait();
    PEN = 1; // stop bit
    
    
    __delay_ms(5);        // for debugging
}

char receive_I2C(char addr)
{
    SSPIF = 0;
    SEN = 1; // start bit
    while(!SSPIF);
    
    SSPIF = 0;
    SSPBUF = 0x50; // device addr
    while(!SSPIF);
    
    while (ACKSTAT);
    
    SSPIF = 0;
    SSPBUF = addr; // addr
    while(!SSPIF);
    
    while (ACKSTAT);
    
    SSPIF = 0;
    RSEN = 1; // data
    while(!SSPIF);
    
    SSPIF = 0;
    RCEN = 1; // receive mode = 1
    while(!SSPIF);
    
    SSPIF = 0;
    ACKEN = 0; // receive finished
    while(!SSPIF);
    
    return SSPBUF;
}

void clear_bits_I2C(char addr, char mask)
{
    char tmp;
    
    tmp = receive_I2C(addr);
    transmit_I2C(addr, tmp&(~mask)); // change only masked bit
}

void set_bits_I2C(char addr, char mask)
{
    char tmp;
    
    tmp = receive_I2C(addr);
    transmit_I2C(addr, tmp|mask);   // change only masked bit
}