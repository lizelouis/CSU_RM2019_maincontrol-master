#ifndef _MAKOS_CONFIG_H
#define _MAKOS_CONFIG_H

/*********************************************/

#define		CLOCK_FREQ		168000000U

/*********************************************/

#define		TASK_PRIO_LOWEST	5

#define		TASK_STACK_SIZE		140//U32
#define		EXCEPTION_STACK_SIZE	//unuse

#define		IRQ_STAT_STACK_SIZE	10//U32

#define		MEMORY_POOL_SIZE	(100*1024/4)//U32

#define		PIPE_PRINT_SIZE		150//U8

/*********************************************/





/*********************************************/

//#define		DEBUG_TICK
//#define		DEBUG_SUSPEND
//#define			DEBUG_SCHEDULE

//#define		DEBUG_MUTEX

//#define			DEBUG_DNDQUEUE
#define			DEBUG_RESOLVER


#endif
