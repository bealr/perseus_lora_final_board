#include "PIC18F44K22_d.h"

float get_vbat()
{
    char buffer[10] = {0};
    I2C_read_multiple_bytes(PIC18F44K22_I2C_ADDR, PIC18F44K22_I2C_VBAT, buffer, 10);
    return 0.;
}
