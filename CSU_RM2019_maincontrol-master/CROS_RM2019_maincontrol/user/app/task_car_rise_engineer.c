#include "task_car_rise.h"
#include "can.h"
#include "task_remote.h"

supply_pid_struct rise_pid[2];

void car_rise_init(void)
{
	int i = 0;
	for(i = 0; i < 2; i++)
	{
		rise_pid[i].kp = 2;
		rise_pid[i].ki = 0.1;
		rise_pid[i].kd = 0;
		

		rise_pid[i].err_0 = 0;
		rise_pid[i].err_1 = 0;
		rise_pid[i].err_2 = 0;
		
		rise_pid[i].err_kp = 0;
		rise_pid[i].err_ki = 0;
		rise_pid[i].err_kd = 0;
		
		rise_pid[i].err_ki_max = 10000;
		
		rise_pid[i].set_speed = 0;
		rise_pid[i].act_speed = 0;
		
		rise_pid[i].increment = 0;
		rise_pid[i].output = 0;

		rise_pid[i].calc = &supply_pid_calc;
	}
}


u8 rise_act[2] = {0};

void car_rise(U32 param)
{
//	if(param == 1)	///前升
//	{
//		rise_pid[0].set_speed = 7000;
//		rise_act[0] = 1;
//	}
//	if(param == 2)	///前降
//	{
//		rise_pid[1].set_speed = 7000;
//		rise_act[1] = 1;
//	}
//	if(param == 3)	///后升
//	{
//		rise_pid[0].set_speed = -7000;
//		rise_act[0] = 1;
//	}
//	if(param == 4)	///后降
//	{
//		rise_pid[1].set_speed = -7000;
//		rise_act[1] = 1;
//	}
}


extern U8 strech_speed_data[8];
extern volatile int16_t rise_speed[2];
U8 rise_speed_data[8];
s16 expect_cricle[2] = {0};
void car_rise_motor(void)
{
	CanTxMsg	canmsg;
	
	if(expect_cricle[0] == 0)
	{
		if(motor_circle[0][0].circle > expect_cricle[0])
		{
			rise_pid[0].set_speed = -5000;
		}
		if(motor_circle[0][0].circle <= expect_cricle[0])
		{
			rise_pid[0].set_speed = 0;
		}
	}
	if(expect_cricle[0] == 800)
	{
		if(motor_circle[0][0].circle < expect_cricle[0])
		{
			rise_pid[0].set_speed = 5000;
		}
		if(motor_circle[0][0].circle >= expect_cricle[0])
		{
			rise_pid[0].set_speed = 0;
		}
	}
	
	if(expect_cricle[1] == 0)
	{
		if(motor_circle[0][1].circle > expect_cricle[1])
		{
			rise_pid[1].set_speed = -5000;
		}
		if(motor_circle[0][1].circle <= expect_cricle[1])
		{
			rise_pid[1].set_speed = 0;
		}
	}
	if(expect_cricle[1] == 1000)
	{
		if(motor_circle[0][1].circle < expect_cricle[1])
		{
			rise_pid[1].set_speed = 5000;
		}
		if(motor_circle[0][1].circle >= expect_cricle[1])
		{
			rise_pid[1].set_speed = 0;
		}
	}
	
	rise_pid[0].act_speed = rise_speed[0];
	rise_pid[1].act_speed = rise_speed[1];
	
	rise_pid[0].calc(&rise_pid[0]);
	rise_pid[1].calc(&rise_pid[1]);
	
	rise_speed_data[0] = rise_pid[0].output>>8;
	rise_speed_data[1] = rise_pid[0].output&0xff;
	rise_speed_data[2] = rise_pid[1].output>>8;
	rise_speed_data[3] = rise_pid[1].output&0xff;
	rise_speed_data[4] = strech_speed_data[4];
	rise_speed_data[5] = strech_speed_data[5];
	
	can_set(&canmsg, 0x200, rise_speed_data);
	CAN_Transmit(CAN1, &canmsg);
}

//void	car_rise_msg(void*);

