/*
 * File:                main.c
 * Author:              rahu7p
 * Development board:   Curiosity HPC
 * MCU:                 PIC18f45k50
 * 
 * This program increments a 4-bit binary counter using the S1 button. The
 * counter is showed in the LEDS (D5 to D2). The program demonstrates that the 
 * increments are uncertain due to S1 bouncing. 
 */

//++++++++++++++++++++++++++++LIBRARIEs SECTION+++++++++++++++++++++++++++++++++
#include "main.h"
#include <xc.h>
#include <stdint.h>//               unified data types for all XC compilers
//++++++++++++++++++++++++++++DIRECTIVEs SECTION++++++++++++++++++++++++++++++++
//Defines
//..
//Enumerations
typedef enum{//                     assign a constant value to a label
    uNibble = 4              
}uSer;
//Variables, constants
//...
//+++++++++++++++++++++++++++++++ISRs SECTION+++++++++++++++++++++++++++++++++++
//ISR for high-priority... ORG 0x08
__interrupt( high_priority ) void high_isr( void ){
    Nop( ); //Function to consume one Instruction Cycle
}
//ISR for low-priority... ORG 0x18
__interrupt( low_priority ) void low_isr( void ) { 
    Nop( ); //Function to consume one Instruction Cycle
}
//++++++++++++++++++++++++FUNCTIONs DECLARATION+++++++++++++++++++++++++++++++++
void CLK_Initialize( void );
void PORT_Initialize( void );
//+++++++++++++++++++++++++++++MAIN SECTION+++++++++++++++++++++++++++++++++++++
void main( void ){
    uint8_t bCounter = 0;
    CLK_Initialize( );//            Clock initializations
    PORT_Initialize( );//           PORT initializations
    while( 1 ){  
        if( PORTBbits.RB4 == 0 ){// if button (S1) is pressed then:
            bCounter++;//           increment the binary counter
            //write the counter value to the LEDs (upper part of Port A)
            LATA = (uint8_t)( bCounter << uNibble );
        }
    }
}
//++++++++++++++++++++++++++FUNCTIONs SECTION+++++++++++++++++++++++++++++++++++
void CLK_Initialize( void ){
    OSCCONbits.IRCF = 0b111;//      set HFINTOSC to 16 MHz
    SLRCON = 0;//                   set a standard slew rate in pin PORTS 
}
void PORT_Initialize( void ){
    LATA = LATA & 0b00001111;//     clear the LEDs (RA7 to RA4)
    TRISA = TRISA & 0b00001111;//   set RA7-RA4 as outputs
    ANSELBbits.ANSB4 = 0;//         enable digital input buffer in RB4
    TRISBbits.TRISB4 = 1;//         set RB4 as input
}
//+++++++++++++++++++++++++++++++++++END++++++++++++++++++++++++++++++++++++++++
