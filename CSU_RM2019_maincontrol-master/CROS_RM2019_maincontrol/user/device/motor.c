#include "motor.h"
#include "can.h"

/*****************************************************************************/
//**********底盘电机ID***********
U16 Driver_ID[4] = {0x201,0x202,0x203,0x204};
//******************************************
U8 speed_data[8];

int16_t chassis_V_T[4] = {0};//目标速度,依次对应HL,HR,BL,BR
int16_t chassis_V_C[4] = {0};//实际速度,依次对应HL,HR,BL,BR
float 	chassis_V_K[4] = {0};
int16_t current_C[4] = {0};//实际电流,依次对应HL,HR,BL,BR
int16_t current_T[4] = {0};//目标电流,依次对应HL,HR,BL,BR
int16_t CAN_data[4] = {0};//CAN发送数据,依次对应HL,HR,BL,BR
//***********************************************************************
/*------------------------------------------------------------------------*/

//*****************control one motor act by id*********************

void motor_data_clear()
{
	u32 i;
	for(i = 0;i < 8;i++)
	{
		speed_data[i] = 0;
	}
}

void motor_cmd(U32 ID,S16 speed)
{
	
	u32 i;
	CanTxMsg	canmsg;

	motor_data_clear();

	for(i = 0;i < 4;i++)
	{
		if(Driver_ID[i] == ID)
		{
			speed_data[2*i] = (U8)(speed>>8);
			speed_data[2*i + 1] = (U8)(speed&0xff);
			break;
		}
	}
	can_set(&canmsg, Control_ID, speed_data);
	CAN_Transmit(CAN2, &canmsg);
}

//*********************底盘整体控制************************

void send_can()
{
	CanTxMsg	canmsg;
	u32 i;
	
	for(i = 0; i < 4;i++)
	{
		speed_data[2*i] = (char)((CAN_data[i])>>8);
		speed_data[(2*i) + 1] = (char)((CAN_data[i])>>8);
	}
	
	can_set(&canmsg, Control_ID,speed_data);//标记次消息ID为主控ID(来源于主控),将速度的目标值发送给驱动板,该信息来源于PC端
	CAN_Transmit(CAN2,&canmsg);	
}

//***********************motor control rule************************

void remote_0_protec(int16_t* data,s32 max)
{
	if((*data < max) && (*data > -max))
	{
		*data = 0;
	}
}


void set_current_symbol()
{
	u32 i;
	for(i = 0;i < 4;i++)
	{
		if(CAN_data[i] < 0)
		{
			current_C[i] = - current_C[i];
		}
	}
}

float GAIN_I = 0.1;
float	GAIN_J = 0.1;
float	GAIN_K = 0.2;
void chassis_move(S32	vx,S32	vy,S32	vr)
{			
		u32 i;
	
		//********************速度解算**********************
		//chassis_V_T[0]	= (int)(vx/GAIN_I		-		vy/GAIN_J	+	 vr/GAIN_K);
		chassis_V_T[0]	= (int)(vx/GAIN_I		-		vy/GAIN_J	+	 vr/GAIN_K);
		chassis_V_T[1]	= (int)(vx/GAIN_I		+		vy/GAIN_J	+	 vr/GAIN_K);
		chassis_V_T[2]	= (int)(-vx/GAIN_I	-		vy/GAIN_J	+	 vr/GAIN_K);
		chassis_V_T[3]	= (int)(-vx/GAIN_I	+		vy/GAIN_J	+	 vr/GAIN_K);	  	
		//**************************************************	
	
		set_current_symbol();
	
		for(i = 0;i < 4;i++)
		{
			//*********遥控零点信息保护**********	
			remote_0_protec(&chassis_V_T[i],30);
			//********************控制************************** 
			//kalman滤波 
			//chassis_V_K[i] = KalmanFilter_HL(chassis_V_C[i],KF_Q,KF_R,0);			
			//速度环
			current_T[i] = (s16)PID_Update_Absolute2(&Pid_speed[i],(float)chassis_V_T[i],(float)chassis_V_C[i]);	
			//电流环
			CAN_data[i] = (s16)PID_Update_Incre(&Pid_current[i],(float)current_T[i],current_C[i]);			
			//**************************************************		
		}
		send_can();
}

void chassis_stop()
{
	chassis_move(0 ,0 ,0 );
}

//********************************************************************
