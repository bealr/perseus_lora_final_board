/* 
 * File:   d_NT3H.h
 * Author: julien
 *
 * Created on February 9, 2017, 4:57 PM
 */

#ifndef D_NT3H_H
#define	D_NT3H_H

#include <xc.h>
#include "I2C_d.h"

#define NT3H_I2C_ADDR 0xAA
#define NT3H_SESSION_REG_ADDR 0xFE
#define NT3H_NS_REG_ADDR 0x06

void NT3H_init();
void NT3H_read_data_block(char mem_addr, char* data);
void NT3H_write_data_block(char mem_addr, char* data);
void NT3H_clear_data_block(char mem_addr);
char NT3H_read_NS_REG_byte();
void NT3H_write_NS_REG_byte(char ns_reg_data, char mask);

void NT3H_wait_until_available();

#endif	/* D_NT3H_H */

