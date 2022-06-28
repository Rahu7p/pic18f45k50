/*
 * File:                main.c
 * Author:              rahu7p
 * Development board:   Curiosity HPC
 * MCU:                 PIC18F45K50
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
}osTime;
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
        __delay_ms( oneSecond );//  delay function XC8 compiler
        LATAbits.LATA4 = 0;//       turn OFF LED (D2))
        __delay_ms( oneSecond );//  delay function XC8 compiler
    }
}
//++++++++++++++++++++++++++FUNCTIONs SECTION+++++++++++++++++++++++++++++++++++
void CLK_Initialize( void ){
    OSCCONbits.IRCF = 0b111;//      set HFINTOSC to 16 MHz
}
void PORT_Initialize( void ){  
    TRISAbits.TRISA4 = 0;//         set RA4 as output
    TRISAbits.TRISA7 = 0;//         set RA7 as output
    TRISBbits.TRISB4 = 1;//         set RB4 as input
    ANSELBbits.ANSB4 = 0;//         enable digital input buffer in RB4
}
//+++++++++++++++++++++++++++++++++++END++++++++++++++++++++++++++++++++++++++++
