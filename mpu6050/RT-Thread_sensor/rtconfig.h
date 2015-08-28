/* RT-Thread config file */
#ifndef __RTTHREAD_CFG_H__
#define __RTTHREAD_CFG_H__

/* RT_NAME_MAX*/
#define RT_NAME_MAX     6

/* RT_ALIGN_SIZE*/
#define RT_ALIGN_SIZE	4

/* PRIORITY_MAX */
#define RT_THREAD_PRIORITY_MAX	32

/* Tick per Second */
#define RT_TICK_PER_SECOND	100

/* SECTION: RT_DEBUG */
/* Thread Debug */
#define RT_DEBUG
#define RT_USING_OVERFLOW_CHECK

/* Using Hook */
#define RT_USING_HOOK

/* SECTION: IPC */
/* Using Semaphore*/
#define RT_USING_SEMAPHORE
/* Using Mutex */
#define RT_USING_MUTEX
/* Using MailBox */
#define RT_USING_MAILBOX
#define RT_USING_EVENT

/* SECTION: Memory Management */
/* Using Memory Pool Management*/
// #define RT_USING_MEMPOOL

/* Using Dynamic Heap Management */
#define RT_USING_HEAP
/* Using Small MM */
#define RT_USING_SMALL_MEM

// <bool name="RT_USING_COMPONENTS_INIT" description="Using RT-Thread components initialization" default="true" />
#define RT_USING_COMPONENTS_INIT

/* SECTION: Device System */
/* Using Device System */
#define RT_USING_DEVICE
// <bool name="RT_USING_DEVICE_IPC" description="Using device communication" default="true" />
#define RT_USING_DEVICE_IPC
// <bool name="RT_USING_SERIAL" description="Using Serial" default="true" />
#define RT_USING_SERIAL

/* SECTION: Console options */
#define RT_USING_CONSOLE
/* the buffer size of console*/
#define RT_CONSOLEBUF_SIZE	        128
// <string name="RT_CONSOLE_DEVICE_NAME" description="The device name for console" default="uart1" />
#define RT_CONSOLE_DEVICE_NAME	    "uart1"

/* SECTION: finsh, a C-Express shell */
#define RT_USING_FINSH
#define FINSH_THREAD_PRIORITY   3
/* Using symbol table */
#define FINSH_USING_SYMTAB
#define FINSH_USING_DESCRIPTION
#define FINSH_USING_MSH
#define FINSH_USING_MSH_ONLY

/* SPI device */
#define RT_USING_SPI

/* IIC device */
#define RT_USING_I2C
#define RT_USING_I2C_BITOPS

/* Enable C++ support */
#define RT_USING_CPLUSPLUS
#define RT_USING_LIBC

#define RT_USING_SENSOR

#endif
