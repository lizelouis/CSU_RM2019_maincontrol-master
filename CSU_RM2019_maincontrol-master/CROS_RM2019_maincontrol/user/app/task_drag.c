#include "task_drag.h"
#include "task_remote.h"
#include "task_holder.h"


unsigned char count = 0;
S16	current = 0;
extern
S16	drag_angle;
void SAVE(int Rec)
{
	if(Rec == 1)
		{
		if((drag_angle < 2100)&&(drag_angle > 100))
      current = -800;//	RM6025Cmd(-800,0);//1400-800
		else if((drag_angle > 2100)&&(drag_angle < 8100))
		  current = 700;//RM6025Cmd(700,0);
		else
		  current = -500;//RM6025Cmd(-500,0);//wait 2
	}
		else if(Rec == 2)
		{
					current = 0;
		}
		else if(Rec == 3)
		{
			if(drag_angle>7000)
			{
				current = -1400;//RM6025Cmd(-1400,0);
				count = 1;	
			}     
			if(count == 1)			
	       if(drag_angle < 4800)
				 {
					 current = -200;//RM6025Cmd(-200,0);
					 count = 0;
				 }
		}	
		RM6025_set_yaw(current);
}



U32		drag = DRAG_UP;

void	drag_test(void* param)
{
	while(1)
	{
		PRINT("drag: %x\r\n", drag);
		task_delay_ms(1000);
	}
}

extern
CanTxMsg	RM6025_set_data;

void	task_drag(void* param)
{
	p_drag_ctrl	data;
	
	//task_insert(drag_test, NULL, 2);
	
	RM6025_set_data_init(&RM6025_set_data);
	
	while(1)
	{
		data	= msg_get_read_try(&drag_msg);
		if(data)
		{
			drag	= data->drag;
			msg_read_finish_try(&drag_msg);
		}
		switch(drag)
		{
			case DRAG_UP:
				SAVE(3);
				break;
			case DRAG_MID:
				SAVE(1);
				break;
			case DRAG_DOWN:
				SAVE(2);
				break;
			default:
				break;
		}
		task_delay_ms(10);
	}
}





