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
//...
//Enumerations
//...
//Variables, constants
//...
//+++++++++++++++++++++++++++++++ISRs SECTION+++++++++++++++++++++++++++++++++++
//ISR for high-priority... ORG 0x08
__interrupt( high_priority ) void high_isr( void ){
    PIR2bits.CCP2IF = 0;//          clears compare match flag 
    TMR1H = 0;
    TMR1L = 0;//                    clear TMR1 count
}
//ISR for low-priority... ORG 0x18
__interrupt( low_priority ) void low_isr( void ) { 
    Nop( ); //Function to consume one Instruction Cycle
}
//++++++++++++++++++++++++FUNCTIONs DECLARATION+++++++++++++++++++++++++++++++++
void CLK_Initialize( void );
void PORT_Initialize( void );
void INTERRUPTS_Initialize( void );
void CCP_Compare_Initialize( void );
void TMR1_Initialize( void );
//+++++++++++++++++++++++++++++MAIN SECTION+++++++++++++++++++++++++++++++++++++
void main( void ){  
    CLK_Initialize( );
    PORT_Initialize( );
    INTERRUPTS_Initialize( );
    CCP_Compare_Initialize( );
    TMR1_Initialize( );
    while( 1 ){

    }
}
//++++++++++++++++++++++++++FUNCTIONs SECTION+++++++++++++++++++++++++++++++++++
void CLK_Initialize( void ){
    OSCCONbits.IRCF = 0b111;//      set HFINTOSC to 16 MHz
    SLRCON = 0;//                   set a standard slew rate in pin PORTS 
}
void PORT_Initialize( void ){  
    TRISCbits.TRISC1 = 0;//         set pin RC1 as the Compare 2 output
}
void INTERRUPTS_Initialize( void ){
    RCONbits.IPEN   = 0;//          disables priority levels on interrupts        
    PIR2bits.CCP2IF = 0;//          clears compare match flag 
    PIE2bits.CCP2IE = 1;//          enables standard compare mode interrupt
    INTCONbits.PEIE = 1;//          enable all peripheral interrupts 
    INTCONbits.GIE  = 1;//          enables all unmasked interrupts    
}
void CCP_Compare_Initialize( void ){
    CCPR2H = 0x03;
    CCPR2L = 0xE7;//                match value to be constantly compared (TMR1)
    CCPTMRSbits.C2TSEL  = 0;//      select TMR1 for standard compare mode (CCP2)
    CCP2CONbits.CCP2M   = 0b0010;// compare mode: toggle output on match
}
void TMR1_Initialize( void ){
    T1CONbits.TMR1ON  = 0;//        stops Timer 1
    T1CONbits.RD16    = 1;//        16-bit operation of TMR1 register
    TMR1H = 0;
    TMR1L = 0;//                    clear TMR1 count
    T1CONbits.TMR1CS  = 0b00;//     instr. cycle as the clock source (Fosc/4)
    T1CONbits.T1CKPS  = 0b00;//     1:1 prescaler value
    T1CONbits.TMR1ON  = 1;//        enables Timer 1
    T1GCONbits.TMR1GE = 0;//        TMR1 counts regardless of gate function
}
//+++++++++++++++++++++++++++++++++++END++++++++++++++++++++++++++++++++++++++++
