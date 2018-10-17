#ifndef _MOTOR_H
#define _MOTOR_H

#include "makos_type.h"
#include "pid.h"
#include "can.h"

extern msg_t	chassis_msg;  
//*******************************************				
extern U8 speed_data[8];

extern int16_t chassis_V_T[4];
extern int16_t chassis_V_C[4];
extern int16_t CAN_data[4];//CAN发送电流
extern s16 current_T[4];
extern s16 current_C[4];
extern u16 Driver_ID[4];
extern float GAIN_I;
extern float	GAIN_J;
extern float	GAIN_K;
//***********************************************************************

//*************************************************************************************
void motor_cmd(U32 ID,S16 speed);//驱动指定ID电机(不带控制)
//******************
void chassis_move(S32	front,S32	right,S32	clockwise);//底盘电机移动
void chassis_stop(void);//底盘电机停止
//*************************************************************************************
void remote_0_protec(int16_t* data,s32 max);

#endif
