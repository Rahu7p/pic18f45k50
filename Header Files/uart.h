#ifndef UART_H
#define UART_H

#include <stdint.h>

void UART_Initialize( void );
void UART_TransmitByte( uint8_t data );

#endif
