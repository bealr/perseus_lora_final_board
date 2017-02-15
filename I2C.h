/* 
 * File:   I2C.h
 * Author: rbeal
 *
 * Created on August 15, 2016, 1:21 PM
 */

#ifndef I2C_H
#define	I2C_H

void init_I2C();

void transmit_I2C(char addr, char data);
char receive_I2C(char addr);

void clear_bits_I2C(char addr, char data);
void set_bits_I2C(char addr, char data);

void I2C_wait();


#endif	/* I2C_H */

