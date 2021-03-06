/*
 * File:   main.c
 * Author: L01356840
 *
 * Created on 24 de junio de 2022, 12:13 PM
 */

//++++++++++++++++++++++++++++LIBRARIEs SECTION+++++++++++++++++++++++++++++++++
#include "main.h"
#include <xc.h>
#include <stdint.h>//               unified data types for all XC compilers
//++++++++++++++++++++++++++++DIRECTIVEs SECTION++++++++++++++++++++++++++++++++
//Defines
#define _XTAL_FREQ 16000000//       MACRO for __delay_ms() function
//Enumerations
typedef enum{//                     assign a constant value to a label
    oneSecond   = 1000              
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
    //CONFIGURATIONS************************************************************
    CLK_Initialize( );//            Clock initializations
    PORT_Initialize( );//           PORT initializations
    //INFINITE CYCLE************************************************************
    while( 1 ){  
        //IF A PUSH-BUTTON IS PRESSED, A LED IS TURNED ON***********************
        if( PORTBbits.RB4 == 0 ){// if button (S1) is pressed then:
            LATAbits.LATA7 = 1;//   turn ON LED (D5)
        } else {//                  otherwise:
            LATAbits.LATA7 = 0;//   turn OFF LED (D5)
        }//LED TURN-ON FOR 1 SECOND AND TURN-OFF FOR ANOTHER SECOND*************
        LATAbits.LATA4 = 1;//       turn ON LED (D2)
        __delay_ms( oneSecond );//  delay MACRO of the XC8 compiler
        LATAbits.LATA4 = 0;//       turn OFF LED (D2))
        __delay_ms( oneSecond );//  delay MACRO of the XC8 compiler
    }
}
//++++++++++++++++++++++++++FUNCTIONs SECTION+++++++++++++++++++++++++++++++++++
void CLK_Initialize( void ){
    OSCCONbits.IRCF = 0b111;//      set HFINTOSC to 16 MHz
    SLRCON = 0;//                   set a standard slew rate in pin PORTS 
}
void PORT_Initialize( void ){
    LATA = LATA & 0b01101111;//     clear RA4 and RA7 data latches
    TRISA = TRISA & 0b01101111;//   set RA4 and RA7 as output
    TRISBbits.TRISB4 = 1;//         set RB4 as input
    ANSELBbits.ANSB4 = 0;//         enable digital input buffer in RB4
}
//+++++++++++++++++++++++++++++++++++END++++++++++++++++++++++++++++++++++++++++
