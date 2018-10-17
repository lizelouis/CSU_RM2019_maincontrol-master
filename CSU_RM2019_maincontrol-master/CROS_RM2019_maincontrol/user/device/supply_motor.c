#include "supply_motor.h"
#include "can.h"

supply_pid_struct supply_pid;
bullet_supply_struct bullet_supply;


extern volatile int16_t supply_read_speed;
extern volatile U16 supply_read_angle;

void bullet_supply_count(struct bullet_supply_struct *bullet_supply)
{
	bullet_supply->current_angle = supply_read_angle;
	if(bullet_supply->count_judge_type == 1)
	{
		if((bullet_supply->count_angle_low < bullet_supply->current_angle)&&(bullet_supply->current_angle < bullet_supply->count_angle_high))
		{
			if(bullet_supply->change_flag == 1)
			{
				bullet_supply->circle_count++;
				bullet_supply->change_flag = 0;
			}
		}
		else
		{
			if(bullet_supply->change_flag == 0)
			{
				bullet_supply->change_flag = 1;
			}
		}
	}
	if(bullet_supply->count_judge_type == 0)
	{
		if((bullet_supply->count_angle_low < bullet_supply->current_angle)||(bullet_supply->current_angle < bullet_supply->count_angle_high))
		{
			if(bullet_supply->change_flag == 1)
			{
				bullet_supply->circle_count++;
				bullet_supply->change_flag = 0;
			}
		}
		else
		{
			if(bullet_supply->change_flag == 0)
			{
				bullet_supply->change_flag = 1;
			}
		}
	}
	if(bullet_supply->circle_count >= 37)
	{
		bullet_supply->circle_count = 0;
	}
	//PRINT_RECORD("%d\r\n", supply_read_angle);
}

void bullet_supply_move(struct bullet_supply_struct *bullet_supply)
{
	if(bullet_supply->act_flag == 1)
	{
		if((bullet_supply->circle_count%bullet_supply->move_angle) != 0)
		{
			speed_update(500);
		}
		if((bullet_supply->circle_count%bullet_supply->move_angle) == 0)
		{
//			speed_update(-10000);
//			task_delay_ms(40);
//			speed_update(0);
			bullet_supply->act_flag = 0;
		}
	}
}

void bullet_supply_init(struct bullet_supply_struct *bullet_supply)
{
	bullet_supply->move_speed = 5000;
	bullet_supply->reverse_speed = -10000;
	bullet_supply->reverse_time = 40;
	
	
	bullet_supply->start_angle = supply_read_angle;
	bullet_supply->count_angle_low = bullet_supply->start_angle - 500;
	bullet_supply->count_angle_high = bullet_supply->start_angle + 500;
	
	if(bullet_supply->count_angle_low < 0)
		bullet_supply->count_angle_low = bullet_supply->count_angle_low + 8191;
	if(bullet_supply->count_angle_high > 8191)
		bullet_supply->count_angle_high = 8191 - bullet_supply->count_angle_high;
	
	if(bullet_supply->count_angle_low < bullet_supply->count_angle_high)
		bullet_supply->count_judge_type = 1;
	if(bullet_supply->count_angle_low > bullet_supply->count_angle_high)
		bullet_supply->count_judge_type = 0;
	
	bullet_supply->change_flag = 0;

	bullet_supply->count = &bullet_supply_count;
	bullet_supply->move = &bullet_supply_move;
}



































void supply_pid_calc(struct supply_pid_struct *supply_pid)
{
	
//	s16 err_kp, err_ki, err_kd;
	
	//supply_pid->act_speed = supply_read_speed;
	
	//PRINT("%d\n", supply_read_speed);
	
	supply_pid->err_0 = supply_pid->set_speed - supply_pid->act_speed;
	
	//PRINT("%d\n", supply_pid->err_0);
	
	supply_pid->err_kp = supply_pid->err_0 - supply_pid->err_1;
	supply_pid->err_ki = supply_pid->err_0;
	supply_pid->err_kd = supply_pid->err_0 - 2*supply_pid->err_1 + supply_pid->err_2;
	
//	err_kp = supply_pid->err_0 - supply_pid->err_1;
//	err_ki = supply_pid->err_0;
//	err_kd = supply_pid->err_0 - 2*supply_pid->err_1 + supply_pid->err_2;
	

	
	if(supply_pid->err_ki > supply_pid->err_ki_max)
		supply_pid->err_ki = supply_pid->err_ki_max;
	else if(supply_pid->err_ki < -supply_pid->err_ki_max)
		supply_pid->err_ki = -supply_pid->err_ki_max;
	
	supply_pid->increment = supply_pid->kp*supply_pid->err_kp + supply_pid->ki*supply_pid->err_ki + supply_pid->kd*supply_pid->err_kd;
	//supply_pid->increment = 1*supply_pid->err_kp + 0*supply_pid->err_ki + 20*supply_pid->err_kd;
	//supply_pid->increment = 1*err_kp + 0*err_ki + 20*err_kd;
	
	//PRINT("%d\n", supply_pid->increment);
	
	supply_pid->err_1 = supply_pid->err_0;
	supply_pid->err_2 = supply_pid->err_1;
	
	if(supply_pid->increment > 10000)
		supply_pid->increment = 10000;
	else if(supply_pid->increment < -10000)
		supply_pid->increment = -10000;
	
	
	supply_pid->output = supply_pid->output + (s16)(supply_pid->increment);
	
}


