#include <stdio.h>
#include "NUC472_442.h"
#include"Usart.h"

void Usart_Init(void)
{

   	  /* Enable IP clock */
    CLK_EnableModuleClock(UART3_MODULE);
    /* Select IP clock source */
    CLK_SetModuleClock(UART3_MODULE, CLK_CLKSEL1_UARTSEL_HIRC , CLK_CLKDIV0_UART(1));
   /* Set GPD multi-function pins for UART3 RXD and TXD */
    SYS->GPD_MFPL |= SYS_GPD_MFPL_PD4MFP_UART3_RXD | SYS_GPD_MFPL_PD5MFP_UART3_TXD  ;
	GPIO_SetMode(PD, 5, GPIO_MODE_OUTPUT);
	GPIO_SetMode(PD, 4, GPIO_MODE_INPUT);
    UART_Open(UART3, 9600);
   // UART_DisableFlowCtrl(UART3);
	//UART_SetLine_Config(UART3, 115200, UART_WORD_LEN_8,UART_PARITY_NONE,UART_STOP_BIT_1);
//	UART_EnableInt(UART3, UART_INTEN_RDAIEN_Msk );
//	NVIC_SetPriority(UART3_IRQn, 2 );
   	//NVIC_EnableIRQ(UART3_IRQn);	//UART_INTEN_THREIEN_Msk 
    NVIC_DisableIRQ(UART3_IRQn);	
  UART_Write(UART3,"Usart3 init done!\r\n", sizeof("Usart3 init done!\r\n"));
 
}

 
 /* 
void UART3_IRQHandler(void)
{
    uint32_t u32IntSts= UART3->INTSTS;
	char u;
	  
	if(u32IntSts & UART_INTSTS_RDAINT_Msk)
	 {
		
        while(UART_IS_RX_READY(UART3))
		 {
		     u=UART_READ(UART3);  
		}
     }
	 if(u32IntSts & UART_INTSTS_THREINT_Msk)
	 
	  {
	   	  UART_ClearIntFlag(UART3, UART_INTSTS_THREINT_Msk);
	 }	  
	
}

	  */
   







