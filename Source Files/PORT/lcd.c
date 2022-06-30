/*
 * File:                lcd.c
 * Author:              rahu7p
 * Development board:   Curiosity HPC
 * MCU:                 PIC18f45k50
 * 
 */

#include <xc.h>
#include "lcd.h"

/* The following function waits until the LCD is not busy */
void LCD_rdy( void ) {
    char test;
    LCD_D_DIR   = 0xFF;     //configure LCD data bus for input
    test        = 0x80;
    while( test ) {
        LCD_RS  = 0;        //select IR register (RS)
        LCD_RW  = 1;        //set read mode
        LCD_E   = 1;        //setup to clock data (enable)
        test    = LCD_DATA; 
        Nop();
        LCD_E   = 0;        //complete a read cycle (enable)
        test   &= 0x80;     //check bit 7 (flag busy)
    }
    LCD_D_DIR   = 0;        //configure LCD data bus for output
}

/* The following function sends a command to the LCD */
void LCD_cmd( char cx ) {
    LCD_rdy();              //wait until LCD is ready
    LCD_RS      = 0;        //select IR register
    LCD_RW      = 0;        //set write mode
    LCD_E       = 1;        //setup to clock data
    Nop();
    LCD_DATA    = cx;       //send out the command
    Nop();                  //small delay to lengthen E pulse
    LCD_E       = 0;        //complete an external write cycle
}

/* The following function initializes the LCD properly*/
void LCD_Initialize( void ) {
    PORTC       = 0;        //make sure LCD control port is low
    LCD_E_DIR   = 0;        //config LCD enable pin for output
    LCD_RS_DIR  = 0;        //config LCD RS pin for output
    LCD_RW_DIR  = 0;        //config LCD R/W pin for output
    LCD_cmd( 0x3C );        //configure display to 2x40
    LCD_cmd( 0x0F );        //turn on display, cursor and blinking
    LCD_cmd( 0x14 );        //shift cursor right
    LCD_cmd( 0x01 );        //clear display and move cursor to home
}

/* The following function obtains the LCD cursor address*/
char get_DDRAM_addr( void ) {
    char temp;
    LCD_D_DIR   = 0xFF;             //configure LCD data port for input
    LCD_RS      = 0;                //select IR register
    LCD_RW      = 1;                //setup to read busy flag
    LCD_E       = 1;                //pull LCD E-line to high
    temp        = LCD_DATA & 0x7F;  //read DDRAM address
    Nop();
    LCD_E       = 0;                //pull LCD E-line to low
    return temp;
}

/* The following function sends a character to the LCD. The character cannot be
   displayed at the end of a row, so we need to reoutput it to the first 
   column of the next row*/
void LCD_putch( char dx ) {
    char addr;
    LCD_rdy( );                     //wait until LCD internal operation is complete
    send2LCD( dx );
    LCD_rdy( );                     //wait until LCD internal operation is complete
    addr = get_DDRAM_addr( );
    if( addr == 0x13 ) {
        LCD_cmd( 0xC0 );
        LCD_rdy( );
        send2LCD( dx );             //output it to the column 1 of the next row
    } else if( addr == 0x53 ) {
        LCD_cmd( 0x94 );
        LCD_rdy( );
        send2LCD( dx );             //output it to the column 1 of the next row
    } else if( addr == 0x27 ) {
        LCD_cmd( 0xD4 );
        LCD_rdy( );
        send2LCD( dx );             //output it to the column 1 of the next row
    }
}

/* The following function outputs a string to the LCD*/
void LCD_putstr( char *ptr ) {
    while( *ptr ) {
        LCD_putch( *ptr );
        ptr++;
    }
}

/* The following function outputs a string to the LCD*/
void send2LCD( char xy ) {
    LCD_RS      = 1;          
    LCD_RW      = 0;          
    LCD_E       = 1;
    LCD_DATA    = xy;
    Nop();
    Nop();
    LCD_E       = 0;
}
