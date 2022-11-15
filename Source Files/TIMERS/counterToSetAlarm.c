/*
 * File:                main.c
 * Author:              rahu7p
 * Development board:   Curiosity HPC
 * MCU:                 PIC18f45k50
 * 
 * This program turns ON an alarm (LED D3) for 1 second when 10 events (falling
 * edges) are counted using Timer 0 module. The 1 second delay is also
 * implemented with Timer 0. 
 * The S1 push button is used as follows: 
 *  - when pressed, generates the falling-edge signal using pin RB3 
 *  - when released, returns the pin RB3 to high state 
 * A jumper (connection) is needed between pin RB3 to pin RA4 (T0CKI).
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
void TIMER0_counter_10e( void );
//+++++++++++++++++++++++++++++MAIN SECTION+++++++++++++++++++++++++++++++++++++
void main( void ){
    CLK_Initialize( );//                        Clock initializations
    PORT_Initialize( );//                       PORT initializations
    while( 1 ){
        TIMER0_counter_10e( );//                count 10 events
        LATAbits.LATA5 = 1;//                   turn ON Led D3 (alarm)
        TIMER0_delay_1s( );//                   1 second delay
        LATAbits.LATA5 = 0;//                   turn OFF Led D3 (alarm)
        /* STEP 3 Ends */
    }    
}
//++++++++++++++++++++++++++FUNCTIONs SECTION+++++++++++++++++++++++++++++++++++
void CLK_Initialize( void ){
    OSCCONbits.IRCF = 0b111;//          set HFINTOSC to 16 MHz
    SLRCON = 0;//                       set a standard slew rate in pin PORTS 
}
void PORT_Initialize( void ){
    /* STEP 1 Begins */
    LATA = LATA & 0b00011111;//         clear the LEDs (RA7 to RA5)
    TRISA = TRISA & 0b00011111;//       set RA7-RA5 as outputs
    TRISAbits.TRISA4 = 1;//             set RA4 as T0CKI
    //set RB4 as input (S1 button)
    ANSELBbits.ANSB4 = 0;
    TRISBbits.TRISB4 = 1;
    //set RB3 as output (signal to generate the falling edge events)
    ANSELBbits.ANSB3 = 0;
    TRISBbits.TRISB3 = 0;
    LATBbits.LATB3 = 1;//               initialize the signal to high state
    /* STEP 1 Ends */
}
void TIMER0_delay_1s( void ){
    T0CON = 0b00000101;//               initialize Timer 0 with:
    //                                  mode=16bit,internal clock,1:64 presc
    TMR0H = 0x0B;//                     load initial count, high-byte first  
    TMR0L = 0xDC;//                     then low-byte      
    INTCONbits.TMR0IF = 0;//            clear the Timer 0 overflow flag
    T0CONbits.TMR0ON = 1;//             enables Timer 0
    while( !INTCONbits.TMR0IF ){ }//    wait for Timer 0 overflow
    T0CONbits.TMR0ON = 0;//             stop Timer 0
}
void TIMER0_counter_10e( void ){
    /* STEP 2 Begins */
    T0CON = 0b01111000;//               initialize Timer 0 with:
    //                                  mode=8bit,external clk(falling),1:1
    TMR0L = 246;//                      load initial count      
    INTCONbits.TMR0IF = 0;//            clear the Timer 0 overflow flag
    T0CONbits.TMR0ON = 1;//             enables Timer 0
    /* STEP 2 Ends */
    /* STEP 3 Begins */
    while( !INTCONbits.TMR0IF ){//      wait for Timer 0 overflow 
        //while waiting, generate the falling edges using the S1 button and RB3
        if( !PORTBbits.RB4 ){//         if S1 is pressed
            LATBbits.LATB3 = 0;//       make the signal transition high-to-low
            while( !PORTBbits.RB4 );//  wait until S1 is released
            LATBbits.LATB3 = 1;//       return the signal to high state
        }
    }
    T0CONbits.TMR0ON = 0;//             stop Timer 0
}
//+++++++++++++++++++++++++++++++++++END++++++++++++++++++++++++++++++++++++++++
