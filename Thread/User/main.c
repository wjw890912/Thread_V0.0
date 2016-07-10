
#include "stm32f10x_it.h"
#include "usart.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* Tick per Second */	//10ms
#define RT_TICK_PER_SECOND	100

void  SysTick_Configuration(void);
void NVIC_Configuration(void);
/* ----------------------- Defines ------------------------------------------*/
#define REG_INPUT_START 1000
#define REG_INPUT_NREGS 4

/* ----------------------- Static variables ---------------------------------*/
static USHORT   usRegInputStart = REG_INPUT_START;
static USHORT   usRegInputBuf[REG_INPUT_NREGS];
 extern uint32_t Tick;
/* ----------------------- Start implementation -----------------------------*/
int
main( void )
{
    const UCHAR     ucSlaveID[] = { 0xAA, 0xBB, 0xCC };
    eMBErrorCode    eStatus;
	 char senddata[9]={0x04,2,3,4,5,6,7,8,9};

	/* init system setting */
	SystemInit();
	/*NVIC init*/
	NVIC_Configuration();
	/* init sysTick*/
   SysTick_Configuration();
	 /* init USART*/
	 Hw_Usart_init();
	
/*	 while(1)
	 {

	 	if(Tick>=100)
	 	{
  	UART1_SendStr("hello stm32 \r\n",sizeof("hello stm32 \r\n"));
	   Tick=0;
	    }
	 }
	*/

    eStatus = eMBMasterInit( MB_RTU, 2, 38400,MB_PAR_EVEN );

   // eStatus = eMBSetSlaveID( 0x34, TRUE, ucSlaveID, 3 );
 
    /* Enable the Modbus Protocol Stack. */
    eStatus = eMBMasterEnable();




    for( ;; )
    {
		 	if(Tick>=100)
	 	{
		 eMBMasterRTUSend( 2, &senddata[1],3 ) ;
		 Tick=0;
		}

        (void)eMBMasterPoll();
        /* Here we simply count the number of poll cycles. */
        usRegInputBuf[0]++;
    }
}


void  SysTick_Configuration(void)
{
	RCC_ClocksTypeDef  rcc_clocks;
	uint32_t         cnts;

	RCC_GetClocksFreq(&rcc_clocks);

	cnts = (uint32_t)rcc_clocks.HCLK_Frequency / RT_TICK_PER_SECOND;

	SysTick_Config(cnts);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
}


/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
#ifdef  VECT_TAB_RAM
	/* Set the Vector Table base location at 0x20000000 */
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
	/* Set the Vector Table base location at 0x08000000 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif
}



