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
#include <math.h>
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
    Nop( );
}
//ISR for low-priority... ORG 0x18
__interrupt( low_priority ) void low_isr( void ) { 
    Nop( );
}
//++++++++++++++++++++++++FUNCTIONs DECLARATION+++++++++++++++++++++++++++++++++
void CLK_Initialize( void );
void PORT_Initialize( void );
void UART_Initialize( void );
void UART_TransmitByte( uint8_t data );
void UART_TransmitStr( uint8_t *pData, uint16_t size );
void CCP_Capture_Initialize( void );
void TMR1_Initialize( void );
uint32_t FREQ_Calculations( void );
void FREQ_Print( uint32_t freq );
//+++++++++++++++++++++++++++++MAIN SECTION+++++++++++++++++++++++++++++++++++++
void main( void ){  
    CLK_Initialize( );//                Clock initializations
    PORT_Initialize( );
    UART_Initialize( );
    CCP_Capture_Initialize( );
    TMR1_Initialize( );
    while( 1 ){
        FREQ_Print( FREQ_Calculations( ) );
    }
}
//++++++++++++++++++++++++++FUNCTIONs SECTION+++++++++++++++++++++++++++++++++++
void CLK_Initialize( void ){
    OSCCONbits.IRCF = 0b111;//          set HFINTOSC to 16 MHz
    SLRCON = 0;//                       set a standard slew rate in pin PORTS 
}
void PORT_Initialize( void ){  
    TRISCbits.TRISC1 = 1;//         set pin RC1 as the Capture 2 input
    //Pin configurations for the UART
    ANSELCbits.ANSC6 = 0;//         configure RC6 (TX) pin as:
    TRISCbits.TRISC6 = 1;//         EUSART asynchronous transmit data output
}
void UART_Initialize( void ){
    BAUDCON1bits.BRG16 = 1;//       16-bit baudrate
    //Configure a 9600 BAUDRATE according to TABLE 17-5
    SPBRGH1 = 0x01;       
    SPBRG1 = 0xA0;
    TXSTA1bits.BRGH = 1;//          high-speed
    TXSTA1bits.SYNC = 0;//          asynchronous mode
    RCSTA1bits.SPEN = 1;//          serial port enabled
    TXSTA1bits.TXEN = 1;//          transmit enabled
}
void UART_TransmitByte( uint8_t data ){
    while( TXSTA1bits.TRMT == 0 );//    wait until TSR is empty
    TXREG = data;//                     transmit a character
}
void UART_TransmitStr( uint8_t *pData, uint16_t size ){
    for( int i = 0; i < size; i++ ){
        UART_TransmitByte( *pData++ );
    }
}
void CCP_Capture_Initialize( void ){
    CCPTMRSbits.C2TSEL = 0;//       select TMR1 for standard compare mode (CCP2)
    CCP2CONbits.CCP2M = 0b0100;//   capture mode: every falling edge
}
void TMR1_Initialize( void ){
    T1CONbits.TMR1ON = 0;//         stops Timer 1
    T1CONbits.RD16 = 1;//           16-bit operation of TMR1 register
    TMR1H = 0;
    TMR1L = 0;//                    clear TMR1 count
    T1CONbits.TMR1CS = 0b00;//      instr. cycle as the clock source (Fosc/4)
    T1CONbits.T1CKPS = 0b11;//      1:8 prescaler value
    T1CONbits.TMR1ON = 1;//         enables Timer 1
    T1GCONbits.TMR1GE = 0;//        TMR1 counts regardless of gate function
}
uint32_t FREQ_Calculations( void ){
    uint8_t period1_high, period1_low, period2_high, period2_low;
    uint16_t period1, period2;
    uint32_t total, freq;
    PIR2bits.CCP2IF = 0;//          clears compare match flag (CCP2)
    TMR1H = 0;
    TMR1L = 0;//                    clear TMR1 count
    while( PIR2bits.CCP2IF == 0 ){ }
    period1_low = CCPR2L;//         read captured value, low byte first
    period1_high = CCPR2H;//        then high byte
    //concatenate two 8-bit variables into one 16-bit variable
    period1 = ( uint16_t )( ( period1_high << 8 ) | period1_low );
    PIR2bits.CCP2IF = 0;//          clears compare match flag (CCP2)
    while( PIR2bits.CCP2IF == 0 ){ }
    period2_low = CCPR2L;//         read captured value, low byte first
    period2_high = CCPR2H;//        then high byte
    //concatenate two 8-bit variables into one 16-bit variable
    period2 = ( uint16_t )( ( period2_high << 8 ) | period2_low );
    //the period of the signal is the difference between the second captured
    //value and the first captured value
    total = ( uint32_t )( period2 - period1 );
    //to know the period value in time, you must multiply the Ticks per the
    //time value of each Tick
    //Period in seconds = totalperiod * prescaler * instruction cycle
    //Period in seconds = totalperiod * prescaler * ( 4 * Tosc)
    //to make this more clear, lets calculate the frequency
    //Frequency in hertz = Fosc / ( totalperiod * prescaler * 4 )
    //since we know that Fosc is 16MHz     
    freq = ( uint32_t )( round( 16000000.0 / ( total * 8.0 * 4.0 )) );
    return freq;
}
void FREQ_Print( uint32_t freq ){
    uint8_t array[ 6 ];
    uint32_t tmp = 100000;
    for( int i = 0; i < 6; i++ ){
        array[ i ] = ( uint8_t )( freq / tmp ) + '0';
        freq = freq % tmp;
        tmp = tmp / 10;
    }
    UART_TransmitStr( "Frequency: ", sizeof( "Frequency: ") );
    UART_TransmitStr( array, sizeof( array ) );
    UART_TransmitStr( " Hz\r", sizeof( " Hz\r") );
}
//+++++++++++++++++++++++++++++++++++END++++++++++++++++++++++++++++++++++++++++
