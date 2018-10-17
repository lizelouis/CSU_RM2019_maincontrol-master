#include "task_holder.h"
#include "task_remote.h"

#include "holder_pid.h"

#include "can.h"
#include "iic.h"

#include "flash_data.h"

void	iic_delay(void);

S16	holder_yaw_angle;
S16	holder_pitch_angle;

S16	holder_x_mpu	= 0;
S16	holder_y_mpu	= 0;
S16	holder_x_mpu_fix;
S16	holder_y_mpu_fix;

int16_t temp1 = 0;
int16_t temp2 = 0;
int16_t temp3 = 0;
int16_t temp4 = 0;


CanTxMsg	RM6025_set_data;


U32	holder_print		= 0;
U32	holder_print_mark	= 0;
void	holder_control(U32 param)
{
	if(param <= 2)
		holder_print	= param;
}

void	task_holder_control(void* param);

void	remote_data_dispose(p_remote_data tg);

S16	holder_x_init = -1;
S16	holder_y_init;
S16	holder_x_target;
S16	holder_y_target;
void	task_holder(void* param)
{
	S32		mpu_sum;
	size_t		i;
	p_holder_ctrl	data;
	
	/*{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		while(1)
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_8);
			GPIO_ResetBits(GPIOB,GPIO_Pin_9);
			//task_delay_ms(10);
			iic_delay();
			GPIO_SetBits(GPIOB,GPIO_Pin_8);
			GPIO_SetBits(GPIOB,GPIO_Pin_9);
			//task_delay_ms(10);
			iic_delay();
		}
			
	}*/
	
	
	task_delay_ms(1000);
	if(FD.car_id == CAR_ID_SOLDIER){
		holder_x_init	= holder_yaw_angle;
		holder_y_init	= 4100;
	}else if(FD.car_id == CAR_ID_HERO){
		holder_x_init	= holder_yaw_angle;
		holder_y_init	= 4100;
	}
	holder_x_target	= holder_x_init;
	holder_y_target	= holder_y_init;
	
	resolve_add("holder", holder_control);
	
	task_delay_ms(50);
	RM6025_set_data_init(&RM6025_set_data);
	
	//RM6025_set_pitch(0);
	//RM6025_set_yaw(0);
	
	IIC_Config();
	task_delay_ms(50);
	MPU6050_Init();
	
	mpu_sum	= 0;
	for(i=0; i<100; i++){
		mpu_sum	= mpu_sum - ((int16_t)(GetData(GYRO_ZOUT_H)) + 180);
		task_delay_ms(1);
	}
	holder_x_mpu_fix	= -(mpu_sum/100)/13.1072;
	mpu_sum	= 0;
	for(i=0; i<100; i++){
		mpu_sum	+= ((int16_t)(GetData(GYRO_YOUT_H)));
		task_delay_ms(1);
	}
	holder_y_mpu_fix	= -(mpu_sum/100)/13.1072;
	
	MAK_PRINT0("holder inited\r\n");
	
	task_delay_ms(50);
	task_insert(task_holder_control, NULL, 3);
	task_delay_ms(50);
	
	while(1)
	{
		data	= msg_get_read_try(&holder_msg);
		if(data){
			if(FD.car_id == CAR_ID_SOLDIER){
				holder_x_target  = -(S16)(data->mouse_x) + holder_x_init;
				if((S16)(data->mouse_y) >= 0){
					holder_y_target  = -(S16)(data->mouse_y)*7/10 + holder_y_init;
				}else{
					holder_y_target  = -(S16)(data->mouse_y)*7/5 + holder_y_init;
				}
			}else if(FD.car_id == CAR_ID_HERO){
				holder_x_target  = -(S16)(data->mouse_x)*2 + holder_x_init;
				holder_y_target  = (S16)(data->mouse_y)*11/5 + holder_y_init;
			}
			msg_read_finish_try(&holder_msg);
		}else{
			;
		}
		
		
		holder_x_mpu = -((int16_t)(GetData(GYRO_ZOUT_H)) + 180)/13.1072 + holder_x_mpu_fix;
		holder_y_mpu = (int16_t)(GetData(GYRO_YOUT_H))/13.1072  + holder_y_mpu_fix;
		
		
		if(holder_print && (system_clock%1000 < 50) && holder_print_mark)
		{
			MAK_PRINT2("holder:\r\ntarget: %d, %d\r\n", S16, holder_x_target, S16, holder_y_target);
			MAK_PRINT2("angle: %d, %d\r\n", S16, holder_yaw_angle, S16, holder_pitch_angle);
			MAK_PRINT2("mpu: %d, %d\r\n", S16, holder_x_mpu, S16, holder_y_mpu);
			PRINT("temp3: %d\r\n", temp3);
			if(holder_print == 2)
				holder_print	= 0;
			holder_print_mark	= 0;
		}
		if(system_clock%1000>100)
			holder_print_mark	= 1;
		
		task_delay_ms(10);
	}
}


