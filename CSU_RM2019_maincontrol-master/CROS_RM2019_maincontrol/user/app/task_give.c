#include "task_give.h"
#include "task_remote.h"

#include "tim3_4.h"

void	task_give_test(void* param)
{
	/*
	give_motor_open(100);
	while(give_motor_circle() > -800)
		task_delay_ms(0);
	*/
	
	give_motor_close(100);
	while(give_motor_circle() < 100)
		task_delay_ms(0);
	
	give_motor_stop();
	while(1){
		PRINT("count: %d\r\n", give_motor_circle());
		task_delay_ms(100);
	}
}

void	task_give(void* param)
{
	p_give_ctrl	data;
	U32		door;
	//S32		mark=0;
	
	TIM3_init();
	TIM4_init();
	
	task_delay_ms(50);
	
	give_motor_stop();
	
	//task_insert(task_give_test, NULL, 2);
	
	while(1)
	{
		data	= msg_get_read_try(&give_msg);
		if(data)
		{
			door	= data->door;
			msg_read_finish_try(&give_msg);
			switch(door)
			{
				/*
				case DOOR_OPEN:
					if(give_motor_circle() > -900){
						give_motor_open(100);
					}else{
						give_motor_stop();
					}
					break;
				case DOOR_CLOSE:
					if(give_motor_circle() < -10){
						give_motor_close(100);
					}else{
						give_motor_stop();
					}
					break;
				*/
				/*
				case DOOR_OPEN:
					if(give_motor_circle() > -100){
						if(mark != 1){
							give_motor_open(30);
							mark = 1;
						}
					}else{
						give_motor_stop();
					}
					break;
				case DOOR_CLOSE:
					if(give_motor_circle() < -10){
						if(mark != -1){
							give_motor_close(30);
							mark = -1;
						}
					}else{
						give_motor_stop();
					}
					break;
				*/
				/*
				case DOOR_OPEN:
					give_motor_open(30);
					task_delay_ms(1000);
					break;
				case DOOR_CLOSE:
					give_motor_close(30);
					task_delay_ms(1000);
					break;
				case DOOR_STOP:
					give_motor_stop();
					break;
					*/
				case DOOR_OPEN:
					give_motor_open(80);
					task_delay_ms(300);
					break;
				case DOOR_CLOSE:
					give_motor_close(80);
					task_delay_ms(300);
					break;
				case DOOR_STOP:
					give_motor_stop();
					break;
				default:
					break;
			}
		}
		task_delay_ms(10);
	}
}





