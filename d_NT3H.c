#include "d_NT3H.h"

void NT3H_init() {
    
}

void NT3H_read_data_block(char mem_addr, char* data) {
    NT3H_wait_until_available();
    I2C_write_single_byte(NT3H_I2C_ADDR, mem_addr);
    I2C_read_multiple_bytes(NT3H_I2C_ADDR, data, 16);
    //I2C_LOCKED reset
    NT3H_write_NS_REG_byte(0, 0x40);
}

void NT3H_write_data_block(char mem_addr, char* data) {
    char bytes[17] = {0};
    bytes[0] = mem_addr;
    char i;
    for(i = 1; i<17; i++)
        bytes[i] = data[i-1];
    NT3H_wait_until_available();
    I2C_write_multiple_bytes(NT3H_I2C_ADDR, bytes, 17);
    //I2C_LOCKED reset
}

void NT3H_clear_data_block(char mem_addr) {
    char clear_buffer[16] = {0};
    NT3H_wait_until_available();
    NT3H_write_data_block(mem_addr, clear_buffer); 
    //I2C_LOCKED reset   
}

char NT3H_read_NS_REG_byte() {
    char data[2] = {NT3H_SESSION_REG_ADDR, NT3H_NS_REG_ADDR};
    char reg_data = 0;
    I2C_write_multiple_bytes(NT3H_I2C_ADDR, data, 2);
    I2C_read_single_byte(NT3H_I2C_ADDR, &reg_data);
    return reg_data;
}

void NT3H_write_NS_REG_byte(char ns_reg_data, char mask) {
    char data[4] = {NT3H_SESSION_REG_ADDR, NT3H_NS_REG_ADDR, 0};
    data[2] = mask;
    data[3] = ns_reg_data;
    NT3H_wait_until_available();
    I2C_write_multiple_bytes(NT3H_I2C_ADDR, data, 4);
}

void NT3H_wait_until_available() {
    //while RF_LOCKED && I2C_LOCKED && EEPROM_WR_BUSY
    while(NT3H_read_NS_REG_byte() & 0x46);
}