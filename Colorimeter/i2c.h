/* 
 * File:   i2c.h
 * Author: rahu7p
 */

#include <xc.h>

#define _XTAL_FREQ 16000000

#define I2C_100KHZ 0x27//       for a Fosc = 16 MHz
#define I2C_308KHZ 0x0C//       for a Fosc = 16 MHz
#define I2C_400KHZ 0x09//       for a Fosc = 16 MHz

#define I2C_MASTER_MODE
//#define I2C_SLAVE_MODE

#ifdef I2C_MASTER_MODE
void I2C_Init_Master(uint8_t sp_i2c);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Restart(void);
void I2C_Ack(void);
void I2C_Nack(void);
uint8_t I2C_Read(void);
uint8_t I2C_Write(uint8_t data);
void I2C_MasterWrite( uint8_t *pData, uint8_t length, uint8_t address );
void I2C_MasterWriteRead( uint8_t *pData, uint8_t length, uint8_t address, uint8_t reg );
#endif

#ifdef I2C_SLAVE_MODE
void I2C_Init_Slave(unsigned char add_slave);
unsigned char I2C_Read_Slave(void);
void I2C_Write_Slave(char dato_i2c);
void I2C_Error_Data(void);
short I2C_Write_Mode(void);
short I2C_Read_Mode(void);
short I2C_Error_Read(void);
#endif
