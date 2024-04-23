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
 * File: $Id: porttimer_m.c,v 1.60 2013/08/13 15:07:05 Armink add Master Functions$
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mb_m.h"
#include "mbport.h"
#include "mbconfig.h"

#if MB_MASTER_RTU_ENABLED > 0 || MB_MASTER_ASCII_ENABLED > 0
/* ----------------------- Variables ----------------------------------------*/
static USHORT usT35TimeOut50us;



/* ----------------------- static functions ---------------------------------*/


/* ----------------------- Start implementation -----------------------------*/
BOOL xMBMasterPortTimersInit(USHORT usTimeOut50us)
{
    /* backup T35 ticks */
    usT35TimeOut50us = usTimeOut50us * 500;
    //MRT_Init(); // this is called outside

    return TRUE;
}

void vMBMasterPortTimersT35Enable()
{
    MRT1_Start(usT35TimeOut50us);
}

void vMBMasterPortTimersConvertDelayEnable()
{
    unsigned int timer_tick = MB_MASTER_DELAY_MS_CONVERT * 1000;

    /* Set current timer mode, don't change it.*/
    vMBMasterSetCurTimerMode(MB_TMODE_CONVERT_DELAY);

    //rt_timer_control(&timer, RT_TIMER_CTRL_SET_TIME, &timer_tick);

    //rt_timer_start(&timer);
    MRT1_Start(timer_tick);
}

void vMBMasterPortTimersRespondTimeoutEnable()
{
    unsigned int timer_tick = MB_MASTER_DELAY_MS_CONVERT * 1000;

    /* Set current timer mode, don't change it.*/
    vMBMasterSetCurTimerMode(MB_TMODE_RESPOND_TIMEOUT);

    MRT1_Start(timer_tick);
}

void vMBMasterPortTimersDisable()
{
    MRT1_Stop();
}

void prvvMBMasterTIMERExpiredISR(void)
{
    (void) pxMBMasterPortCBTimerExpired();
}


#endif
