

#include <rtthread.h>
#include <stdio.h>
#include "NUC472_442.h"
#include "gpio.h"
#include "ade7878_Int.h" 
#include "ADE7878.h"
#include "ADE7878_SPI.h"
extern unsigned char IRQFlag ;
void Config_ADE7878_IRQ(void)
{

	#ifdef BT_BOARD

	GPIO_SetMode(PD, BIT3,  GPIO_MODE_INPUT);
    GPIO_EnableInt(PD, 2, GPIO_INT_FALLING); //IRQ1
	GPIO_EnableInt(PD, 3, GPIO_INT_FALLING); //IRQ0
	NVIC_SetPriority(GPD_IRQn, 5 );
    NVIC_EnableIRQ(GPD_IRQn);

	#else

 /*  Configure PB4-5 as INPUT mode and enable interrupt by falling edge trigger */
    GPIO_SetMode(PB, BIT5,  GPIO_MODE_INPUT);
    GPIO_EnableInt(PB, 4, GPIO_INT_FALLING); //IRQ1
	GPIO_EnableInt(PB, 5, GPIO_INT_FALLING); //IRQ0
	NVIC_SetPriority(GPB_IRQn, 5 );
    NVIC_EnableIRQ(GPB_IRQn);

   #endif

}
 #ifdef   USING_WEIGHT
// IR value
extern uint32_t number;//������
extern uint8_t flage;//��־��һ�ν��ж�   
extern uint8_t bit_number;//����λ
extern uint8_t tab[34];//�źŴ洢
extern uint8_t reciv_end;//�������
extern uint8_t dat[4]; //������������
 #endif

 #ifdef BT_BOARD
 void GPD_IRQHandler(void)
 {
 	  /* To check if PD2 interrupt occurred */
    if (PD->INTSRC & BIT2) 
	{
        PD->INTSRC = BIT2;
    
		 IRQFlag=1;//IRQ1 is occrued .
									  

    } else 
	/* To check if PD3 interrupt occurred */
	if(PD->INTSRC & BIT3)
	{	
		PD->INTSRC = BIT3;
	   	IRQFlag=1;//IRQ0 is occrued .
	}
	else
	{  
      PD->INTSRC = PD->INTSRC ;  
    }
 }

#endif



void GPB_IRQHandler(void)
{
	 
    /* To check if PB4 interrupt occurred */
    if (PB->INTSRC & BIT4) 
	{
        PB->INTSRC = BIT4;
    
		 IRQFlag=1;//IRQ1 is occrued .
									  

    } else 
	/* To check if PB5 interrupt occurred */
	if(PB->INTSRC & BIT5)
	{	
		PB->INTSRC = BIT5;
	   	IRQFlag=1;//IRQ0 is occrued .
	}
	else
	if(PB->INTSRC & BIT14)
	{
	   PB->INTSRC = BIT14;
	     
		 #ifdef   USING_WEIGHT
		 if(flage)	//�ж��ǲ��ǵ�һ�ν����ж�
	  		{
			
			   
			  if(number>900)//	�������������
			  {
			   bit_number=0;//�������ݼ���
			  
			  }
	  		tab[bit_number]=number;	 //�Ѽ���ֵ����������
			
			number=0;	   //����ֵ���㣬�ȴ���һ�μ���
			bit_number++; //����һ������׼��

			if(bit_number==33) //�ɼ���һ֡ ��
			{
		 	 reciv_end=1; //�������
		 
		 	  
			
			}

	  	
	 		 }
	 		 else
	 		 {
	  
	  		 number=0;	 //
	  		 flage=1;
		
			  }
	   		#endif
	}
	else
	{  
      PB->INTSRC = PB->INTSRC ;  
    }


}
 