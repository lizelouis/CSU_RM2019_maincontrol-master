#include "task_clip.h"
#include "task_remote.h"

void car_clip(U32 param)
{
	if(param == 0)
	{
		PRINT("low\r\n");
		GPIO_ResetBits(GPIOA, GPIO_Pin_2);
	}
	if(param == 1)
	{
		PRINT("high\r\n");
		GPIO_SetBits(GPIOA, GPIO_Pin_2);
	}
}


void task_clip(void* clip)
{
	p_clip_ctrl	pclip = NULL;
	U32		temp = CLIP_OPEN;
	
	GPIO_InitTypeDef gpio;
	
	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	gpio.GPIO_Pin = GPIO_Pin_2;
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_OType = GPIO_OType_PP;  
	gpio.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOA, &gpio);
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);
	
	resolve_add("car_clip", car_clip);
	
	while(1)
	{
		pclip = msg_get_read_try(&clip_msg);
		if(pclip)
		{
			temp	= pclip->clip;
			msg_read_finish_try(&clip_msg);
		}
		if(temp == CLIP_CLOSE)
		{
			GPIO_SetBits(GPIOA, GPIO_Pin_2);
		}
		else if(temp == CLIP_OPEN)
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_2);
		}
		task_delay_ms(10);
	}
}