//U32	risef_test;
//U32	riseb_test;
///*
void task_car_rise_engineer(void* param)
{
	CanTxMsg	canmsg;
	p_lift_ctrl rise_data;
	
	car_rise_init();
	resolve_add("car_rise", car_rise);
	
	//task_insert(car_rise_msg, NULL, 4);
	
	while(1)
	{
		rise_data = msg_get_read_try(&lift_msg);
		if(rise_data){
			//risef_test = rise_data->front;
			//riseb_test = rise_data->back;
			
			if(rise_data->front == LIFT_UP){
				expect_cricle[0] = 800;
			}
			if(rise_data->front == LIFT_DOWN){
				expect_cricle[0] = 0;
			}
			if(rise_data->back == LIFT_UP){
				expect_cricle[1] = 1000;
			}
			if(rise_data->back == LIFT_DOWN){
				expect_cricle[1] = 0;
			}
			msg_read_finish_try(&lift_msg);
		}
		car_rise_motor();
		task_delay_ms(10);
	}
}

//U32	car_rise_print = 0;
//void	car_rise_print_control(U32 param)
//{
//	car_rise_print = param;
//}

//void	car_rise_msg(void* param)
//{
//	resolve_add("rise_print", car_rise_print_control);
//	while(1)
//	{
//		
//		if(car_rise_print){
//			PRINT("fspeed: %d\r\n", rise_speed[0]);
//			if(risef_test == LIFT_UP){
//				PRINT("risef: up  ");
//			}else if(risef_test == LIFT_DOWN){
//				PRINT("risef: down  ");
//			}else{
//				PRINT("risef: no signal  ");
//			}
//			if(riseb_test == LIFT_UP){
//				PRINT("risef: up\r\n");
//			}else if(risef_test == LIFT_DOWN){
//				PRINT("riseb: down\r\n");
//			}else{
//				PRINT("riseb: no signal\r\n");
//			}
//			PRINT("liftf circle: %d, b: %d\r\n", motor_circle[0][0].circle, motor_circle[0][1].circle);
//		}
//		task_delay_ms(1000);
//	}
//}



//#include "task_shoot.h"
//extern U8 strech_speed_data[8];
//void task_car_rise(void* param)
//{
//	CanTxMsg	canmsg;
//	p_lift_ctrl	rise_data;
//	mpid_t		rf_pid;
//	mpid_t		rb_pid;
//	S32		speedf = 0;
//	S32		speedb = 0;
//	S32		outputf;
//	S32		outputb;
//	
//	mpid_init(&rf_pid, 2, 0, 0);
//	mpid_init(&rb_pid, 2, 0, 0);
//	
//	rise_speed_data[0]	= 0;
//	rise_speed_data[1]	= 0;
//	rise_speed_data[2]	= 0;
//	rise_speed_data[3]	= 0;
//	while(1)
//	{
//		rise_data = msg_get_read_try(&lift_msg);
//		if(rise_data){
//			if(rise_data->front == LIFT_UP){
//				if(motor_circle[0][0].circle > -350)
//					speedf	= -5000;
//				else
//					speedf	= 0;
//			}
//			if(rise_data->front == LIFT_DOWN){
//				if(motor_circle[0][0].circle < -10)
//					speedf	= 4000;
//				else
//					speedf	= 0;
//			}
//			if(rise_data->back == LIFT_UP){
//				if(motor_circle[0][1].circle < 350)
//					speedb	= 5000;
//				else
//					speedb	= 0;
//			}
//			if(rise_data->back == LIFT_DOWN){
//				if(motor_circle[0][1].circle > 10)
//					speedb	= -4000;
//				else
//					speedb	= 0;
//			}
//			msg_read_finish_try(&lift_msg);
//		}
//		outputf	= mpid_v(&rf_pid, speedf, rise_speed[0]);
//		outputb	= mpid_v(&rb_pid, speedb, rise_speed[1]);
//		
//		rise_speed_data[0]	= outputf>>8;
//		rise_speed_data[1]	= outputf&0xff;
//		rise_speed_data[2]	= outputb>>8;
//		rise_speed_data[3]	= outputb&0xff;
//		rise_speed_data[4] = strech_speed_data[4];
//		rise_speed_data[5] = strech_speed_data[5];
//		
//		can_set(&canmsg, 0x200, rise_speed_data);
//		CAN_Transmit(CAN1, &canmsg);
//		
//		
//		task_delay_ms(10);
//	}
//}

