#include "task_fl_test.h"

FUNC_LIST_DEFINE(fltest);

	
void*	fltest1(void* p1, void* p2, void* p3, void* p4)
{
	PRINT("fltest1: %d\r\n", *(U32*)p1);
	return	NULL;
}

void*	fltest2(void* p1, void* p2, void* p3, void* p4)
{
	PRINT("fltest2: %d\r\n", *(U32*)p2);
	return	NULL;
}

U32	a1=1, a2=2;

void	task_fl_test(void* param)
{
	fltest_fl_add(fltest1);
	fltest_fl_add(fltest2);
	
	while(1)
	{
		fltest_fl_run(&a1, &a2, NULL, NULL);
		task_delay_ms(1000);
	}
}

