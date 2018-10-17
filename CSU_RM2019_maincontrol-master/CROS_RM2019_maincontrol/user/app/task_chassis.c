/*
	FILE NAME:	task_chassis.c
	DESCRIPTION:	see by yourself
	EDIT RECORDS:
	---------
	AUTHOR:    
	EDIT TIME:	2018/4/13-17:00
	MODIFICATION: 
	---------
	AUTHOR:		
	EDIT TIME:	
	MODIFICATION:	
	---------

??:
1.PID??
??:
    kp(param)
		k = param/10
		id????
		
		pid_show(param)  ????PID??
		
2.????:
??:
		wave_key(param)
		param = 1   send speed_wave
		param = 0   stop send
		
*/
#include "task_chassis.h"
#include "task_holder.h"
#include "task_remote.h"
#include "flash_data.h"
#include "usart.h"
#include "adc.h"
#include "motor.h"
#include "pid.h"
#include "math.h"


//******************** modi the param from the upper **********************
void set_P(float data)
{
//	Pid_speed[0].kp = data;
//	Pid_speed[1].kp = data;
//	Pid_speed[2].kp = data;
//	Pid_speed[3].kp = data;
}

void set_I(float data)
{
//	Pid_speed[0].ki = data;
//	Pid_speed[1].ki = data;
//	Pid_speed[2].ki = data;
//	Pid_speed[3].ki = data;
}

void set_D(float data)
{
//	Pid_speed[0].kd = data;
//	Pid_speed[1].kd = data;
//	Pid_speed[2].kd = data;
//	Pid_speed[3].kd = data;
}

void set_err_lim(float data)
{
//	Pid_speed[0].errILim = data;
//	Pid_speed[1].errILim = data;
//	Pid_speed[2].errILim = data;
//	Pid_speed[3].errILim = data;
}

void upper_monit_kp(U32 param)
{
	float tmp;
	tmp = (float)param;
	tmp = param*(0.1);
	
	PRINT("now kp is %.3f",tmp);
	
	set_P(tmp);
}

void upper_monit_ki(U32 param)
{
	float tmp;
	tmp = (float)param;
	tmp = param*(0.1);
	
	PRINT("now ki is %.3f",tmp);
	
	set_I(tmp);
}

void upper_monit_kd(U32 param)
{
	float tmp;
	tmp = (float)param;
	tmp = param*(0.1);
	
	PRINT("now kd is %.3f",tmp);
	
	set_D(tmp);
}

float err_lim = 3000;
void upper_monit_err_lim(U32 param)
{
	float tmp;
	tmp = (float)param;
	tmp = param*(0.1);
	
	PRINT("now err_lim is %.3f",tmp);
	
	set_err_lim(tmp);
}

extern float v_k;
void pid_show(U32 param)
{
		PRINT("-----------------------------\r\n");
//		PRINT("kp: %.3f \r\n",Pid_speed[0].kp);
//		PRINT("ki: %.3f \r\n",Pid_speed[0].ki);
//		PRINT("kd: %.3f \r\n",Pid_speed[0].kd);
		PRINT("kd: %.3f \r\n",v_k);
		//PRINT("kd: %.3f \r\n",Pid_speed[0].errILim);
		PRINT("-----------------------------\r\n");
}


void v_k_func(U32 param)
{
	if(param == 0)
		printf("now the v_k is %f\r\n",v_k);
	else
	{
		v_k = param * (0.1);
		printf("now the v_k is %f\r\n",v_k);	
	}
}

//********************************************************************

U32	speed_show_G = 0;

void speed_show_set(U32 param)
{
	speed_show_G = param;
}

u8 send_wave_flag = 0;
u8 wave_open_G = 0;
void wave_key(U32 param)
{
	wave_open_G = param;
}


void print_chassis_speed()
{
	u32 i;
	PRINT("-------------speed---------------\r\n");
	for(i = 0;i < 4;i++)
	{
		PRINT("%d: %d \r\n",i+1,chassis_V_C[i]);
	}	
}

void print_current()
{
	u32 i;	
	PRINT("-------------current---------------\r\n");
	for(i = 0;i < 4;i++)
	{
		PRINT("%d: %d \r\n",i+1,current_C[i]);
	}
}

S32 remote_data_proc(S32 data)
{
		if(data < -1024)
			data	= -1024;
		else if(data > 1023)
			data	= 1023;			
		
		data = (S32)(((float)data)*v_k);
		
		return data;
}

