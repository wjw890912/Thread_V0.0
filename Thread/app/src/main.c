#include <rthw.h>
#include <rtthread.h>    
#include <stdio.h>
#include "NUC472_442.h"
#include "gpio.h"
#include "ade7878_Int.h" 
#include"Usart.h"
#include "ADE7878_Measure.h"
#include"Config.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

	
#define SYSTEMTICK_PERIOD_MS  (RT_TICK_PER_SECOND/1000)
uint32_t LocalTime =0;
uint32_t Mbtick	   =0;
void SetSysTick(uint32_t us)
{
    SysTick->LOAD = us * CyclesPerUs;
    SysTick->VAL  =  (0x00);
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
  	NVIC_SetPriority(SysTick_IRQn,1);

}

void SYS_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable External XTAL (4~24 MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);

    /* Waiting for 12MHz clock ready */
    CLK_WaitClockReady( CLK_STATUS_HXTSTB_Msk);

    /* Switch HCLK clock source to HXT */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HXT,CLK_CLKDIV0_HCLK(1));

    /* Set PLL to power down mode and PLL_STB bit in CLKSTATUS register will be cleared by hardware.*/
    CLK->PLLCTL |= CLK_PLLCTL_PD_Msk;

    /* Set PLL frequency */
    CLK->PLLCTL = CLK_PLLCTL_84MHz_HXT;

    /* Waiting for clock ready */
    CLK_WaitClockReady(CLK_STATUS_PLLSTB_Msk);

    /* Switch HCLK clock source to PLL */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_PLL,CLK_CLKDIV0_HCLK(1));
  
    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock. */
    SystemCoreClockUpdate();
	 	
	SetSysTick(SYSTEMTICK_PERIOD_MS*1000);		// Set SysTick as 1ms
    /* Lock protected registers */
   // SYS_LockReg();
}

/**
 * This is the systick timer interrupt service routine.
 *
 */
void SysTick_Handler(void)
{	
	LocalTime += SYSTEMTICK_PERIOD_MS;	
  #ifdef THREAD_MODE
	/* enter interrupt */
	rt_interrupt_enter();

	rt_tick_increase();

	/* leave interrupt */
	rt_interrupt_leave();
  #else
	(void)eMBPoll();
  #endif


}
//delaytime=time *100ms(sysytick)
void Sysdelay(uint32_t time)
{
	  uint32_t tick;
	   

	for(time=time;time;time--)
	{
		  for(tick=10000;tick;tick--) ;

	}

}
extern USHORT   usSRegHoldBuf[100];
extern void PowerMeasurment(void);
extern void TestEEPROM_Filesystem(void);
#ifndef THREAD_MODE
int main(void)
{ 
eMBErrorCode    eStatus;

  TestEEPROM_Filesystem();
  SYS_Init(); 	 
  SYS_Config(); 
  ADE7878_main();
 eStatus = eMBInit( MB_RTU, 2, 0,9600, MB_PAR_NONE );
 /* Enable the Modbus Protocol Stack. */
 eStatus = eMBEnable();
  for(;;)
   {

   	 	if(LocalTime-Mbtick>=2000)
	 	{
		 Mbtick=LocalTime;
		}
	 	 PowerMeasurment();
    	
	
   }

}
#endif