PID_IncrementType	pid_holder_x_speed;
PID_AbsoluteType	pid_holder_x_position;
PID_IncrementType	pid_holder_y_speed;
PID_AbsoluteType	pid_holder_y_position;

void	task_holder_control(void* param)
{
	if(FD.car_id == CAR_ID_SOLDIER){
		holder_pid_init_absolute(&pid_holder_x_position, 6, 0.008, 0, 2000);
		holder_pid_init_increment(&pid_holder_x_speed, 3, 0.008, 0, 0);
		holder_pid_init_absolute(&pid_holder_y_position, 0.8, 0.005, 0, 2000);
		holder_pid_init_increment(&pid_holder_y_speed, 0.7, 0.005, 0, 1400);
	}else if(FD.car_id == CAR_ID_HERO){
		holder_pid_init_absolute(&pid_holder_x_position, 1.3, 0.001, 0, 2000);
		holder_pid_init_increment(&pid_holder_x_speed, 2.2, 0.004, 0, 0);
		holder_pid_init_absolute(&pid_holder_y_position, 0.5, 0.002, 0, 1170);
		holder_pid_init_increment(&pid_holder_y_speed, 0.5, 0.002, 0, 1400);
	}
	
	while(1)
	{	if(FD.car_id == CAR_ID_SOLDIER){
			temp4 = -PID_Update_Yaw(holder_y_target, holder_pitch_angle, &pid_holder_y_position);
			temp3 = -PID_IncrementMode_Yaw(temp4, holder_y_mpu, &pid_holder_y_speed);
		}else if(FD.car_id == CAR_ID_HERO){
			temp4 = PID_Update_Yaw(holder_y_target, holder_pitch_angle, &pid_holder_y_position);
			temp3 = PID_IncrementMode_Yaw(temp4, holder_y_mpu, &pid_holder_y_speed);
		}
		temp2 = PID_Update_Yaw(holder_x_target,holder_yaw_angle,&pid_holder_x_position);
		temp1 = PID_IncrementMode_Yaw(temp2,holder_x_mpu,&pid_holder_x_speed);
		
		//temp3 = -1500;
		
		RM6025_set_yaw_pitch(temp1, temp3);
		
		task_delay_ms(5);
	}
}



void	RM6025_set_data_init(CanTxMsg* data)
{
        data->StdId	= 0x1ff;
        data->IDE	= CAN_ID_STD;
        data->RTR	= CAN_RTR_DATA;
        data->DLC	= 8;
        data->Data[0]	= 0x00;
        data->Data[1]	= 0x00;
        data->Data[2]	= 0x00;
        data->Data[3]	= 0x00;
        data->Data[4]	= 0x00;
        data->Data[5]	= 0x00;
        data->Data[6]	= 0x00;
        data->Data[7]	= 0x00;
}

U32	RM6025_set_yaw(S16 yaw)
{
	if((yaw>((S16)5000)) || (yaw<((S16)(-5000))))
		return	0;
	
        RM6025_set_data.Data[0]	= (U8)(yaw>>8);
        RM6025_set_data.Data[1]	= (U8)(yaw&0xff);
	
        CAN_Transmit(CAN2, &RM6025_set_data);
	
	return	1;
}

U32	RM6025_set_pitch(S16 pitch)
{
	if((pitch>((S16)5000)) || (pitch<((S16)(-5000))))
		return	0;
	
        RM6025_set_data.Data[2]	= (U8)(pitch>>8);
        RM6025_set_data.Data[3]	= (U8)(pitch&0xff);
	
        CAN_Transmit(CAN2, &RM6025_set_data);
	
	return	1;
}

U32	RM6025_set_yaw_pitch(S16 yaw, S16 pitch)
{
	if((yaw>((S16)5000)) || (yaw<((S16)(-5000))))
		return	0;
	if((pitch>((S16)5000)) || (pitch<((S16)(-5000))))
		return	0;
	
	
        RM6025_set_data.Data[0]	= (U8)(yaw>>8);
        RM6025_set_data.Data[1]	= (U8)(yaw&0xff);
        RM6025_set_data.Data[2]	= (U8)(pitch>>8);
        RM6025_set_data.Data[3]	= (U8)(pitch&0xff);
	
        CAN_Transmit(CAN2, &RM6025_set_data);
	
	return	1;
}



