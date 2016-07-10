



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
#include "modbus_app.h"

#ifdef USING_MODBUS

ALIGN(RT_ALIGN_SIZE)
static char thread_Rs485comm_stack[1024];
struct rt_thread thread_Rs485comm;
static void rt_thread_entry_Rs485comm(void* parameter)
{
  
    while(1)
    {

		//10MS采集一次485数据
		#ifdef BT_BOARD
	   // PD14=0;
	    #else
	    PB12=0;
	    #endif
      	(void)eMBPoll();
		#ifdef BT_BOARD
	   // PD14=0;
	    #else
	    PB12=0;
	    #endif
        rt_thread_delay(2);		

    }
}


  void init_Modbus_thread(char priority,int tick)
  {

 //------- init Rs485comm thread
    rt_thread_init(&thread_Rs485comm,
                   "Rs485comm",
                   rt_thread_entry_Rs485comm,
                   RT_NULL,
                   &thread_Rs485comm_stack[0],
                   sizeof(thread_Rs485comm_stack),priority,tick);
    rt_thread_startup(&thread_Rs485comm);
   }
   
#endif	