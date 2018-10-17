#include "stm32f4xx.h"
#include "tim5.h"

//PA15---PWM---TIM2_CH1
//PB3---PWM---TIM2_CH2
//PB10---PWM---TIM2_CH3
//PB11---PWM---TIM2_CH4
//u8 surgewheel_state;
//??
void TIM5_Configuration(U32 num)
{
	TIM_TimeBaseInitTypeDef	tim;
	GPIO_InitTypeDef	gpio;
	TIM_OCInitTypeDef	oc;	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	
	GPIO_PinAFConfig(GPIOA, (U8)(num-1), GPIO_AF_TIM5);
	
	gpio.GPIO_Pin = (1<<(num-1));   //???? TIM2_CH2;CH3;CH4
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
	
	if(num == 1){
		TIM_OC1Init(TIM5, &oc);
		TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);
	}else if(num == 2){
		TIM_OC2Init(TIM5, &oc);
		TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);
	}else if(num == 3){
		TIM_OC3Init(TIM5, &oc);
		TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);
	}else if(num == 4){
		TIM_OC4Init(TIM5, &oc);
		TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);
	}
	
	TIM_CtrlPWMOutputs(TIM5, ENABLE);
	TIM_ARRPreloadConfig(TIM5, ENABLE);
	TIM_Cmd(TIM5, ENABLE);
}
