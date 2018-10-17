/*
	FILE NAME:	task_init.c
	DESCRIPTION:	
	
	EDIT RECORDS:
	---------
	AUTHOR:		FAN YAOWEN
	EDIT TIME:	2018/3/10
	MODIFICATION:	
	---------
	AUTHOR:		
	EDIT TIME:	
	MODIFICATION:	
	---------
*/


#include "task_init.h"

#include "flash_data.h"

#include "task_remote.h"
#include "task_chassis.h"
#include "task_holder.h"
#include "task_shoot.h"
#include "task_drag.h"
#include "task_give.h"
#include "task_car_rise.h"
#include "task_clip.h"
#include "task_camera_move.h"
#include "task_bullet.h"
#include "task_guard.h"
#include "task_judge.h"
#include "task_uartdma_test.h"



#define	READ_FLASH()								\
	do{									\
		U32	i;							\
		U32	buff[10];						\
		PRINT("read flash: ");						\
		Flash_Read32BitDatas(ADDR_FLASH_SECTOR_9, 10, (S32*)buff);	\
		for(i=0; i<10; i++)						\
			PRINT("%d/", *(buff+i));				\
		PRINT("\r\n");							\
	}while(0)





void	task_init(void* param)
{
	CAN1_Configuration((((u32)Control_ID<<3)&0xffff0000)>>16,(((u32)Control_ID<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xffff);
	can2_Configuration((((u32)Control_ID<<3)&0xffff0000)>>16,(((u32)Control_ID<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xffff);
	
	remote_send_msg_init();
	
	switch(FD.car_id)
	{
		case CAR_ID_SOLDIER:
			PRINT("CAR ID is SOLDIER\r\n");
			
			task_insert(task_remote,	NULL,	1);
			task_insert(task_chassis,	NULL,	2);
			//task_insert(task_holder,	NULL,	3);
		  //7task_insert(task_shoot,		NULL,	2);
			//task_insert(task_judge,		NULL,	2);
			//task_insert(task_uartdma_test,NULL,2);
			
			break;
		
		case CAR_ID_HERO:
			PRINT("CAR ID is HERO\r\n");
			
			task_insert(task_remote,	NULL,	1);
			task_insert(task_chassis,	NULL,	2);
			//task_insert(task_holder,	NULL,	3);
			task_insert(task_shoot,		NULL,	2);
			task_insert(task_car_rise_hero,	NULL,	2);
			task_insert(task_bullet,	NULL,	2);
			task_insert(task_clip,		NULL,	2);
			task_insert(task_camera_move,	NULL,	2);
			
			break;
		case CAR_ID_ENGINEER:
			PRINT("CAR ID is ENGINEER\r\n");
			
			task_insert(task_remote,	NULL,	1);
			task_insert(task_chassis,	NULL,	2);
			task_insert(task_drag,		NULL,	2);
			task_insert(task_give,		NULL,	2);
			task_insert(task_car_rise_engineer,	NULL,	2);
			task_insert(task_bullet,	NULL,	2);
			task_insert(task_clip,		NULL,	2);
			task_insert(task_camera_move,	NULL,	2);
			
			break;
		case CAR_ID_GUARD:
			PRINT("CAR ID is GUARD\r\n");
			
			task_insert(task_guard_move,	NULL,	2);
			
			break;
		default:
			PRINT_ERROR("CAR ID is unknown: %d\r\n", FD.car_id);
			//break;
	}
	
	task_suspend(ptcb_curr);
	
	while(1)
	{
		task_delay_ms(1000);
	}
}


