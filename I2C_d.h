/* 
 * File:   I2C_d.h
 * Author: rbeal
 *
 * Created on October 10, 2016, 6:59 PM
 */

#ifndef I2C_D_H
#define	I2C_D_H

void I2C_init();
void I2C_write_single_byte(char device_addr, char addr, char data);
void I2C_write_multiple_bytes(char device_addr, char addr, char* data, char nb_bytes);
char I2C_read_single_byte(char device_addr, char addr);
void I2C_read_multiple_bytes(char device_addr, char addr, char* data, char nb_bytes);
char I2C_read_reg_byte(char device_addr, char addr, char reg_addr);
void I2C_busy();
void wait_ack();

#endif	/* I2C_D_H */

