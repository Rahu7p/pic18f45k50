/*
 * File:                main.c
 * Author:              rahu7p
 * Development board:   Curiosity HPC
 * MCU:                 PIC18f45k50
 * 
 * 
 */

//++++++++++++++++++++++++++++LIBRARIEs SECTION+++++++++++++++++++++++++++++++++
#include "main.h"
#include <stdint.h>//               unified data types for all XC compilers
#include "myprintf.h"
//++++++++++++++++++++++++++++DIRECTIVEs SECTION++++++++++++++++++++++++++++++++
//Defines
#define _XTAL_FREQ 16000000
//Enumerations
//...
//Variables, constants
uint8_t vin_str[5] = {0,'.',0,0,'\n'};
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
void UART_Initialize( void );
void ADC_Initialize( void );
void UART_TransmitByte( uint8_t data );
void UART_TransmitStr( uint8_t *pData, uint16_t size );
uint16_t ADC_analogRead( void );
void floatToStr( float v );
//+++++++++++++++++++++++++++++MAIN SECTION+++++++++++++++++++++++++++++++++++++
void main( void ){
    CLK_Initialize( );//                Clock initializations
    PORT_Initialize( );//               PORT initializations
    UART_Initialize( );//               UART initializations
    ADC_Initialize( );//                ADC initializations
    while( 1 ){
        
    }    
}
//++++++++++++++++++++++++++FUNCTIONs SECTION+++++++++++++++++++++++++++++++++++
void CLK_Initialize( void ){
    OSCCONbits.IRCF = 0b111;//          set HFINTOSC to 16 MHz
    SLRCON = 0;//                       set a standard slew rate in pin PORTS 
}
void PORT_Initialize( void ){
   //Pin configurations for the UART
    ANSELCbits.ANSC6 = 0;//         configure RC6 (TX) pin as:
    TRISCbits.TRISC6 = 1;//         EUSART asynchronous transmit data output
    ANSELCbits.ANSC7 = 0;//         configure RC7 (RX) pin as:
    TRISCbits.TRISC7 = 1;//         EUSART asynchronous receive data in
    //Pin configurations for the ADC
    ANSELAbits.ANSA0 = 1;//         configure RA0 (AN0) pin as analog
    TRISAbits.TRISA0 = 1;//         disable RA0 (AN0) pin output driver1
}
void UART_Initialize( void ){
    BAUDCON1bits.BRG16 = 1;//           16-bit baudrate
    //Configure a 9600 BAUDRATE according to TABLE 17-5
    SPBRGH1 = 0x01;       
    SPBRG1 = 0xA0;
    TXSTA1bits.BRGH = 1;//              high-speed
    TXSTA1bits.SYNC = 0;//              asynchronous mode
    RCSTA1bits.SPEN = 1;//              serial port enabled
    TXSTA1bits.TXEN = 1;//              transmit enabled
    RCSTA1bits.CREN = 1;//              enables receiver 
}
void ADC_Initialize( void ){
    //                                  initialize ADC with:
    ADCON2 = 0b00101010;//              left justify, Fosc/32, 12 TAD ACQ time
    ADCON1 = 0b00000000;//              ADC ref = Vdd, Vss
    ADCON0 = 0b00000000;//              AN0 channel
    ADCON0bits.ADON = 1;//              ADC ON
}
void UART_TransmitByte( uint8_t data ){
    while( TXSTA1bits.TRMT == 0 );//    wait until TSR is empty
    TXREG = data;//                     transmit byte
}
void UART_TransmitStr( uint8_t *pData, uint16_t size ){
    for( int i = 0; i < size; i++ ){
        UART_TransmitByte( *pData++ );
    }
}
uint16_t ADC_analogRead( void ){
    ADCON0bits.GO = 1;//                start conversion
    while( ADCON0bits.DONE == 1 );//    wait until conversion is done
    return (uint16_t)( ADRESH << 2 | ADRESL >> 6 );
}
void floatToStr( float v ){
    uint8_t tmp;
    uint16_t vin_int;
    vin_int = (uint16_t)( v * 100 );
    vin_str[0] = (uint8_t)( ( vin_int / 100 ) + '0' );
    tmp = vin_int % 100;
    vin_str[2] = ( tmp / 10 ) + '0';
    vin_str[3] = ( tmp % 10 ) + '0';
}
//+++++++++++++++++++++++++++++++++++END++++++++++++++++++++++++++++++++++++++++
