/* 
 * File:   i2c.c
 * Author: rahu7p
 */

#include "i2c.h"

#ifdef I2C_MASTER_MODE
void I2C_Init_Master( uint8_t sp_i2c )
{  
    //MSSP status register
    if( sp_i2c == I2C_100KHZ )
        SSP1STAT = 0x80;//  Slew rate control disabled for standard speed
    else
        SSP1STAT = 0x00;//  Slew rate control enabled for high speed
	  //MSSP control register 1
    SSP1CON1 = 0x28;//      Enables SDA/SCL pins and I2C master mode
    SSP1CON2 = 0x00;//      MSSP control register 2
    SSP1ADD = sp_i2c;//     MSSP baud rate register  
}

void I2C_Start( void )
{
    SSP1CON2bits.SEN = 1;//         initiates start condition        
    while(PIR1bits.SSPIF == 0)//    wait until master sync serial port flag
        ;
    PIR1bits.SSPIF = 0;//           cleared by software
}

void I2C_Stop( void )
{
    SSP1CON2bits.PEN = 1;//         initiates stop condition
    while(PIR1bits.SSPIF == 0)//    wait until master sync serial port flag
        ;
    PIR1bits.SSPIF = 0;//           cleared by software
}

void I2C_Restart( void )
{
    SSP1CON2bits.RSEN = 1;//        initiates repeated start condition
    while(PIR1bits.SSPIF == 0)//    wait until master sync serial port flag
        ;
    PIR1bits.SSPIF = 0;//           cleared by software
}

void I2C_Ack( void )
{
    SSP1CON2bits.ACKDT = 0;//       select acknowledge value
	  SSP1CON2bits.ACKEN = 1;//       initiate acknowledge sequence 
	  while(PIR1bits.SSPIF == 0)//    wait until master sync serial port flag
        ;
    PIR1bits.SSPIF = 0;//           cleared by software
}

void I2C_Nack( void )
{
    SSP1CON2bits.ACKDT = 1;//       select not acknowledge value
	  SSP1CON2bits.ACKEN = 1;//       initiate acknowledge sequence
	  while(PIR1bits.SSPIF == 0)//    wait until master sync serial port flag
        ;
    PIR1bits.SSPIF = 0;//           cleared by software
}

uint8_t I2C_Write( uint8_t data )
{
    //MSSP receive buffer/transmit register
    SSP1BUF = data;//               transmit data                   
    while(PIR1bits.SSPIF == 0)//    wait until master sync serial port flag
        ;
    PIR1bits.SSPIF = 0;//           cleared by software
    return SSP1CON2bits.ACKSTAT;//  return acknowledge status
}

uint8_t I2C_Read( void )
{
    SSP1CON2bits.RCEN = 1;//        enables receiver mode
    while(PIR1bits.SSPIF == 0)//    wait until master sync serial port flag
        ;
    PIR1bits.SSPIF = 0;//           cleared by software
    return SSP1BUF;//               return received data
}

void I2C_MasterWrite( uint8_t *pData, uint8_t length, uint8_t address ){
    I2C_Start();
    I2C_Write(address);
    for( uint8_t i = 0; i < length; i++ )
        I2C_Write( *pData++ );
    I2C_Stop(); 
}

void I2C_MasterWriteRead( uint8_t *pData, uint8_t length, uint8_t address, uint8_t reg ){
    I2C_Start();
    I2C_Write(address);
    I2C_Write(reg);   
    I2C_Restart();
    I2C_Write(address | 1);
    for( uint8_t i = 0; i < (length - 1); i++ ){
        *pData++ = I2C_Read();
        I2C_Ack();
    }
    *pData = I2C_Read();
    I2C_Nack();
    I2C_Stop();   
}
#endif

#ifdef I2C_SLAVE_MODE
void I2C_Init_Slave(unsigned char add_slave)
{
    
}

short I2C_Error_Read(void)
{
    
}

short I2C_Write_Mode(void)
{
    
}
short I2C_Read_Mode(void)
{
    
}

void I2C_Error_Data(void)
{
    
}

unsigned char I2C_Read_Slave(void)
{

}

void I2C_Write_Slave(char dato_i2c)
{

}
#endif
