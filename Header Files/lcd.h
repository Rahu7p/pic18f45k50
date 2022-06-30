/*
 * File:                lcd.h
 * Author:              rahu7p
 * Development board:   Curiosity HPC
 * MCU:                 PIC18f45k50
 * 
 */

#ifndef LCD_H
#define	LCD_H

//Lista de definicion de pines
#define LCD_DATA    PORTD
#define LCD_D_DIR   TRISD
#define LCD_RS      LATCbits.LATC2
#define LCD_RS_DIR  TRISCbits.TRISC2
#define LCD_RW      LATCbits.LATC1
#define LCD_RW_DIR  TRISCbits.TRISC1
#define LCD_E       LATCbits.LATC0
#define LCD_E_DIR   TRISCbits.TRISC0

//Definimos los nombres de los comandos para el LCD
#define LCD_Clear( )			LCD_cmd( 0x01U )//	Borra la pantalla
#define LCD_Display_ON( )		LCD_cmd( 0x0EU )//	Pantalla LCD activa
#define LCD_Display_OFF( )		LCD_cmd( 0x08U )//	Pantalla LCD inactiva
#define LCD_Cursor_Home( )		LCD_cmd( 0x02U )//	Establecer el cursor a 'Home'
#define LCD_Cursor_Blink( )		LCD_cmd( 0x0FU )//	Cursor intermitente
#define LCD_Cursor_ON( )		LCD_cmd( 0x0EU )//	Cursor visible activo
#define LCD_Cursor_OFF( )		LCD_cmd( 0x0CU )//	Cursor inactivo
#define LCD_Cursor_Left( )		LCD_cmd( 0x10U )//	Movimiento hacia la izquierda del cursor
#define LCD_Cursor_Right( )		LCD_cmd( 0x14U )//	Movimiento hacia la derecha del cursor
#define LCD_Cursor_SLeft( )		LCD_cmd( 0x18U )//	Movimiento hacia la izquierda de la pantalla
#define LCD_Cursor_SRight( )	LCD_cmd( 0x1CU )//	Movimiento hacia la derecha de la pantalla

//Lista de funciones
void LCD_rdy( void );       
void LCD_cmd( char cx );
void LCD_Initialize( void );
char get_DDRAM_addr( void );
void LCD_putch( char dx );
void LCD_putstr( char *ptr );
void send2LCD( char xy );

#endif	/* LCD_H */
