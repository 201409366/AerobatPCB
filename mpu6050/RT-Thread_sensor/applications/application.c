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
 * 2014-04-10     Bernard      build for STM32F401 discovery board
 */

#include <rtthread.h>
#include <components.h>

static void init_thread_entry(void* parameter)
{
    /* print board information */
    rt_kprintf("Sensor branch in STM32F401VC discovery board.\n");
    rt_components_init();
}

int rt_application_init()
{
    rt_thread_t tid;
    tid = rt_thread_create("init",
                           init_thread_entry, RT_NULL,
                           2048, RT_THREAD_PRIORITY_MAX/3, 20);
    if (tid != RT_NULL)
        rt_thread_startup(tid);

    return 0;
}
