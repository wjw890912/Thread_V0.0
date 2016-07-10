


#include <rthw.h>
#include <rtthread.h>    
#include <stdio.h>
#include "NUC472_442.h"
#include "gpio.h"
#include"Config.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "weight.h"
#ifdef USING_WEIGHT

//weight value

void CleanWeight(void);
static uint32_t WeightAD1,WeightAD2;//the RAW weight AD value
static uint32_t	WeightADCalibration=1010;//Calibration adc =1010
static uint32_t WeightADCSignBit=0;//SignBit 0:+ 1:-
static uint32_t Heavy=0,HeavyAlarm=1000;//Wet=1KG
extern uint16_t  usSRegHoldBuf[100];					   

// IR value
uint32_t number;//计数器
uint8_t flage;//标志第一次进中断   
uint8_t bit_number;//数据位
uint8_t tab[34];//信号存储
uint8_t reciv_end;//接收完毕
uint8_t dat[4]; //解晚的码放这里




//The ADC conver chip is <HX711>  -=-HW
 //The ADC Data defined
 #define  ADDO_UP 
 #define  ADDO_DOWN 
 #define  ADDO		 PB1
//The ADC Clock defined
 #define  ADSK_UP 	   
 #define  ADSK_DOWN 
 #define  ADSK		 PB15
 //The positive and negative critical value
 #define  PMVAL      500
 //allow a tiny delay!!!
 #define   TINY_SW_DELAY 


void ConfigHWPwm0(void)
{

		  /* Unlock protected registers */
    SYS_UnlockReg();
	CLK_EnableModuleClock(PWM0CH01_MODULE);
    //CLK_EnableModuleClock(PWM0CH23_MODULE);
    //CLK_EnableModuleClock(PWM0CH45_MODULE);

	 /* Set PCLK as PWM0 channel 0~3 clock source */
   // CLK_SetModuleClock(PWM0CH01_MODULE,CLK_CLKSEL2_PWM0CH01SEL_PCLK,1);
    //CLK_SetModuleClock(PWM0CH23_MODULE,CLK_CLKSEL2_PWM0CH23SEL_PCLK,1);
    //CLK_SetModuleClock(PWM0CH45_MODULE,CLK_CLKSEL2_PWM0CH45SEL_PCLK,1);
	 /* Set PCLK as PWM0 channel 0~1 clock source */
    CLK_SetModuleClock(PWM0CH01_MODULE,CLK_CLKSEL2_PWM0CH01SEL_PCLK,1);
	 /* Set GPG multi-function pins for CKO */
   // SYS->GPC_MFPL = SYS_GPC_MFPL_PC5MFP_CLK_O ;  //no use

	/* Set A5 A6 C10 C11 A12 A11 multi-function pins for PWM Channel 0~5  */
    //SYS->GPA_MFPL =  SYS_GPA_MFPL_PA5MFP_PWM0_CH0 | SYS_GPA_MFPL_PA6MFP_PWM0_CH1;
   // SYS->GPC_MFPH =  SYS_GPC_MFPH_PC10MFP_PWM0_CH2 | SYS_GPC_MFPH_PC11MFP_PWM0_CH3;
    //SYS->GPA_MFPH =  SYS_GPA_MFPH_PA12MFP_PWM0_CH4 | SYS_GPA_MFPH_PA11MFP_PWM0_CH5;
	 
	 
	 /*SET PF10 pin for PWM0  Channel 1	*/
	  SYS->GPF_MFPH =  SYS_GPF_MFPH_PF10MFP_PWM0_CH1;

	  /* Vref connect to AVDD */
    SYS->VREFCTL |= SYS_VREFCTL_ADCMODESEL_Msk;



/**************PWM config**************/


     // PWM0 frequency is 100Hz, duty 30%,
//    PWM_ConfigOutputChannel(PWM0, 0, 100, 30);
   // PWM_EnableDeadZone(PWM0, 0, 400);

    // PWM2 frequency is 300Hz, duty 50%
   // PWM_ConfigOutputChannel(PWM0, 2, 300, 50);
  //  PWM_EnableDeadZone(PWM0, 2, 200);

    // PWM4 frequency is 500Hz, duty 70%
   // PWM_ConfigOutputChannel(PWM0, 4, 600, 70);
  //  PWM_EnableDeadZone(PWM0, 4, 100);

    // Enable complementary mode
   // PWM_ENABLE_COMPLEMENTARY_MODE(PWM0);

    // Enable output of all PWM channels
    //PWM_EnableOutput(PWM0, 0x3F);

    // Enable PWM channel 0 period interrupt, use channel 0 to measure time.
   // PWM_EnablePeriodInt(PWM0, 0, 0);
  //  NVIC_EnableIRQ(PWM0CH0_IRQn);

    // Start
   // PWM_Start(PWM0, 0x3F);
	//    PWM_Start(PWM0, (PWM_CH_0_MASK|PWM_CH_2_MASK));

	  /* PWM0 frequency is 1000Hz, duty 50%,*/
   PWM_ConfigOutputChannel(PWM0, 1, 1000, 50);
   PWM_EnableDeadZone(PWM0, 1, 0);
	 // Enable output of all PWM channels
   PWM_EnableOutput(PWM0, PWM_CH_1_MASK);
   PWM_Start(PWM0, (PWM_CH_1_MASK));
}
 