S32 is_2s_interval()
{
 if(system_clock%2000 < 50)
 {
	return 1;
 }	 
 else
 {
	return 0;
 }	  
}

//#define CMD_WARE     3
//U8	curve_cmd1 = CMD_WARE;
//U8	curve_cmd2 = ~CMD_WARE;

void send_speed_wave(u32 i)
{   
	
		//**************???????**************	
		#define CMD_WARE     3
    uint8_t cmdf[2] = {CMD_WARE, ~CMD_WARE};   
    uint8_t cmdr[2] = {~CMD_WARE, CMD_WARE};    

    usart3_send_string(cmdf, sizeof(cmdf));    
 	
    usart3_send_string((uint8_t *)&(chassis_V_C[i]), 2);    
    usart3_send_string((uint8_t *)&(chassis_V_T[i]), 2);    
		usart3_send_string((uint8_t *)&(current_T[i]), 2);
		usart3_send_string((uint8_t *)&(current_C[i]), 2);
		usart3_send_string((uint8_t *)&(CAN_data[i]), 2);
		
    usart3_send_string(cmdr, sizeof(cmdr));    
//******************************************
}


U32 adc_print_G = 0;
void adc_print(U32 param)
{
	adc_print_G = param;
}
#define CURRENT_BASE 30
void ad_filter(s16 delay_time) 
{    
	int sum[4] = {0};
	int AD_tmp[4] = {0};
	u32 count,i;
	for(count = 0;count < N;count++) 
	{    
		for(i = 0;i < 4;i++)
		{
			sum[i] += uhADCxConvertedValue[i];
		}
		while(delay_time)
		{
			delay_time--;
		}
	} 
	
	for(i = 0;i < 4;i++)
	{
		AD_tmp[i] = sum[i]/N;
		current_C[i] = (AD_tmp[i] * 3300/0xFFF - CURRENT_BASE)/2;
		//current_C[i] = uhADCxConvertedValue[i];
		//remote_0_protec(&current_C[i],30);
	}		

}


#define HOLDER_MID 550

extern
S16	holder_x_init;


