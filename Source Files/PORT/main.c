//++++++++++++++++++++++++++++LIBRARIEs SECTION+++++++++++++++++++++++++++++++++
#include "main.h"
#include <xc.h>
#include <stdint.h>//            unified data types for all XC compilers
//++++++++++++++++++++++++++++DIRECTIVEs SECTION++++++++++++++++++++++++++++++++
//Defines
#define _XTAL_FREQ 16000000//    MACRO for __delay_ms() function
#define ONE_SECOND 1000      
//Global variables, constants
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
  CLK_Initialize( );//                     Clock initializations
  PORT_Initialize( );//                    PORT initializations
  while( 1 ){  
    LATAbits.LATA4 = LATAbits.LATA4 ^ 1;// toggle the RA4 pin (LED)
    __delay_ms(ONE_SECOND);//              delay MACRO of the XC8 compiler
  }
}
//++++++++++++++++++++++++++FUNCTIONs SECTION+++++++++++++++++++++++++++++++++++
void CLK_Initialize( void ){
  OSCCONbits.IRCF = 0b111;//   set HFINTOSC to 16 MHz
  SLRCON = 0;//                set a standard slew rate in pin PORTS 
}
void PORT_Initialize( void ){
  LATAbits.LATA4 = 0;//        clear the RA4 output register
  TRISAbits.TRISA4 = 0;//      set RA4 as output
}
//+++++++++++++++++++++++++++++++++++END++++++++++++++++++++++++++++++++++++++++