void PWM0CH0_IRQHandler(void)
{
    static uint32_t cnt;
    static uint32_t out;

    // Channel 0 frequency is 100Hz, every 1 second enter this IRQ handler 100 times.
    if(++cnt == 100) {
        if(out)
            PWM_EnableOutput(PWM0, 0x3F);
        else
            PWM_DisableOutput(PWM0, 0x3F);
        out ^= 1;
        cnt = 0;
    }
    // Clear channel 0 period interrupt flag
    PWM_ClearPeriodIntFlag(PWM0, 0);
}

/**************TIMR1 config**************/

 void ConfigHW_IR_IRQ(void)
{
 /*  Configure PB4-5 as INPUT mode and enable interrupt by falling edge trigger */
    GPIO_SetMode(PB, BIT14,  GPIO_MODE_INPUT);
    GPIO_EnableInt(PB, 14, GPIO_INT_FALLING); //IRQ1
    NVIC_EnableIRQ(GPB_IRQn);

}


/***********************
  读一个字节
  利用了周期的长短，长的就是1短的就是0
 ************************/
void  read_byte() //读字节
{	  /******
		临时变量
		******/
	  uint8_t a;
	  uint8_t b=1;
	  uint8_t j;
	  uint8_t da;//接收数据
   for(j=0;j<4;j++)
		{									
for(a=0;a<8;a++)
{
		if(tab[b]>0x80)	//大于1.5ms是1，小于是0
		{
		/*set_bit(da,a);*/
		  da|=(0x01<<a);
		}
		else
		{
		/*clr_bit(da,a);*/
		  da&=~(0x01<<a);
		}
        b++;
														
}		
	  dat[j]=da;
		}		


}


//--IR time sever--
void ConfigSWTimr1(void)
{
    CLK_EnableModuleClock(TMR1_MODULE);
	CLK_SetModuleClock(TMR1_MODULE,CLK_CLKSEL1_TMR1SEL_PCLK, 0);
	 // Set timer frequency to 100KHZ
    TIMER_Open(TIMER1, TIMER_PERIODIC_MODE, 100000);//10us
    // Enable timer interrupt
    TIMER_EnableInt(TIMER1);
	NVIC_SetPriority(TMR1_IRQn,4);
	NVIC_EnableIRQ(TMR1_IRQn);
    // Start Timer 1
   // TIMER_Start(TIMER1);
   TIMER_Stop(TIMER1);// inited the timer is has closed  .
}

void TMR1_IRQHandler(void)
{
	  number++;//计数？YES IT IS	
    // clear timer interrupt 
    TIMER_ClearIntFlag(TIMER1);
}


 void ConfigHWADCHX711(void)
{

 	GPIO_SetMode(PB, BIT1, GPIO_MODE_INPUT); //DATA
  	GPIO_SetMode(PB, BIT15, GPIO_MODE_OUTPUT); //SCLK

} 

 void delayp(void)//smile delay 10US-0US
 {
  /*
   This delay can be added, also can not use,
   a tiny 50 - about 10 us delay.
   Enough good output waveform.

   using tiny delay is default so like this define 

   #define   TINY_SW_DELAY 



   */
   #ifdef TINY_SW_DELAY

//K=20 that mean's the physical time is about 1us by softwave
   int k=20;
   while(k)k--; 
   #endif
 
 }

