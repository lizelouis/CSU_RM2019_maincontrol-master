/*
	FILE NAME:	task_shoot.c
	DESCRIPTION:	control the soldier and hero's shooting.
			left click to single shoot.
			right click to multi shoot.
	
	EDIT RECORDS:
	---------
	AUTHOR:		FAN YAOWEN
	EDIT TIME:	2018/3/31
	MODIFICATION:	the shoot_msg tested
*/

#include "task_shoot.h"
#include "task_remote.h"

#include "flash_data.h"

#include "supply_motor.h"

#include "can.h"
#include "tim5.h"


#include "mak_pid.h"



U32	shoot_test	= 0;
void	shoot_test_control(U32 param)
{
	shoot_test = 1;
}

U32	shoot_print		= 0;
U32	shoot_print_mark	= 0;
void	shoot_control(U32 param)
{
	if(param < 3)
		shoot_print = param;
}

static
S32		circle = 0;

#define	SHOOT_CIRCLE	motor_circle[0][4].circle

void	task_shoot_control(void* param);
void	task_shoot(void* param)
{
	p_shoot_ctrl	data;
	U32		shoot;
	U32		delta;
	
	resolve_add("shoot", shoot_control);
	resolve_add("shoot_test", shoot_test_control);
	
	if(FD.car_id == CAR_ID_SOLDIER){
		delta	= 6;
		{
			TIM_TimeBaseInitTypeDef tim;
			GPIO_InitTypeDef gpio;
			TIM_OCInitTypeDef oc;
			
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
			
			GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
			GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);
			GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM5);
			GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM5);
			
			gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
			gpio.GPIO_Mode = GPIO_Mode_AF;
			gpio.GPIO_OType = GPIO_OType_PP;
			gpio.GPIO_Speed = GPIO_Speed_100MHz;
			gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
			GPIO_Init(GPIOA, &gpio);
			
			TIM_DeInit(TIM5);

			tim.TIM_Prescaler = 84 - 1;
			tim.TIM_CounterMode = TIM_CounterMode_Up;
			tim.TIM_ClockDivision = TIM_CKD_DIV1;
			tim.TIM_Period = 20000;//20ms
			TIM_TimeBaseInit(TIM5, &tim);

			oc.TIM_OCMode = TIM_OCMode_PWM2;
			oc.TIM_OutputState = TIM_OutputState_Enable;
			oc.TIM_OutputNState = TIM_OutputState_Disable;
			oc.TIM_Pulse =0;
			oc.TIM_OCPolarity = TIM_OCPolarity_Low;
			oc.TIM_OCNPolarity = TIM_OCPolarity_High;
			oc.TIM_OCIdleState = TIM_OCIdleState_Reset;
			oc.TIM_OCNIdleState = TIM_OCIdleState_Set;

			TIM_OC1Init(TIM5, &oc);
			TIM_OC2Init(TIM5, &oc);
			TIM_OC3Init(TIM5, &oc);
			TIM_OC4Init(TIM5, &oc);
			
			TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);
			TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);
			TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);
			TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);
			TIM_CtrlPWMOutputs(TIM5, ENABLE);
			TIM_ARRPreloadConfig(TIM5, ENABLE);
			TIM_Cmd(TIM5, ENABLE);
			
			TIM5->CCR1=1000;
			TIM5->CCR2=1000;
			TIM5->CCR3=1000;
			TIM5->CCR4=1000;
			task_delay_ms(10000);
			TIM5->CCR1=1700;
			TIM5->CCR2=1700;
			TIM5->CCR3=1700;
			TIM5->CCR4=1700;
		}
	}else if(FD.car_id == CAR_ID_HERO){
		delta	= 9;
		{
			TIM_TimeBaseInitTypeDef tim;
			GPIO_InitTypeDef gpio;
			TIM_OCInitTypeDef oc;
			
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
			
			GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
			GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);
			GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM5);
			
			gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3;   //¹Ü½ÅÉèÖÃ TIM2_CH2;CH3;CH4
			gpio.GPIO_Mode = GPIO_Mode_AF;
			gpio.GPIO_OType = GPIO_OType_PP;
			gpio.GPIO_Speed = GPIO_Speed_100MHz;
			gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
			GPIO_Init(GPIOA, &gpio);
			TIM_DeInit(TIM5);

			tim.TIM_Prescaler = 84 - 1;
			tim.TIM_CounterMode = TIM_CounterMode_Up;
			tim.TIM_ClockDivision = TIM_CKD_DIV1;
			tim.TIM_Period = 20000;//20ms
			TIM_TimeBaseInit(TIM5, &tim);

			oc.TIM_OCMode = TIM_OCMode_PWM2;
			oc.TIM_OutputState = TIM_OutputState_Enable;
			oc.TIM_OutputNState = TIM_OutputState_Disable;
			oc.TIM_Pulse =0;
			oc.TIM_OCPolarity = TIM_OCPolarity_Low;
			oc.TIM_OCNPolarity = TIM_OCPolarity_High;
			oc.TIM_OCIdleState = TIM_OCIdleState_Reset;
			oc.TIM_OCNIdleState = TIM_OCIdleState_Set;

			TIM_OC1Init(TIM5, &oc);
			TIM_OC2Init(TIM5, &oc);
			TIM_OC4Init(TIM5, &oc);
			
			TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);
			TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);
			TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);
			TIM_CtrlPWMOutputs(TIM5, ENABLE);
			TIM_ARRPreloadConfig(TIM5, ENABLE);
			TIM_Cmd(TIM5, ENABLE);
			
			TIM5->CCR1=1000;
			TIM5->CCR2=1000;
			task_delay_ms(10000);
			TIM5->CCR1=1700;
			TIM5->CCR2=1700;
		}
	}else{
		PRINT_ERROR("this car cant shoot");
	}
	
	task_insert(task_shoot_control, NULL, 3);
	
	while(1)
	{
		data	= msg_get_read_try(&shoot_msg);
		if(data)
		{
			shoot = data->shoot;
			msg_read_finish_try(&shoot_msg);
			switch(shoot)
			{
				case SHOOT_HOLD:
					//shoot_motor_stop();
					break;
				case SHOOT_SINGLE:
					if(FD.car_id == CAR_ID_SOLDIER)
						circle += delta;
					else if(FD.car_id == CAR_ID_HERO)
						circle -= delta;
				case SHOOT_MULTI:
					if(FD.car_id == CAR_ID_SOLDIER){
						if(SHOOT_CIRCLE > circle-((delta>>2)+1))
						circle += delta;
					}else if(FD.car_id == CAR_ID_HERO){
						if(SHOOT_CIRCLE < circle+((delta>>2)+1))
						circle -= delta;
					}
					break;
				default:
					break;
			}
		}
		if(shoot_test)
		{
			circle -= delta;
			PRINT("c:%d\r\n", circle);
			shoot_test = 0;
		}
		
		
		if(shoot_print && (system_clock%1000 < 50) && shoot_print_mark)
		{
			switch(shoot)
			{
				case SHOOT_HOLD: PRINT("shoot: hold\r\n"); break;
				case SHOOT_SINGLE: PRINT("shoot: single\r\n"); break;
				case SHOOT_MULTI: PRINT("shoot: multi\r\n"); break;
				default: PRINT("shoot: unknown(%d)\r\n", shoot);break;
			}
			PRINT("circle: %d\r\n", SHOOT_CIRCLE);
			if(shoot_print == 2)
				shoot_print = 0;
			shoot_print_mark = 0;
		}
		if(system_clock%1000>100)
			shoot_print_mark = 1;
		
		task_delay_ms(10);
	}
}

void	task_shoot_control(void* param)
{
	S32		speed = 0;
	mpid_t		vcontrol;
	mpid_t		scontrol;
	float		temp;
	
	mpid_init(&vcontrol, 1, 0, 0);
	mpid_init(&scontrol, 300, 0, 0);
	
	while(1)
	{
		temp	= mpid_v(&scontrol, circle, SHOOT_CIRCLE);
		speed	= (S32)mpid_v(&vcontrol, temp, (float)shoot_speed);
		
		if(speed > 3000)
			speed = 3000;
		if(speed < -3000)
			speed = -3000;
		speed_update(speed);
		task_delay_ms(50);
	}
}

