#include "d_NT3H.h"

void NT3H_init() {
    
}

int NT3H_read(char mem_addr, char* data) {
    I2C_write_single_byte(NT3H_I2C_ADDR, mem_addr);
    I2C_read_multiple_bytes(NT3H_I2C_ADDR, data, 16);
}

int NT3H_write(char mem_addr, char* data) {
    char bytes[17] = {0};
    bytes[0] = mem_addr;
    char i;
    for(i = 1; i<17; i++)
        bytes[i] = data[i-1];
    I2C_write_multiple_bytes(NT3H_I2C_ADDR, bytes, 17);    
}