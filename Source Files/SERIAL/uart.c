#include <xc.h>
#include "uart.h"

void UART_Initialize( void ){
    //STEP 1. Pin configurations for the UART
    ANSELCbits.ANSC6 = 0;//         configure RC6 (TX) pin as:
    TRISCbits.TRISC6 = 1;//         EUSART asynchronous transmit data output
    ANSELCbits.ANSC7 = 0;//         configure RC7 (RX) pin as:
    TRISCbits.TRISC7 = 1;//         EUSART asynchronous receive data in
    //STEP 2. Configure a 9600 BAUDRATE according to TABLE 17-5
    BAUDCON1bits.BRG16 = 1;//       16-bit baudrate
    TXSTA1bits.BRGH = 1;//          high-speed
    SPBRGH1 = 0x01;       
    SPBRG1 = 0xA0;
    //STEP 3. UART initializations
    TXSTA1bits.SYNC = 0;//          asynchronous mode
    RCSTA1bits.SPEN = 1;//          serial port enabled
    TXSTA1bits.TXEN = 1;//          transmit enabled
    RCSTA1bits.CREN = 1;//          enables receiver 
}

void UART_TransmitByte( uint8_t data ){
    while( TXSTA1bits.TRMT == 0 );//    wait until TSR is empty
    TXREG1 = data;//                     transmit byte
}
