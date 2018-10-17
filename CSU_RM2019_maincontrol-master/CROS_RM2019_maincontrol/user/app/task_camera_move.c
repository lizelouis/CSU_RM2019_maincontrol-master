#include "task_camera_move.h"
#include "task_remote.h"

u16 servo_pwm = 0;

void camera_move(U32	param)
{
	if(param == 0)
	{
		servo_pwm = 500;
	}
	else if(param == 1)
	{
		servo_pwm = 2500;
	}
}


p_view_ctrl	pview;
void task_camera_move(void* param)
{
	resolve_add("camera_move", camera_move);

	while(1)
	{
		pview = msg_get_read_try(&view_msg);
		if(pview){
			if(pview->view == VIEW_FRONT)
				servo_pwm = 500;
			else if(pview->view == VIEW_BACK)
				servo_pwm = 2500;
			msg_read_finish_try(&view_msg);
		}
		
		TIM5->CCR4 = servo_pwm;
		task_delay_ms(10);
	}
	
}
