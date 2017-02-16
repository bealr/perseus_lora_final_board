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
    do {
        NT3H_read_data_block(0x01, nt3h_buffer);
    }while(nt3h_buffer[0] == 'S' && nt3h_buffer[1] == 'T' && nt3h_buffer[2] == 'A' && nt3h_buffer[3] == 'R' && nt3h_buffer[4] == 'T');
    LATAbits.LATA4 = 1;
    NT3H_clear_data_block(0x01);
    
    //LATA5 = 1;
    
    /*
    // Check I2C with ntag : try to read addr 0, it should return 0x04
    for(;;) {
        NT3H_read_data_block(0x00, nt3h_buffer);
        if(nt3h_buffer[0] == 0x04) {
            LATAbits.LATA5 = 1;
            __delay_ms(1000);
            LATAbits.LATA5 = 0;
        }
        __delay_ms(2000);
    }*/
        



            
}


void init() {
    //32MHz
    OSCCONbits.IRCF = 0b1110;
    OSCCONbits.SPLLEN = 1;
    
    ANSELA = 0;
    
    //Power Switch
    TRISAbits.TRISA4 = 0; // sortie - sortie avec le shunt ...
    LATA4 = 0;
    
    //IRQ
    //TRISAbits.TRISA5 = 1;
    //debug
    TRISAbits.TRISA5 = 0;
    LATA5 = 0;
    
    // I2C alternate pin
    APFCONbits.SDSEL = 0; // SDA on RA2
    
    //timer
    OPTION_REGbits.TMR0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS = 0x07;
    //timer interrupt
    INTCONbits.GIE = 1;
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;
    TMR0 = 100;
}


void timer0_isr() {
    static int count = 0;
    
    if(INTCONbits.TMR0IF) {
        if(++count == 200) { //every second
            LATA5 = ~LATA5;
            count = 0;
        }
        
        
        /*if(++count == 6000) { //every 30s
            //get_vbat
            count = 0;
        }*/
        INTCONbits.TMR0IF = 0;
    }
}