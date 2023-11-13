/*
 * File:                main.c
 * Author:              rahu7p
 * Development board:   Curiosity HPC
 * MCU:                 PIC18f45k50
 * 
 * This program uses a PWM signal to dim the bright of a LED (D2) each 1 second. 
 * The CCP2 module is configured in PWM mode. The Timer 0 is configured for the 
 * 1 second delay 
 * The PWM signal has the following features: 
 *  - Frequency: 4 kHz 
 *  - Duty cycle: 25% steps from 0% to 100%
 * A jumper (connection) is needed between pin RC1 (CCP2) to pin RA4 (LED D2).
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
void CCP_PWM_Initialize( void );
void TMR2_Initialize( void );
void TMR0_Delay_1s( void );
void dutyCycle_Update( float duty );
//+++++++++++++++++++++++++++++MAIN SECTION+++++++++++++++++++++++++++++++++++++
void main( void ){
    float duty_cycle = 0.0;
    CLK_Initialize( );//                Clock initializations
    PORT_Initialize( );//               PORT initializations
    CCP_PWM_Initialize( );//            CCP initializations as PWM
    TMR2_Initialize( );//               Timer 2 initializations
    while( 1 ){
        /* STEP 3 Begins */
        dutyCycle_Update( duty );//     update the duty cycle
        TMR0_Delay_1s( );//             1 second delay
        //Incrementing the duty in (25% steps)
        duty_cycle = duty_cycle + 0.25;
	    if( duty_cycle == 1.25 )
		    duty_cycle = 0.0;
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
    //CCP2 output is multiplexed with RC1, this is configured in "main.h" file
    TRISCbits.TRISC1 = 1;//             disable RC1 as CCP2 PWM output
    /* STEP 1 Ends */
}
void CCP_PWM_Initialize( void ){
    /* STEP 2 Begins */
    PR2 = 249;//                        PWM = 4 kHz     
    CCP2CON = 0b00001100;//             CCP2 module: PWM mode
    CCPR2L = 0;//                       initialize duty cycle to 0%  
}
void TMR2_Initialize( void ){
    T2CON = 0b00000001;//               configure the TMR2 prescaler 1:4
    TMR2 = 0;//                         clear TMR2 count   
    PIR1bits.TMR2IF = 0;//              clear TMR2 interrupt flag
    T2CONbits.TMR2ON = 1;//             Timer 2 enabled
    //in order to send a complete duty cycle and period on the 1st PWM output
    while( !PIR1bits.TMR2IF );//        wait until TMR2 overflows++
    TRISCbits.TRISC1 = 0;//             enable the CCP2 pin output
    /* STEP 2 Ends */
}
void dutyCycle_Update( float duty ){
    uint16_t dutyTemp = duty * 4 * (PR2 + 1);// calculating the 10-bit duty  
    CCPR2L = (uint8_t)( dutyTemp >> 2 );//      load (bit10 to bit2) of the 10-bit duty 
    CCP2CONbits.DC2B = dutyTemp & 0x0003;//     load (bit1 and bit0) of the 10-bit duty
}
void TMR0_Delay_1s( void ){
    T0CON = 0b00000101;//               initialize Timer 0 with:
    //                                  mode=16bit,internal clock,1:64 presc
    TMR0H = 0x0B;//                     load initial count, high-byte first  
    TMR0L = 0xDC;//                     then low-byte      
    INTCONbits.TMR0IF = 0;//            clear the Timer 0 overflow flag
    T0CONbits.TMR0ON = 1;//             enables Timer 0
    while( !INTCONbits.TMR0IF );//      wait for Timer 0 overflow
    T0CONbits.TMR0ON = 0;//             stop Timer 0
}
//+++++++++++++++++++++++++++++++++++END++++++++++++++++++++++++++++++++++++++++
