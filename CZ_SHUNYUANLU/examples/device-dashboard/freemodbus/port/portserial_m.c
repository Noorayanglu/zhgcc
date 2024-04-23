/*
 * FreeModbus Libary: RT-Thread Port
 * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
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
 * File: $Id: portserial_m.c,v 1.60 2013/08/13 15:07:05 Armink add Master Functions $
 */


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include "port.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "mbconfig.h"
#include "mbctx.h"


#if MB_MASTER_RTU_ENABLED > 0 || MB_MASTER_ASCII_ENABLED > 0
/* ----------------------- Static variables ---------------------------------*/
/* ----------------------- static functions ---------------------------------*/
void prvvMBMasterUARTTxReadyISR(void);
void prvvMBMasterUARTRxISR(void);


/* ----------------------- Start implementation -----------------------------*/
BOOL xMBMasterPortSerialInit(fmodbus_t* ctx)
{
    CHAR            szDevice[32];
    BOOL            bStatus = TRUE;

    struct termios  xNewTIO;
    speed_t         xNewSpeed;

    snprintf( szDevice, 32, "%s", ctx->szPort );

    if( ( ctx->iSerialFd = open( szDevice, O_RDWR | O_NOCTTY ) ) < 0 ){
        vMBPortLog( MB_LOG_ERROR, "SER-INIT", "Can't open serial port %s: %s\n", szDevice, strerror( errno ) );
    }else if( tcgetattr( ctx->iSerialFd, &(ctx->xOldTIO) ) != 0 ){
        vMBPortLog( MB_LOG_ERROR, "SER-INIT", "Can't get settings from port %s: %s\n", szDevice, strerror( errno ) );
    }else{
        vMBPortLog( MB_LOG_INFO, "SER-INIT", "init ok %s\n", szDevice );

        bzero( &xNewTIO, sizeof( struct termios ) );

        xNewTIO.c_iflag |= IGNBRK | INPCK;
        xNewTIO.c_cflag |= CREAD | CLOCAL;
        switch ( ctx->eParity )
        {
        case MB_PAR_NONE:
            break;
        case MB_PAR_EVEN:
            xNewTIO.c_cflag |= PARENB;
            break;
        case MB_PAR_ODD:
            xNewTIO.c_cflag |= PARENB | PARODD;
            break;
        default:
            bStatus = FALSE;
        }
        switch ( ctx->ucDataBits )
        {
        case 8:
            xNewTIO.c_cflag |= CS8;
            break;
        case 7:
            xNewTIO.c_cflag |= CS7;
            break;
        default:
            bStatus = FALSE;
        }
        switch ( ctx->ulBaudRate )
        {
        case 4800:
            xNewSpeed = B4800;
            break;

        case 9600:
            xNewSpeed = B9600;
            break;
        case 19200:
            xNewSpeed = B19200;
            break;
        case 38400:
            xNewSpeed = B38400;
            break;
        case 57600:
            xNewSpeed = B57600;
            break;
        case 115200:
            xNewSpeed = B115200;
            break;
        default:
            bStatus = FALSE;
        }
        if( bStatus )
        {
            if( cfsetispeed( &xNewTIO, xNewSpeed ) != 0 )
            {
                vMBPortLog( MB_LOG_ERROR, "SER-INIT", "Can't set baud rate %ld for port %s: %s\n",
                            ctx->ulBaudRate, strerror( errno ) );
            }
            else if( cfsetospeed( &xNewTIO, xNewSpeed ) != 0 )
            {
                vMBPortLog( MB_LOG_ERROR, "SER-INIT", "Can't set baud rate %ld for port %s: %s\n",
                            ctx->ulBaudRate, szDevice, strerror( errno ) );
            }
            else if( tcsetattr( ctx->iSerialFd, TCSANOW, &xNewTIO ) != 0 )
            {
                vMBPortLog( MB_LOG_ERROR, "SER-INIT", "Can't set settings for port %s: %s\n",
                            szDevice, strerror( errno ) );
            }
            else
            {
                vMBPortSerialEnable(ctx, FALSE, FALSE );
                bStatus = TRUE;
            }
        }
    }
    return bStatus;
}

void vMBMasterPortSerialEnable(fmodbus_t* ctx, BOOL bEnableRx, BOOL bEnableTx )
{
    /* it is not allowed that both receiver and transmitter are enabled. */
    assert( !bEnableRx || !bEnableTx );

    if( bEnableRx )
    {
        ( void )tcflush( ctx->iSerialFd, TCIFLUSH );
        ctx->uiRxBufferPos = 0;
        ctx->bRxEnabled = TRUE;
    }
    else
    {
        ctx->bRxEnabled = FALSE;
    }
    if( bEnableTx )
    {
        ctx->bTxEnabled = TRUE;
        ctx->uiTxBufferPos = 0;
    }
    else
    {
        ctx->bTxEnabled = FALSE;
    }
}

void vMBMasterPortClose( fmodbus_t* ctx )
{
    if( ctx->iSerialFd != -1 )
    {
        ( void )tcsetattr( ctx->iSerialFd, TCSANOW, &ctx->xOldTIO );
        ( void )close( ctx->iSerialFd );
        ctx->iSerialFd = -1;
    }
}

BOOL xMBMasterPortSerialPutByte(fmodbus_t* ctx, CHAR ucByte )
{
    assert( ctx->uiTxBufferPos < BUF_SIZE );
    ctx->ucBuffer[ctx->uiTxBufferPos] = ucByte;
    ctx->uiTxBufferPos++;
    return TRUE;
}

BOOL xMBMasterPortSerialGetByte(fmodbus_t* ctx, CHAR * pucByte )
{
    assert( ctx->uiRxBufferPos < BUF_SIZE );
    *pucByte = ctx->ucBuffer[ctx->uiRxBufferPos];
    ctx->uiRxBufferPos++;
    return TRUE;
}

/* 
 * Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
void prvvMBMasterUARTTxReadyISR(void)
{
    pxMBMasterFrameCBTransmitterEmpty();
}

/* 
 * Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
void prvvMBMasterUARTRxISR(void)
{
    pxMBMasterFrameCBByteReceived();
}




#endif
