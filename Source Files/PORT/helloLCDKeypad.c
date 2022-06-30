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
#include "lcd.h"
//++++++++++++++++++++++++++++DIRECTIVEs SECTION++++++++++++++++++++++++++++++++
//Defines
#define _XTAL_FREQ 16000000//       MACRO for __delay_ms() function
//Enumerations
typedef enum{//                     assign a constant value to a label
    limSLeft    = 10,
    twentyMS    = 20,
    twoHundMS   = 200,
    posR0C10    = 0x8A,
    posR1C0     = 0xC0,
    notKey      = 0xFF            
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
uint8_t getKey( void );
void fancyString( void );
//+++++++++++++++++++++++++++++MAIN SECTION+++++++++++++++++++++++++++++++++++++
void main( void ){  
    uint8_t key;
    CLK_Initialize( );//                    Clock initializations
    PORT_Initialize( );//                   PORT initializations
    LCD_Initialize( );//                    LCD initializations
    fancyString( );
    LCD_cmd( posR1C0 );//                   cursor position in Row1, Col0
    LCD_Cursor_ON( );//                     cursor is visible
    while( 1 ){
        key = getKey();//                   call the function to get the key
        if( key != notKey ){//              if a Key was returned
            key = key + '0';//              get the ASCII value of the key
            LCD_putch( key );//             write the key character in the LCD        
            while( PORTBbits.RB0 == 0 );//  wait until Key is released
        }
    }
}
//++++++++++++++++++++++++++FUNCTIONs SECTION+++++++++++++++++++++++++++++++++++
void CLK_Initialize( void ){
    OSCCONbits.IRCF = 0b111;//      set HFINTOSC to 16 MHz
    SLRCON = 0;//                   set a standard slew rate in pin PORTS 
}
void PORT_Initialize( void ){  
    //Pin configurations for the LCD
    ANSELD = 0;//                   enable digital buffer in data port
    ANSELCbits.ANSC2 = 0;//         enable digital buffer in RC2
    //Pin configurations for the Keypad
    LATB = 0;//                     clear PORTB data latches
    TRISB = 0;//                    set PORTB as output
    ANSELB = ANSELB & 0b11111110;// enable digital input buffer in RB0
    TRISB = TRISB | 0b00000001;//   set RB0 as input    
    WPUB = WPUB | 0b00000001;//     connect an internal resistor in pin RB0
    INTCON2bits.RBPU = 0;//         enable the internal pull-ups in PORTB
}
uint8_t getKey( void ){
    LATB = LATB | 0b11110000;//     disable with '1' the columns of the keypad 
    LATBbits.LATB4 = 0;//           enable with '0' the C0
    if( PORTBbits.RB0 == 0 ){//     if we find the '0' in R0 
        __delay_ms( twentyMS );//   call a delay to debounce the input
        if( PORTBbits.RB0 == 1 ){// double checking
            return notKey;//        if button was not pressed, no match
        }
        //if button was really pressed
        return 1;//                 assign a match (R0-C0) value of '1'
    }  
    /* Continue with the remain rows.
     * Copy three times the segment of code of the first IF statement.
     * Change the ROW (pins) checkings and the match values.
     */
    
    /* Continue with the remain columns.
     * Copy three times the code from the first line of the function,
     * including the code (IF statements) of the remain rows.
     * Change the COLUMN (pins) writes and the match values.
     */
    
    return notKey;
}
void fancyString( void ){
    LCD_cmd( posR0C10 );//      cursor position in row0, col10
    LCD_putstr( "TE2015" );//   write the string in LCD
    LCD_Cursor_OFF( );//        cursor not visible
    //for-loop to left-shift the string every 200ms by modifying DDRAM positions  
    for( int i = 0; i < limSLeft; i++ ){
        __delay_ms( twoHundMS );
        LCD_Cursor_SLeft( );
    }
    LCD_Cursor_Home( );//       set the cursor to Home to reset DDRAM
    LCD_Clear( );//             clear the string
    LCD_putstr( "TE2015" );//   rewrite the string in row0, col0
}
//+++++++++++++++++++++++++++++++++++END++++++++++++++++++++++++++++++++++++++++
