/*
 * File:                ledControl.c
 * Author:              rahu7p
 * Development board:   Curiosity HPC
 * MCU:                 PIC18f45k50
 * 
 */

#include <xc.h>
#include <pic18f45k50.h>
#include "ledControl.h"

static void SPI1_Initialize( void ){
    SSP1CON1bits.SSPM = 0b0000;//   SPI Master Mode, clock Fosc/4
    SSP1STATbits.CKE = 1;//         SPI Mode 0
    SSP1CON1bits.CKP = 0;//         SPI Mode 0
    SSP1CON1bits.SSPEN = 1;//       enables SPI pins
}

//COMPLETE this function!!
static void SPI1_slave1Select( void ){
                          //        set CS pin to LOW
}

//COMPLETE this function!!
static void SPI1_slave1Deselect( void ){
                          //        set CS pin to HIGH
}

static uint8_t SPI1_exchangeByte( uint8_t data ){
    SSP1BUF = data;
    while( !PIR1bits.SSPIF );//     wait until data is exchanged
    PIR1bits.SSPIF = 0;
    return SSP1BUF;
}

// Send a simple command to the MAX7219 using the hardware SPI interface
void MAX7219_write( uint8_t regName, uint8_t data ){
    uint8_t receiveData;
    SPI1_slave1Select( );
    receiveData = SPI1_exchangeByte( regName );
    receiveData = SPI1_exchangeByte( data );
    SPI1_slave1Deselect( );
}

void MAX7219_clearDisplay( void ){
    for( uint8_t i = 1; i <= 8; i++ ){
        for( uint8_t j = 0; j < 8; j++ ){
            MAX7219_write( i, 0 );
        }           
    }
}

void MAX7219_Initialize( void ){
    SPI1_Initialize( );
    MAX7219_write( DECODE_MODE_REG, DISABLE_DECODE );    
    MAX7219_write( INTESITY_REG, BRIGHTNESS );     
    MAX7219_write( SCAN_LIMIT_REG, SCAN_ALL_DIGITS );     
    MAX7219_write( SHUTDOWN_REG, NORMAL_OPERATION );
    MAX7219_write( DISPLAY_TEST_REG, DISABLE_TEST_MODE );  
    MAX7219_clearDisplay( );
}
