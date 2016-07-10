/*
 * File      : board.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2009 RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      first implementation
 * 2016-04-12     Bernard      first implementation
 */

#include <rthw.h>
#include <rtthread.h>
#include <stdio.h>
#include "NUC472_442.h"
#include "gpio.h"
#include "ade7878_Int.h" 
#include"Usart.h"
#include "ADE7878_Measure.h"
#include"Config.h"
#include "board.h"

/**
 * This function will initial NUC4XX board.
 */
void rt_hw_board_init()
{

  SYS_Init(); 	 
  SYS_Config();
  ADE7878_main();

//	rt_hw_usart_init();
#ifdef RT_USING_CONSOLE
	rt_console_set_device(CONSOLE_DEVICE);
#endif
}

/*@}*/
