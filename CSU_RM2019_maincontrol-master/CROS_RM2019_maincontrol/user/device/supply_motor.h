#ifndef _SUPPLY_MOTOR
#define _SUPPLY_MOTOR

#include "makos_includes.h"
#include "can.h"

typedef struct supply_pid_struct
{
	float kp;
	float ki;
	float kd;
	
	s16 err_0;
	s16 err_1;
	s16 err_2;
	
	s16 err_kp;
	s16 err_ki;
	s16 err_kd;
	
	s16 err_ki_max;
	
	s16 set_speed;
	s16 act_speed;
	
	float increment;
	s16 output;
	
	void(*calc)(struct supply_pid_struct *supply_pid);
} supply_pid_struct;


typedef struct bullet_supply_struct
{
	u8 motor_state;
	
	u16 start_angle;
	u16 current_angle;
	s16 count_angle_low;
	s16 count_angle_high;
	u8 count_judge_type;
	
	u8 circle_count;
	u8 change_flag;
	
	u8 reverse_time;
	s16 move_speed;
	s16 reverse_speed;
	
	u16 move_angle;
	
	u8 act_flag;
	
	void(*reset)(struct bullet_supply_struct *bullet_supply);
	void(*count)(struct bullet_supply_struct *bullet_supply);
	void(*move)(struct bullet_supply_struct *bullet_supply);
	
} bullet_supply_struct;


extern bullet_supply_struct bullet_supply;
extern supply_pid_struct supply_pid;


void bullet_supply_init(struct bullet_supply_struct *bullet_supply);
void one_bullet_supply(u8 angle);
void continue_bullet_supply(u8 deal);



void supply_pid_init(void);
void supply_single_bullet(U8 angle);
void speed_update(s16 speed);

void supply_pid_calc(struct supply_pid_struct *supply_pid);

#endif
