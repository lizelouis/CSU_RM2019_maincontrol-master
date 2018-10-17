#ifndef _MAKOS_RESOLVER_H
#define _MAKOS_RESOLVER_H

#include "makos_kernel_includes.h"

#include "makos_task.h"
#include "makos_system.h"
#include "makos_schedule.h"

#include "makos_pipe.h"

/*
	input format:	func_name(U32_param)
	for example:	cpu(0)
*/

#define TO_U32(x) *((U32 *)x)
#define TO_FLOAT(x) *((float *)x) 	

typedef	void	(*resolve_f)(void * p_param);

typedef	struct	_resolve_t
{
	dnode_t		dnd;
	U8*		name;
	resolve_f	func;
}resolve_t, *p_resolve;


extern	dnode_t	dhead_resolve;
extern	pipe_t	pipe_resolver;

void	USART1_IRQ_init(void);
void	resolver_init(void);
void	resolve_add(U8* name, resolve_f func);

U32	string_get_U32(const U8* s, U32 length);

void	task_resolver(void* param);






#endif
