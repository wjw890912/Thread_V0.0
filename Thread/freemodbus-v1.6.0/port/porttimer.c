/*
 * FreeModbus Libary: STM32 Port
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
 * File: $Id: porttimer.c,v 1.60 2013/08/13 15:07:05 Armink $
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "NUC472_442.h"
/* ----------------------- static functions ---------------------------------*/
static void prvvTIMERExpiredISR(void);

static 	  USHORT  MBTimerout50us;
/* ----------------------- Start implementation -----------------------------*/
BOOL xMBPortTimersInit(USHORT usTim1Timerout50us)
{

	uint16_t PrescalerValue = 0;
	 MBTimerout50us= usTim1Timerout50us;
   CLK_EnableModuleClock(TMR0_MODULE);
    // Set timer frequency to 20KHZ	 (50us)
   CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0SEL_PCLK, 0);
   //Trmclk_prescale=pclk/20000=84000000/20000
   //TIMER_SET_PRESCALE_VALUE(TIMER0, 0);
   TIMER_Open(TIMER0, TIMER_PERIODIC_MODE,20000);//20KHZ

    // Enable timer interrupt
    TIMER_EnableInt(TIMER0);
	NVIC_SetPriority(TMR0_IRQn, 3 ); //��ʱ�����ж����ȼ�Ӧ�ñȴ��ڵĵ�
    NVIC_EnableIRQ(TMR0_IRQn);


    // Stop Timer 0
   TIMER_Stop(TIMER0);
	
	return TRUE;
}

unsigned char Timehandel=0;	//��ʱ����־
int timecut=0;//������

void vMBPortTimersEnable()
{
	 Timehandel=1;//��ʼ��ʱ
	 timecut=0;
	TIMER_Open(TIMER0, TIMER_PERIODIC_MODE,20000);//20
    TIMER_EnableInt(TIMER0);
	NVIC_SetPriority(TMR0_IRQn, 3 ); //��ʱ�����ж����ȼ�Ӧ�ñȴ��ڵĵ�
    NVIC_EnableIRQ(TMR0_IRQn);
	 TIMER_Start(TIMER0);
	 
}

void vMBPortTimersDisable()
{
	TIMER_Stop(TIMER0);
	TIMER_Close(TIMER0);
	Timehandel=0;//��ֹ��ʱ
	timecut=0;//��ռ���
}

void prvvTIMERExpiredISR(void)
{
	(void) pxMBPortCBTimerExpired();
}


void TMR0_IRQHandler(void)
{

  
	if(Timehandel)
	{
		 timecut++;
	   if(timecut>= MBTimerout50us)	//��ʱ�¼�����
	   {
	   	
	   	 	prvvTIMERExpiredISR();//�յ�һ֡RTU
			timecut=0;//�ٴ����
	   }
	   
	
	}
	else
	{
		 timecut=0;
	
	}
		

   TIMER_ClearIntFlag(TIMER0);
}

