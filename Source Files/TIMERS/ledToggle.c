/*
 * File:                main.c
 * Author:              rahu7p
 * Development board:   Curiosity HPC
 * MCU:                 PIC18f45k50
 * 
 * This program toggles the LED D2 each 1 second. The 1 second delay is
 * implemented with the Timer 0 module.
 */

//++++++++++++++++++++++++++++LIBRARIEs SECTION+++++++++++++++++++++++++++++++++
#include "main.h"
#include <stdint.h>//               unified data types for all XC compilers
//++++++++++++++++++++++++++++DIRECTIVEs SECTION++++++++++++++++++++++++++++++++
//Defines
//...
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
void TIMER0_delay_1s( void );
//+++++++++++++++++++++++++++++MAIN SECTION+++++++++++++++++++++++++++++++++++++
void main( void ){
    CLK_Initialize( );//                        Clock initializations
    PORT_Initialize( );//                       PORT initializations
    while( 1 ){  
        LATAbits.LATA4 = LATAbits.LATA4 ^ 1;//  Toggle Led D2
        /* STEP 3 Ends */
        TIMER0_delay_1s( );//                   1 second delay
    }    
}
//++++++++++++++++++++++++++FUNCTIONs SECTION+++++++++++++++++++++++++++++++++++
void CLK_Initialize( void ){
    OSCCONbits.IRCF = 0b111;//          set HFINTOSC to 16 MHz
    SLRCON = 0;//                       set a standard slew rate in pin PORTS 
}
void PORT_Initialize( void ){
    /* STEP 1 Begins */
    LATA = LATA & 0b00001111;//         clear the LEDs (RA7 to RA4)
    TRISA = TRISA & 0b00001111;//       set RA7-RA4 as outputs
    /* STEP 1 Ends */
}
void TIMER0_delay_1s( void ){
    /* STEP 2 Begins */
    T0CON = 0b00000101;//               initialize Timer 0 with:
    //                                  mode=16bit,internal clock,1:64 presc
    TMR0H = 0x0B;//                     load initial count, high-byte first  
    TMR0L = 0xDC;//                     then low-byte      
    INTCONbits.TMR0IF = 0;//            clear the Timer 0 overflow flag
    T0CONbits.TMR0ON = 1;//             enables Timer 0
    /* STEP 2 Ends */
    /* STEP 3 Begins */
    while( !INTCONbits.TMR0IF ){ }//    wait for Timer 0 overflow
    T0CONbits.TMR0ON = 0;//             stop Timer 0
}
//+++++++++++++++++++++++++++++++++++END++++++++++++++++++++++++++++++++++++++++
