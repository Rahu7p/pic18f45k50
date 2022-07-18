/*
 * File:                main.c
 * Author:              rahu7p
 * Development board:   Curiosity HPC
 * MCU:                 PIC18f45k50
 * 
 */

//++++++++++++++++++++++++++++LIBRARIEs SECTION+++++++++++++++++++++++++++++++++
#include "main.h"
#include <xc.h>
#include <stdint.h>//               unified data types for all XC compilers
//++++++++++++++++++++++++++++DIRECTIVEs SECTION++++++++++++++++++++++++++++++++
//Defines
#define _XTAL_FREQ 16000000//       MACRO for __delay_ms() function
//Enumerations
//...
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
void DAC_Initialize( void );
//+++++++++++++++++++++++++++++MAIN SECTION+++++++++++++++++++++++++++++++++++++
void main( void ){  
    CLK_Initialize( );//                Clock initializations
    PORT_Initialize( );//               PORT initializations
    DAC_Initialize( );//                UART initializations
    while( 1 ){
        //Read the switches (digital value) to output a voltage 
        VREFCON2bits.DACR = PORTB & 0b00011111;
    }
}
//++++++++++++++++++++++++++FUNCTIONs SECTION+++++++++++++++++++++++++++++++++++
void CLK_Initialize( void ){
    OSCCONbits.IRCF = 0b111;//          set HFINTOSC to 16 MHz
    SLRCON = 0;//                       set a standard slew rate in pin PORTS 
}
void PORT_Initialize( void ){  
    ANSELAbits.ANSA2 = 1;//             set pin RA2 as the DAC reference output
    //Pin configurations for the switches
    ANSELB  = ANSELB & 0b11100000;//    enable digital input buffer RB4-RB0
    TRISB   = TRISB  | 0b00011111;//    set RB4-RB0 as inputs
}
void DAC_Initialize( void ){
    VREFCON1bits.DACEN  = 0;//          DAC disabled
    VREFCON1bits.DACOE  = 1;//          DAC voltage level at the DACOUT pin
    VREFCON1bits.DACPSS = 0b00;//       VDD as the DAC positive source
    VREFCON1bits.DACNSS = 0;//          VSS as the DAC negative source
    VREFCON1bits.DACEN  = 1;//          DAC enabled
}
//+++++++++++++++++++++++++++++++++++END++++++++++++++++++++++++++++++++++++++++
