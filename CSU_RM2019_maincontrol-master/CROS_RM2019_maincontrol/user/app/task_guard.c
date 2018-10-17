#include "task_guard.h"


supply_pid_struct guard_move_pid[2];

void guard_move_init(void)
{
	int i = 0;
	for(i = 0; i < 2; i++)
	{
		guard_move_pid[i].kp = 2;
		guard_move_pid[i].ki = 0.1;
		guard_move_pid[i].kd = 0;
		

		guard_move_pid[i].err_0 = 0;
		guard_move_pid[i].err_1 = 0;
		guard_move_pid[i].err_2 = 0;
		
		guard_move_pid[i].err_kp = 0;
		guard_move_pid[i].err_ki = 0;
		guard_move_pid[i].err_kd = 0;
		
		guard_move_pid[i].err_ki_max = 10000;
		
		guard_move_pid[i].set_speed = 0;
		guard_move_pid[i].act_speed = 0;
		
		guard_move_pid[i].increment = 0;
		guard_move_pid[i].output = 0;

		guard_move_pid[i].calc = &supply_pid_calc;
	}
}

void guard_move(void* param)
{
	
}

extern volatile int16_t guard_move_speed[2];
u8 guard_move_data[8];
void task_guard_move(void* param)
{
	CanTxMsg	canmsg;
	int i = 0;
	resolve_add("guard_move", guard_move);
	guard_move_init();
	while(1)
	{
		i++;
		if(i < 1000)
		{
			guard_move_pid[0].set_speed = 3000;
			guard_move_pid[1].set_speed = -3000;
		}
		if(i >= 1000)
		{
			guard_move_pid[0].set_speed = -3000;
			guard_move_pid[1].set_speed = 3000;
		}
		if(i >= 2000)
		{
			i = 0;
		}
		guard_move_pid[0].act_speed = guard_move_speed[0];
		guard_move_pid[1].act_speed = guard_move_speed[1];
		guard_move_pid[0].calc(guard_move_pid);
		guard_move_pid[1].calc(guard_move_pid + 1);
		
		guard_move_data[4] = guard_move_pid[0].output>>8;
		guard_move_data[5] = guard_move_pid[0].output&0xff;
		guard_move_data[6] = guard_move_pid[1].output>>8;
		guard_move_data[7] = guard_move_pid[1].output&0xff;
//		guard_move_data[4] = 500>>8;
//		guard_move_data[5] = 0;
//		guard_move_data[6] = 500>>8;
//		guard_move_data[7] = 0;
		
		can_set(&canmsg, 0x1ff, guard_move_data);
		CAN_Transmit(CAN1, &canmsg);
		task_delay_ms(10);
	}
}


