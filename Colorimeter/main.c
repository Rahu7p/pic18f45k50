/* 
 * File:   main.c
 * Author: rahu7p
 */

#include <stdint.h>
#include "main.h"
#include "i2c.h"
#include "color.h"

#define _XTAL_FREQ 16000000

void CLK_Initialize( void );
void PORT_Initialize( void );

void main()
{
    CLK_Initialize( );
    PORT_Initialize( );
    I2C_Init_Master(I2C_100KHZ);            // Inicializa el protocolo I2C en modo maestro
    Color_Initialize();
    
    while(1)
    {
        Color_Read();
        Color_Display();
    }
}

void CLK_Initialize( void ){
    OSCCONbits.IRCF = 0b111;//      set HFINTOSC to 16 MHz
    SLRCON = 0;//                   set a standard slew rate in pin PORTS 
}

void PORT_Initialize( void ){
    //set pin RB1 as MSSP I2C clock output
    ANSELBbits.ANSB1 = 0;
    TRISBbits.TRISB1 = 1;
    //set pin RB0 as MSSP I2C data I/O
    ANSELBbits.ANSB0 = 0;
    TRISBbits.TRISB0 = 1;       
}
