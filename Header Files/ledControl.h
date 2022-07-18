/*
 * File:                ledControl.h
 * Author:              rahu7p
 * Development board:   Curiosity HPC
 * MCU:                 PIC18f45k50
 * 
 */

#ifndef LEDCONTROL_H
#define	LEDCONTROL_H

//------------------------------------------------------------------------------
#define DECODE_MODE_REG     0x09
#define INTESITY_REG        0x0A
#define SCAN_LIMIT_REG      0x0B
#define SHUTDOWN_REG        0x0C
#define DISPLAY_TEST_REG    0x0F
//------------------------------------------------------------------------------
#define DISABLE_DECODE      0x00
#define DECODE_ALL          0xFF
#define BRIGHTNESS          0x07
#define SCAN_LOW_NIBBLE     0x03
#define SCAN_ALL_DIGITS     0x07
#define SHUTDOWN_MODE       0x00
#define NORMAL_OPERATION    0x01
#define DISABLE_TEST_MODE   0x00
#define ENABLE_TEST_MODE    0x01
//-----------------------------------------------------------------------------

static void SPI1_Initialize( void );
static void SPI1_slave1Select( void );
static void SPI1_slave1Deselect( void );
static uint8_t SPI1_exchangeByte( uint8_t data );

//MAX7219 
void MAX7219_write( uint8_t regName, uint8_t data );
void MAX7219_clearDisplay( void );
void MAX7219_Initialize( void );

#endif	/* LEDCONTROL_H */
