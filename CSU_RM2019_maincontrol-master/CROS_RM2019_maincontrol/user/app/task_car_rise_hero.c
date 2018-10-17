#include "task_car_rise.h"
#include "can.h"
#include "task_remote.h"

#include "flash_data.h"


extern volatile int16_t rise_speed[2];
extern
U8 rise_speed_data[8];

U32	risef_test;
U32	riseb_test;

U32	car_rise_print = 0;
void	car_rise_print_control(U32 param)
{
	car_rise_print = param;
}

void	car_rise_msg(void* param)
{
	resolve_add("rise_print", car_rise_print_control);
	while(1)
	{
		
		if(car_rise_print){
			PRINT("fspeed: %d\r\n", rise_speed[0]);
			if(risef_test == LIFT_UP){
				PRINT("risef: up  ");
			}else if(risef_test == LIFT_DOWN){
				PRINT("risef: down  ");
			}else{
				PRINT("risef: no signal  ");
			}
			if(riseb_test == LIFT_UP){
				PRINT("risef: up\r\n");
			}else if(risef_test == LIFT_DOWN){
				PRINT("riseb: down\r\n");
			}else{
				PRINT("riseb: no signal\r\n");
			}
			PRINT("liftf circle: %d, b: %d\r\n", motor_circle[0][0].circle, motor_circle[0][1].circle);
		}
		task_delay_ms(1000);
	}
}



#include "mak_pid.h"
extern U8 strech_speed_data[8];
void task_car_rise_hero(void* param)
{
	CanTxMsg	canmsg;
	p_lift_ctrl	rise_data;
	mpid_t		rf_pid;
	mpid_t		rb_pid;
	S32		speedf = 0;
	S32		speedb = 0;
	S32		outputf;
	S32		outputb;
	
	mpid_init(&rf_pid, 2, 0, 0);
	mpid_init(&rb_pid, 2, 0, 0);
	
	rise_speed_data[0]	= 0;
	rise_speed_data[1]	= 0;
	rise_speed_data[2]	= 0;
	rise_speed_data[3]	= 0;
	rise_speed_data[4]	= 0;
	rise_speed_data[5]	= 0;
	while(1)
	{
		rise_data = msg_get_read_try(&lift_msg);
		if(rise_data){
			if(FD.car_id == CAR_ID_HERO){
				if(rise_data->front == LIFT_UP){
					if(motor_circle[0][0].circle > -350)
						speedf	= -5000;
					else
						speedf	= 0;
				}
				if(rise_data->front == LIFT_DOWN){
					if(motor_circle[0][0].circle < -10)
						speedf	= 4000;
					else
						speedf	= 0;
				}
			}else{
				if(rise_data->front == LIFT_UP){
					if(motor_circle[0][0].circle < 350)
						speedf	= 5000;
					else
						speedf	= 0;
				}
				if(rise_data->front == LIFT_DOWN){
					if(motor_circle[0][0].circle > 10)
						speedf	= -4000;
					else
						speedf	= 0;
				}
			}
			if(rise_data->back == LIFT_UP){
				if(motor_circle[0][1].circle < 350)
					speedb	= 5000;
				else
					speedb	= 0;
			}
			if(rise_data->back == LIFT_DOWN){
				if(motor_circle[0][1].circle > 10)
					speedb	= -4000;
				else
					speedb	= 0;
			}
			msg_read_finish_try(&lift_msg);
		}
		outputf	= mpid_v(&rf_pid, speedf, rise_speed[0]);
		outputb	= mpid_v(&rb_pid, speedb, rise_speed[1]);
		
		rise_speed_data[0]	= outputf>>8;
		rise_speed_data[1]	= outputf&0xff;
		rise_speed_data[2]	= outputb>>8;
		rise_speed_data[3]	= outputb&0xff;
		rise_speed_data[4] = strech_speed_data[4];
		rise_speed_data[5] = strech_speed_data[5];
		
		can_set(&canmsg, 0x200, rise_speed_data);
		CAN_Transmit(CAN1, &canmsg);
		
		
		task_delay_ms(10);
	}
}


