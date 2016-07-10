#include <rtthread.h> 
#include <stdio.h>
#include "NUC472_442.h"
#include "gpio.h"
#include "ade7878_Int.h" 
#include"Usart.h"
#include "ADE7878_Measure.h"
#include"Config.h"
#include "ds12864.h"
#include "systimer.h"
#include "weight.h"
#include "doorinout.h"

void ConfigHWSpiport(void)
{

		// plese ensure has opened the system clock	of SPI1 mode.
   	CLK_EnableModuleClock(SPI1_MODULE);
	   //1st . config pin MFP function
	/*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/
	 /* SPI1:  PC15=MOSI0, PD0=MISO0, PD1=CLK */
	 //note :there is a qustion that for the Register option must be "OR" bits.
	 //othewith, will occured  changed other Hardwave bits. 
    SYS->GPC_MFPH |= (SYS_GPC_MFPH_PC15MFP_SPI1_MOSI0);
	SYS->GPD_MFPL |= (SYS_GPD_MFPL_PD0MFP_SPI1_MISO0 | SYS_GPD_MFPL_PD1MFP_SPI1_CLK);
	   //2st . config the SPI1 of Hardwave Port 
	/* Configure SPI1 as a master, MSB first, 8-bit transaction, SPI Mode-0 timing, clock is 400KHz */
    SPI_Open(SPI1, SPI_MASTER, SPI_MODE_0, 8, 400000);	//open SPI1 and config it.
    SPI_DisableAutoSS(SPI1);//closed the ss by hardwave and give it for softwave ctrl that well used ctrl SS/HSA pin select  ADE7878 into the SPI mode.
	SPI_TRIGGER(SPI1);//start SPI1 transf.
	   //3st . config the SS/HSA pin 
	GPIO_SetMode(PC, BIT12, GPIO_MODE_OUTPUT);// SP1 SS pin by SOFTWAVE ctrl
	PC12=1;// hold the pin hight..

	//2016 01 03  make by wangjunwei in bittel. 
}
void ConfigHWled(void)
{

	 #ifdef BT_BOARD

	 GPIO_SetMode(PD, BIT15, GPIO_MODE_OUTPUT); //LED1
  	 GPIO_SetMode(PD, BIT14, GPIO_MODE_OUTPUT); //LED2


	 #else
 	GPIO_SetMode(PB, BIT13, GPIO_MODE_OUTPUT); //LED1
  	GPIO_SetMode(PB, BIT12, GPIO_MODE_OUTPUT); //LED2
	 #endif

	 GPIO_SetMode(PD, BIT8, GPIO_MODE_OUTPUT); //R1
  	 GPIO_SetMode(PD, BIT9, GPIO_MODE_OUTPUT); //R2

}



 void SYS_Config(void)
 {
 
    LCD_init();	//init LCD 12864
	ConfigHWled();	   //init LED indecate
    ConfigHWSpiport(); 	//CONFIG FROM THE SPI1 COMMUCATION TO ADE7878 CHIP.
  	Config_ADE7878_IRQ(); //CONFIG FROM THE ADE7878'S "IRQ" TO OUR ...
	#ifdef USING_WEIGHT
	ConfigHWADCHX711();//config HX711 ADC
	ConfigSWTimr1();//using Timer0 server to IR decode.
    ConfigHWPwm0();
	ConfigHW_IR_IRQ();
	#endif
	#ifdef USING_DOOR
	ConfigHWDoor();
	#endif
	
	 

     //Usart_Init();		//init USART3 PORT


 }



