/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */

/**
 * @addtogroup STM32
 */
/*@{*/

#include <stdio.h>


#include <board.h>
#include <rtthread.h>
#include "NUC472_442.h"
#ifdef RT_USING_LWIP
#include <lwip/sys.h>
#include <lwip/api.h>
#include <netif/ethernetif.h>
#include "stm32_eth.h"
#endif
#include <stdio.h>
#include "gpio.h"
#include "ade7878_Int.h" 
#include"Usart.h"
#include "ADE7878_Measure.h"
#include"Config.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- appliction thread includes ----------------------------------*/
#include "weight.h"
#include "doorinout.h"
#include "pidctrl.h"
#include "modbus_app.h"
#include "PowerMeasurment.h"


void rt_init_thread_entry(void* parameter)
{
 eMBErrorCode    eStatus;
 eStatus = eMBInit( MB_RTU, 10, 0,9600, MB_PAR_NONE );
 /* Enable the Modbus Protocol Stack. */
 eStatus = eMBEnable();
//FS
//GUI
}



int rt_application_init()
{
    rt_thread_t init_thread;

#if (RT_THREAD_PRIORITY_MAX == 32)
    init_thread = rt_thread_create("init",
                                   rt_init_thread_entry, RT_NULL,
                                   2048, 8, 20);
#else
    init_thread = rt_thread_create("init",
                                   rt_init_thread_entry, RT_NULL,
                                   2048, 80, 20);
#endif

    if (init_thread != RT_NULL)
        rt_thread_startup(init_thread);


	#ifdef USING_MODBUS
	 //init_Modbus_thread and set priority tick 
	  init_Modbus_thread(10,5);
	#endif
	#ifdef  USING_POWERMEASURMENT 
     //init_Powermearsurment_thread	and set priority tick 
	 init_PowerMeasurment_thread(11,5);
	#endif
	#ifdef  USING_WEIGHT 
	//init_Weight_thread and set priority tick 
	 init_Weight_thread(12,5);
	#endif
	#ifdef USING_DOOR 
	//init_Door_thread and set priority tick 
	 init_Door_thread(13,5);
	#endif
	#ifdef USING_PID
	//init_PID_thread and set priority tick 
	  init_PID_thread(14,5);
	#endif 




    return 0;
}

/*@}*/