S32 L_start_G = 0;
S32 R_start_G = 0;
S32 tick_judg = 0;
float v_k = 0.7;
void	task_chassis(void* param)
{	
	u32 time1,time2;	
	double angle;
	
	p_chassis_ctrl	data;
	
	S32 front;
	S32 right;	
	
	S32 front_temp;
	S32 right_temp;		
	
	S32 clockwise;
	S32 speed;
	
	/* ADC configuration */
  ADC_Config();
  /* Start ADC Software Conversion */ 
	ADC_SoftwareStartConv(ADC1);
	resolve_add("adc", adc_print);
	resolve_add("speed", speed_show_set);
	
	//************pid????**************
	resolve_add("kp", upper_monit_kp);	
  resolve_add("ki", upper_monit_ki);
	resolve_add("kd", upper_monit_kd);
	resolve_add("err_lim", upper_monit_err_lim);
	resolve_add("wave_key", wave_key);
	resolve_add("pid_show",pid_show);
	//***????***
	resolve_add("v_k",v_k_func);
	//*************************************
	
	if(FD.car_id == CAR_ID_ENGINEER)
	{
		PID_speed_Init(1.5 ,0.01 ,1.1,1200);
		//PID_speed_Init(1.5 ,0.01 ,0,1000);
		PID_current_Init(1.5,0.2,0.0,9000);			
	}
	else
	{
		PID_speed_Init(1.5 ,0.01 ,1.1,1000);
		//PID_speed_Init(1.5 ,0.01 ,0,1000);
		PID_current_Init(1.5,0.2,0.0,6000);	
	}
	PID_Struct_Init_abs(&PID_follow,1.1,0.0,0.0,5000);
	
	//*****防止holder_x_init设置不及时*****
	
//	if(FD.car_id == CAR_ID_SOLDIER || FD.car_id == CAR_ID_HERO){
//		while(holder_x_init == -1)
//			task_delay_ms(0);
//	}
	//*************************************
		
	PRINT("chassis init ok\r\n");
	
	//angle = sin(60*3.14/180);

	//printf("angle:%f\r\n",angle);
//	PRINT("p:%.3f\r\n",Pid_speed[0].kp);
//	PRINT("i:%.3f\r\n",Pid_speed[0].ki);
//	PRINT("d:%.3f\r\n",Pid_speed[0].kd);	

	while(1)
	{
//		motor_cmd(0x201,20000);//????ID??(????)
//		motor_cmd(0x202,2000);//????ID??(????)
//		motor_cmd(0x203,5000);//????ID??(????)
//		motor_cmd(0x204,20000);//????ID??(????)		
		
		
		data	= msg_get_read_try(&chassis_msg);//????????
		
		//time1 = system_clock;
		
		if(data)//???????
		{	
			if(FD.car_id == CAR_ID_ENGINEER)
			{
				front = -remote_data_proc(data->front);
				right = remote_data_proc(data->right);
				clockwise = remote_data_proc(data->clockwise);				
			}
			else
			{
				if(data->mode == CHASSIS_FOLLOW)
				{
					front = -remote_data_proc(data->front);
					right = remote_data_proc(data->right);
					
					angle = ((double)(holder_x_init - holder_yaw_angle))/18;
					
					if((angle > 15) || (angle < -15))
					{
						clockwise = (int)(angle * 12.0);
						//clockwise = 20;
					}
//					if((holder_x_init - holder_yaw_angle) > 100)
//					{
//						clockwise = 240;
//					}
//					else if((holder_x_init - holder_yaw_angle) < -100)
//					{
//						clockwise = -240;
//					}
					else
					{
						clockwise = 0;
					}
					//clockwise = remote_data_proc(data->clockwise);	
				}			
//				else if(data->mode == CHASSIS_NOHEAD)
//				{
//					//front_temp = -remote_data_proc(data->front);
//					front_temp = remote_data_proc(data->front);
//					right_temp = remote_data_proc(data->right);
//					angle = ((double)(holder_x_init - holder_yaw_angle))/18;		
//					
//					front = -(int)(((float)front_temp)*((float)cos(angle*3.14/180))) + (int)(((float)right_temp)*((float)sin(angle*3.14/180)));
//					right = ((int)(((float)front_temp)*((float)sin(angle*3.14/180))) + (int)(((float)right_temp)*((float)cos(angle*3.14/180))));	
//					
//					if(angle > 35)
//					{
//						clockwise = 300;
//					}
//					else if(angle < -35)
//					{
//						clockwise = -300;
//					}
//					else 
//					{
//						clockwise = 0;
//					}
//				}		
				else
				{
					front = -remote_data_proc(data->front);
					right = remote_data_proc(data->right);
					//clockwise = 0;
					clockwise = (S32)((1.5)*((float)(remote_data_proc(data->clockwise))));

					//angle = ((double)(holder_x_init - holder_yaw_angle))/18;
					
//					if((angle > 40)&&(L_start_G == 0))
//					{
//						L_start_G = 1;
//					}
//					else if((angle < -40)&&(R_start_G == 0))
//					{
//						R_start_G = 1;
//					}		
//					if(L_start_G == 1)
//					{
//						clockwise = 300;
//						tick_judg++;
//						if((angle < 30) && (tick_judg > 50))
//						{
//							L_start_G = 0;
//							tick_judg = 0;
//							clockwise = 0;
//						}
//					}else if(R_start_G == 1)
//					{
//						clockwise = -300;
//						tick_judg++;
//						if((angle > -30) && (tick_judg > 50))
//						{
//							R_start_G = 0;
//							tick_judg = 0;
//							clockwise = 0;
//						}						
//					}
				}			
			}
	
			msg_read_finish_try(&chassis_msg);
			
      chassis_move(right ,front ,clockwise );	

			send_wave_flag = SEND;
		}
		else//??????
		{		
			chassis_stop();
			send_wave_flag = NO;
		}
		//******************??????*********************
		if(speed_show_G && (is_2s_interval() == YES))//??2s??
		{
			print_chassis_speed();//??????,??
		}
		//******************?????????***************
		if(send_wave_flag && wave_open_G)
		{
			send_speed_wave(0);
		}
		else
		{
			task_delay_ms(10);
		}
		//***********????************
		ad_filter(10);
		//***********??????********
		if(adc_print_G && is_2s_interval())//??2s??
		{
			print_current();
		}		
		//*******************************

		task_delay_ms(12);	
		
		time2 = system_clock;		
		//PRINT("T:%d",time2 - time1);					

	}	
}
