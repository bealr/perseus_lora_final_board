#include "d_NT3H.h"

void NT3H_init() {
    
}

void NT3H_read_data_block(char mem_addr, char* data) {
    //NT3H_wait_until_available();
    I2C_read_multiple_bytes(NT3H_I2C_ADDR, mem_addr, data, 16);
    
    NT3H_write_NS_REG_byte(0, 0x40);//I2C_LOCKED reset
}

void NT3H_write_data_block(char mem_addr, char* data) {
    //NT3H_wait_until_available();
    I2C_write_multiple_bytes(NT3H_I2C_ADDR, mem_addr, data, 16);
    
    NT3H_write_NS_REG_byte(0, 0x40);//I2C_LOCKED reset
}

void NT3H_clear_data_block(char mem_addr) {
    char clear_buffer[16] = {0};
    NT3H_wait_until_available();
    NT3H_write_data_block(mem_addr, clear_buffer); 

    NT3H_write_NS_REG_byte(0, 0x40);//I2C_LOCKED reset
}

char NT3H_read_NS_REG_byte() {
    return I2C_read_reg_byte(NT3H_I2C_ADDR, NT3H_SESSION_REG_ADDR, NT3H_NS_REG_ADDR);
}

void NT3H_write_NS_REG_byte(char ns_reg_data, char mask) {
    char data[3] = {NT3H_NS_REG_ADDR};
    data[1] = mask;
    data[2] = ns_reg_data;
    NT3H_wait_until_available();
    I2C_write_multiple_bytes(NT3H_I2C_ADDR, NT3H_SESSION_REG_ADDR, data, 3);
}

void NT3H_wait_until_available() {
    //while RF_LOCKED && I2C_LOCKED && EEPROM_WR_BUSY
    while(NT3H_read_NS_REG_byte() & 0x46);
}