/* 
Sampling external ADC Start implementation 
The chip is HX711 by DATA and SCLK line
20160501 testing OK!!!
*/
uint32_t ReadADCHX711(void)	
{

		  uint32_t Count;
		  uint8_t i;
		  ADSK=0;
		  Count=0;
		  delayp();
		  while(ADDO);  
		  for(i=0;i<24;i++)
		  {
		    delayp();
		    ADSK=1; 
			Count=Count<<1;
			delayp();
			ADSK=0;
			if(ADDO)Count++;
		  }
		  ADSK=1;
		  Count=Count^0x800000;
		  delayp();
		  ADSK=0;
		
		 return  Count;
}
void CleanWeight(void)
{
   WeightAD1= ReadADCHX711();
   WeightAD2=WeightAD1;
}

void DisplayweightLCD()
{


}

void UpdataWeight(uint32_t wet)
{

//更新RS485-MODBUS HOLD 寄存器
  usSRegHoldBuf[18]= wet;//单位g
  usSRegHoldBuf[19]= WeightADCSignBit;//正负标志
//更新LCD 


}

  //测量重量主线程
static char thread_Weight_stack[1024];
struct rt_thread thread_Weight;
void rt_thread_entry_Weight(void* parameter)
{  

   static uint32_t duty=2;
     WeightAD1= ReadADCHX711(); //Sampling ADC  1
     rt_thread_delay(500); //500ms
     WeightAD1= ReadADCHX711(); //Sampling ADC  2
	 rt_thread_delay(500); //500ms
     WeightAD1= ReadADCHX711(); //Sampling ADC  3 init and save it to WeightAD that it's effective
	 WeightAD2=WeightAD1;
     TIMER_Start(TIMER1);

	while(1)
	{
	
/******************PWM处理*****************************************************/
		   
			duty++;
		if(duty>90)duty=10;
	 PWM_ConfigOutputChannel(PWM0, 1, 1000, duty);
			

/******************称重传感器处理*****************************************************/

		WeightAD1 = ReadADCHX711(); //Sampling ADC
		
		if(WeightAD1>=WeightAD2)//当前采集重量超过上次采样
		{
		   WeightAD1=(WeightAD1-WeightAD2);//计算正差值
		}
		else
		{
		  
		   WeightAD1=(WeightAD2-WeightAD1);//计算负差值

		   if(WeightAD1>=PMVAL)
		   {
		   	 WeightADCSignBit=1;//负数
		   
		   }
		   else
		   {
			 WeightADCSignBit=0;//正数
			 WeightAD1=0;
		   }
		}


	   Heavy=((int)WeightAD1/(int)WeightADCalibration);//1677;	 //计算重量

	  	UpdataWeight(Heavy);//updata DATA

	   if(Heavy>HeavyAlarm)		  
	    {
	      //超重报警
	    }

/******************红外遥控器处理*****************************************************/

	   if(reciv_end)//接收完毕准备解码
		{	
			read_byte();//读取一个字节
				if(dat[2]+dat[3]==255)//IR data is ture ? yes do it other lose them 
				{

			   usSRegHoldBuf[14]=dat[0];
			   usSRegHoldBuf[15]=dat[1];
			   usSRegHoldBuf[16]=dat[2];
			   usSRegHoldBuf[17]=dat[3];
			    }
		   reciv_end=0;	//解码完毕	
		}
/******************红外遥控器处理*****************************************************/

	   rt_thread_delay(250); // 线程CPU主动让出
	  		  
		

	}




}


 void init_Weight_thread( char priority,int tick)
 {
//------- init Weight thread
    rt_thread_init(&thread_Weight,
                   "Weight",
                   rt_thread_entry_Weight,
                   RT_NULL,
                   &thread_Weight_stack[0],
                   sizeof(thread_Weight_stack),priority,tick);
    rt_thread_startup(&thread_Weight);

 }

















#endif


