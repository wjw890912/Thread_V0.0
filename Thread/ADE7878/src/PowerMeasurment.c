






#include <stdio.h>
#include <board.h>
#include <rtthread.h>
#include "NUC472_442.h"
#include <stdio.h>
#include "gpio.h"
#include "ade7878_Int.h" 
#include"Usart.h"
#include "ADE7878_Measure.h"
#include"Config.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "PowerMeasurment.h"


#ifdef  USING_POWERMEASURMENT

ALIGN(RT_ALIGN_SIZE)
static char thread_PowerMeasurment_stack[1024];
struct rt_thread thread_PowerMeasurment;
extern void PowerMeasurment(void);
static void rt_thread_entry_PowerMeasurment(void* parameter)
{
    while (1)
    {	 
	   //50ms采集一次电能数据并显示
	   #ifdef BT_BOARD
	   PD14=0;
	   #else
	    PB13=0;
	  #endif
        rt_thread_delay(250); 
		PowerMeasurment();
		#ifdef BT_BOARD
	   PD14=1;
	   #else
	    PB13=1;
	  #endif
       	rt_thread_delay(250);  
    }
}


  void init_PowerMeasurment_thread(char priority,int tick)
  {
    //------- init PowerMeasurment thread
    rt_thread_init(&thread_PowerMeasurment,
                   "PowerMeasurment",
                   rt_thread_entry_PowerMeasurment,
                   RT_NULL,
                   &thread_PowerMeasurment_stack[0],
                   sizeof(thread_PowerMeasurment_stack),priority,tick);
    rt_thread_startup(&thread_PowerMeasurment);
	}

#endif
