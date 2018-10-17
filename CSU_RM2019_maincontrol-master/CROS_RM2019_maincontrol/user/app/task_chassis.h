/*
	FILE NAME:	task_chassis.c
	DESCRIPTION:	contain the bassis work in chassis task, 
								1.get the message from remote or upper monit,and then drive the 
								motor by doing some pid and calman processing.
								2.modify some param of pid and return the speed of the head left motor if the upper monit
								gives some command.
								3.read the corect direction of the motor and their order from the flash.	
	EDIT RECORDS:
	---------
	AUTHOR:         
	EDIT TIME:	2017/11/21-14:57
	MODIFICATION: the bassis contents
	---------
	AUTHOR:		
	EDIT TIME:	
	MODIFICATION:	
	---------
*/
#ifndef _TASK_CHASSIS_H
#define _TASK_CHASSIS_H

#include "makos_includes.h"
#include "motor.h"
#include "Lower machine.h"

#define SEND   1
#define	NOSEND 0

#define YES 1
#define NO  0


void	task_chassis(void* param);
															
//********************************************************

#endif


