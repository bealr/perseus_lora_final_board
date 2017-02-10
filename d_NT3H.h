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

#define NT3H_I2C_ADDR 0b1010101

void NT3H_init();
int NT3H_read(char addr, char* data);
int NT3H_write(char addr, char* data);



#endif	/* D_NT3H_H */