void supply_pid_struct_init(struct supply_pid_struct *supply_pid)
{	
	supply_pid->err_0 = 0;
	supply_pid->err_1 = 0;
	supply_pid->err_2 = 0;
	
	supply_pid->err_kp = 0;
	supply_pid->err_ki = 0;
	supply_pid->err_kd = 0;
	
	supply_pid->err_ki_max = 10000;
	
	supply_pid->set_speed = 0;
	supply_pid->act_speed = 0;
	
	supply_pid->increment = 0;
	supply_pid->output = 0;

	supply_pid->calc = &supply_pid_calc;
}


void supply_pid_init(void)
{
	supply_pid.kp = 1.5;
	supply_pid.ki = 0.1;
	supply_pid.kd = 20;
	
	supply_pid_struct_init(&supply_pid);
}


U8 supply_speed_data[8];
void speed_update(s16 speed)
{
	CanTxMsg	canmsg;
	
	supply_speed_data[0] = speed>>8;
	supply_speed_data[1] = speed&0xff;
	
	//CAN2_SetMsg(0x1ff, supply_speed_data);
	//CAN_Transmit(CAN2, &TxMessage);
	
	can_set(&canmsg, 0x1ff, supply_speed_data);
	CAN_Transmit(CAN1, &canmsg);
}


//u16 Wheel_angle_expect = 0;
//u16 Wheel_angle_start = 0;
//u16 angle_now = 0;

//s16 angle_limit_l = 0;
//s16 angle_limit_h = 0;

//u8 change_flag = 0;

//u8 circle_count = 0;

//void supply_single_bullet(U8 count)
//{	
//	Wheel_angle_start = supply_read_angle;
//	angle_now = supply_read_angle;
//	
//	angle_limit_l = (s16)(Wheel_angle_start - 500);
//	angle_limit_h = (s16)(Wheel_angle_start + 500);
//	
//	if(angle_limit_l < 0)
//	{
//		angle_limit_l = 8191 + angle_limit_l;
//	}
//	if(angle_limit_h > 8191)
//	{
//		angle_limit_h = angle_limit_h - 8191;
//	}
//	
//	change_flag = 0;
//	
//	while(1)
//	{
//		//PRINT("%d\n", supply_read_angle);
//		//task_delay_ms(100);
//		if(angle_limit_l < angle_limit_h)
//		{
//			if((angle_limit_l < supply_read_angle)&&(supply_read_angle < angle_limit_h))
//			{
//				if(change_flag == 1)
//				{
//					////当之前转子不在界定范围内时
//					circle_count++;////圈数加一
//					angle_now = angle_now + 8191;
//					change_flag = 0;
//				}
//			}
//			else
//			{
//				if(change_flag == 0)
//				{
//					/////当之前轮子在界定范围的时候
//					change_flag = 1;
//				}
//			}
//		}
//		if(angle_limit_l > angle_limit_h)
//		{
//			if((supply_read_angle > angle_limit_l)||(supply_read_angle < angle_limit_h))
//			{
//				if(change_flag == 1)
//				{
//					////之前转子不在界定范围内时
//					circle_count++;////圈数加一
//					angle_now = angle_now + 8191;
//					change_flag = 0;
//				}
//			}
//			else
//			{
//				if(change_flag == 0)
//				{
//					/////当之前轮子在界定范围的时候
//					change_flag = 1;
//				}
//			}
//		}
//			
//		if(circle_count > 0)
//		{
//			if(circle_count%count == 0)
//			{
//				//supply_pid.set_speed = 0;
//				break;
//			}
//			else
//			{
//				supply_pid.set_speed = 500;
//			}
//			if(circle_count >= 36)
//			{
//				circle_count = 0;
//			}
//		}
//		PRINT("%d\n", circle_count);
//		//supply_pid.calc(&supply_pid);
//		speed_update(500);
//		task_delay_ms(0);
//	}
//	speed_update(0);
//	task_delay_ms(10);
//}



