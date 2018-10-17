/*
	FILE NAME:	task_bullet.c
	DESCRIPTION:	control the hero and engineer's bullet collecting(strech arm and clip).
			using c/v(+shift/ctrl) to change the state of strech arm and clip.
	
	EDIT RECORDS:
	---------
	AUTHOR:		FAN YAOWEN
	EDIT TIME:	2018/4/5
	MODIFICATION:	the bullet_msg is tested
*/

#include "task_bullet.h"
#include "task_remote.h"

#include "flash_data.h"


U32	bullet_print	= 0;
U32	bullet_print_mark	= 0;
void	bullet_control(U32 param)
{
	if(param < 3)
		bullet_print = param;
}


extern volatile int16_t strech_speed;
supply_pid_struct strech_pid;

void strech_init(void)
{
	strech_pid.kp = 2;
	strech_pid.ki = 0.1;
	strech_pid.kd = 0;
		

	strech_pid.err_0 = 0;
	strech_pid.err_1 = 0;
	strech_pid.err_2 = 0;
		
	strech_pid.err_kp = 0;
	strech_pid.err_ki = 0;
	strech_pid.err_kd = 0;
		
	strech_pid.err_ki_max = 5000;
		
	strech_pid.set_speed = 0;
	strech_pid.act_speed = 0;
		
	strech_pid.increment = 0;
	strech_pid.output = 0;

	strech_pid.calc = &supply_pid_calc;
}

s16 strech_expect_cricle;
U8 strech_speed_data[8];

void strech_motor(void)
{
	CanTxMsg	canmsg;
	
	if(motor_circle[0][2].circle > strech_expect_cricle)
	{
		strech_pid.set_speed = -500;
	}
	if(motor_circle[0][2].circle < strech_expect_cricle)
	{
		strech_pid.set_speed = 500;
	}
	if(motor_circle[0][2].circle == strech_expect_cricle)
	{
		strech_pid.set_speed = 0;
	}
	
	strech_pid.act_speed = strech_speed;
	
	strech_pid.calc(&strech_pid);
	
	strech_speed_data[4] = strech_pid.output>>8;
	strech_speed_data[5] = strech_pid.output&0xff;
	
	
}





void	task_bullet(void* param)
{
	p_bullet_ctrl	data;
	U32		strech;
	strech_init();
	resolve_add("bullet", bullet_control);
	
	while(1)
	{
		data	= msg_get_read_try(&bullet_msg);
		if(data)
		{
			strech	= data->strech;
			switch(strech)
			{
				case STRECH_IN:
					strech_expect_cricle = 0;
					break;
				case STRECH_MID:
					if(FD.car_id == CAR_ID_HERO){
						strech_expect_cricle = 12;
					}else{
						strech_expect_cricle = 30;
					}
					break;
				case STRECH_OUT:
					if(FD.car_id == CAR_ID_HERO){
						strech_expect_cricle = 25;
					}else{
						strech_expect_cricle = 50;
					}
					/*
					if(strech < out)
						strech_out();
					else
						strech_stop();
					*/
					break;
				default:
					break;
			}
			msg_read_finish_try(&bullet_msg);
		}
		
		strech_motor();
		
		if(bullet_print && (system_clock%1000 < 50) && bullet_print_mark)
		{
			switch(strech)
			{
				case STRECH_IN: PRINT("strech: in\r\n"); break;
				case STRECH_MID: PRINT("strech: mid\r\n"); break;
				case STRECH_OUT: PRINT("strech: out\r\n"); break;
				default: PRINT("strech: unknown(%d)\r\n", strech);break;
			}
			if(bullet_print == 2)
				bullet_print = 0;
			bullet_print_mark = 0;
		}
		if(system_clock%1000>100)
			bullet_print_mark = 1;
		
		
		task_delay_ms(10);
	}
}







