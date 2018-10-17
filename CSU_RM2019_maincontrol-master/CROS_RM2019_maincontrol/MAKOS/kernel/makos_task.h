#ifndef _MAKOS_TASK_H
#define _MAKOS_TASK_H

#include "makos_kernel_includes.h"

#include "makos_pipe.h"

typedef	void	(*task_f)(void*);
typedef		U32		task_stack_t;
typedef		U32		task_tick_t;
typedef 	U8		task_prio_t;
typedef enum
{
	state_run,
	state_ready,
	state_delay,
	state_suspend,
} task_state_t;

#pragma pack(8) 
typedef struct _TCB_t
{
	task_stack_t*	sp;
	
	task_state_t	state;
	task_prio_t	priority;
	task_tick_t	delay_tick;
	usage_t		period_usage;//p
	
	dnode_t		dnd_state;
	dnode_t		dnd_sus;
	dnode_t		dnd_record;
	
	pipe_t		pipe_print;
	dnode_t		dnd_print;
	
	U8 * pr_tcb_name;

	int temp;//用于解决任务堆栈8字节对齐	
	
	task_stack_t	task_stack[TASK_STACK_SIZE];
} TCB_t, *p_TCB;
#pragma pack() 

#define	task_create(func, param, prio)	task_create_full(func, #func, param, prio, PIPE_PRINT_SIZE)

p_TCB	task_create_full(task_f func,u8 * pre_tcb_name, void* param, task_prio_t prio, U32 pipe_size);

#define	task_insert_full(func, param, prio, pipe)		\
	do							\
	{							\
		irq_close();					\
		task_create_full(func, #func,param, prio, pipe);	\
		irq_restore();					\
	}while(0)				

#define	task_insert(func, param, prio)	task_insert_full(func, param, prio, PIPE_PRINT_SIZE)


#endif
