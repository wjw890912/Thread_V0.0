

#include <stdio.h>
#include "NUC472_442.h"
#include "systimer.h"


 //初始化定时器用于MODBUS RTU超时时间
void Timer_Init(void)
{

   CLK_EnableModuleClock(TMR0_MODULE);
   CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0SEL_PCLK, 0);
    // Set timer frequency to 20KHZ	 (50us)
    TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, 20000);

    // Enable timer interrupt
    TIMER_EnableInt(TIMER0);
    NVIC_EnableIRQ(TMR0_IRQn);


    // Stop Timer 0
    TIMER_Stop(TIMER0);
	//TIMER_Start(TIMER0);


}

