// PIC12LF1552 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will not cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "d_NT3H.h"


void init();

void main()
{
    char nt3h_buffer[16] = {0};
    init();
    I2C_init();
    //NT3H_init();
    
    LATA4 = 1;
    LATA5 = 1;
    
    for (;;) {
        NT3H_read_data_block(0x00, nt3h_buffer);
        if (nt3h_buffer[0] == 0x04)
            LATA5 = 0;
        
        __delay_ms(100);
    }
    
    for(;;) {
        
        /* Check I2C with ntag
         * try to read addr 0, it should return 0x04*/
        NT3H_read_data_block(0x00, nt3h_buffer);
        if(nt3h_buffer[0] == 0x04) {
            LATAbits.LATA5 = 1;
            __delay_ms(1000);
            LATAbits.LATA5 = 0;
        }
        __delay_ms(2000);
         
        
        /* main code
         * 

        do {
            NT3H_read(0x01, ntag_buffer);
        }while(ntag_buffer[0] == 'S' && ntag_buffer[1] == 'T' && ntag_buffer[2] == 'A' && ntag_buffer[3] == 'R' && ntag_buffer[4] == 'T');
        LATAbits.LATA4 = 1;

         NT3H_clear_data_block(0x01);
         */        
    }
}


void init() {
    //__delay_ms(3000);//useful during program upload with I2C
    //32MHz
    OSCCONbits.IRCF = 0b1110;
    OSCCONbits.SPLLEN = 1;
    
    ANSELA = 0;
    
    //Power Switch
    TRISAbits.TRISA4 = 0; // sortie - sortie avec le shunt ...
    LATA4 = 1;
    
    //IRQ
    TRISAbits.TRISA5 = 0;
    LATAbits.LATA5 = 0;
    
    // I2C alternate pin
    APFCONbits.SDSEL = 0; // SDA on RA2
    
    /* Interrupt on A5 
     * enable interrupts
     */
}