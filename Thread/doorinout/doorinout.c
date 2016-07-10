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
#include "doorinout.h"



#ifdef USING_DOOR 


#define  GET_BUTTON  	   (PB11 & 0x01)

#define  DoorCtrl_KEY 		        0x01    


extern uint16_t  usSRegHoldBuf[100];




  void ConfigHWDoor(void)
{

 	GPIO_SetMode(PB, BIT11,GPIO_MODE_INPUT); //开关
  	GPIO_SetMode(PB, BIT10,GPIO_MODE_INPUT); //门磁
	GPIO_SetMode(PB, BIT9, GPIO_MODE_INPUT); //
  	GPIO_SetMode(PB, BIT8, GPIO_MODE_OUTPUT); //door


} 

unsigned char Trg;//触发
unsigned char Cont;//连续
static char flage=0;

void KeyRead( void )
{
    unsigned char ReadData = 255;

		  ReadData =255;
	if(GET_BUTTON==0){ReadData=(ReadData&0xfe);}

//	if(!GET_BUTTON2){ReadData=(ReadData&0xfd);}

//	if(!GET_BUTTON3){ReadData=(ReadData&0xfb);}

//	if(!GET_BUTTON4){ReadData=(ReadData&0xf7);}

	ReadData =ReadData ^0xff;
    Trg = ReadData & (ReadData ^ Cont); 
    Cont = ReadData; 

}

void KeyProc(void)
{
  
	//static char child_lock_time=0;
	if(Cont|Trg )
	{
		
	}
	
       if (Cont & DoorCtrl_KEY)
    {    




	}



	  if (Trg & DoorCtrl_KEY)
	  {
	  	 
					if(flage==0)
					{
					  flage=1;

				   PD15=0;//指示继电器状态
				   PB8=1;//打开继电器
				   usSRegHoldBuf[22]=0x01;	 //继电器状态
				 
				 	}
					else
					{
					  flage=0;
                   PD15=1;//指示继电器状态
				   PB8=0;//关闭继电器
				   usSRegHoldBuf[22]=0x00;	 //继电器状态

					}
				 
	  
	  }

 }


//测量门磁输入和按键输入以及控制继电器输出主线程
static char thread_Door_stack[1024];
struct rt_thread thread_Door;
void rt_thread_entry_Door(void* parameter)
{  
	   

	  //modbus 地址是10 
	  PB8=1;//打开继电器
	  usSRegHoldBuf[20]=0x00;//控制继电器寄存器为默认值挤不开也不关
	  usSRegHoldBuf[22]=0x01;//继电器状态打开
	while(1)
	{


			 if(PB10==0) //门磁状态
			 {
			 
			    usSRegHoldBuf[21]=0x01;	 //门磁状态
			 
			 }
			 else
			 if(PB10==1)
			 {
			 
			 	usSRegHoldBuf[21]=0x00; //门磁状态

			 }
		



			if(usSRegHoldBuf[20]==0x1)//如果485控制下
			{
			  	
			   PD15=0;//指示继电器状态
			   PB8=1;//打开继电器
			   usSRegHoldBuf[22]=0x01;	 //继电器状态
			   usSRegHoldBuf[20]=0; //归位
			  	
			}
			else 
			if (usSRegHoldBuf[20]==0x2)
			{
			   
			   PD15=1;//指示继电器状态
			   PB8=0;//关闭继电器
			   usSRegHoldBuf[22]=0x00;	 //继电器状态
			   usSRegHoldBuf[20]=0; //归位
			}


			  KeyRead();
			  KeyProc();	



	 rt_thread_delay(50); // 线程CPU主动让出	

	}




}


 void init_Door_thread(char priority,int tick)
 {
//------- init Door thread
    rt_thread_init(&thread_Door,
                   "Door",
                   rt_thread_entry_Door,
                   RT_NULL,
                   &thread_Door_stack[0],
                   sizeof(thread_Door_stack),priority,tick);
    rt_thread_startup(&thread_Door);

 }

 #endif
