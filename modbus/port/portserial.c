/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "usart.h"

/* ----------------------- static functions ---------------------------------*/
void prvvUARTTxReadyISR( void );
void prvvUARTRxISR( void );

/* ----------------------- Start implementation -----------------------------*/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    /* If xRXEnable enable serial receive interrupts. If xTxENable enable
     * transmitter empty interrupts.
     */

//	USART_ITConfig(Modbus_Port, USART_IT_RXNE, DISABLE);
//	USART_ITConfig(Modbus_Port, USART_IT_TXE, DISABLE);

	if(TRUE == xRxEnable)
	{
		RS_485_RX_EN();
		USART_ITConfig(Modbus_Port, USART_IT_RXNE, ENABLE);
	}
	else
	{
		USART_ITConfig(Modbus_Port, USART_IT_RXNE, DISABLE);
	}

	if(TRUE == xTxEnable)
	{
		RS_485_TX_EN();
		USART_ITConfig(Modbus_Port, USART_IT_TXE, ENABLE);
	}
	else
	{
		USART_ITConfig(Modbus_Port, USART_IT_TXE, DISABLE);
	}

//	if(Modbus_Port==USART1)
//	{
//		if(TRUE == xTxEnable)
//		else
//	}


}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
	USART_Configuration(Modbus_Port,ulBaudRate);
    return TRUE;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
    /* Put a byte in the UARTs transmit buffer. This function is called
     * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
     * called. */

//	if(Modbus_Port==USART1)
//	{
//		RS_485_TX_EN();
//	}

	USART_SendData(Modbus_Port,ucByte);
	//while(USART_GetFlagStatus(Modbus_Port,USART_FLAG_TC==RESET));

//	if(Modbus_Port==USART1)
//	{
//		RS_485_RX_EN();
//	}

    return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
    /* Return the byte in the UARTs receive buffer. This function is called
     * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
     */

//	if(Modbus_Port==USART1)
//	{
//		RS_485_RX_EN();
//	}

	*pucByte=USART_ReceiveData(Modbus_Port);
    return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
void prvvUARTTxReadyISR( void )
{
    pxMBFrameCBTransmitterEmpty(  );
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
void prvvUARTRxISR( void )
{
    pxMBFrameCBByteReceived(  );
}
