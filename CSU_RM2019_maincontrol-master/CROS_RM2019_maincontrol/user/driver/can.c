/*
	FILE NAME:	can.c
	DESCRIPTION:	the task dispose remote messages by the control rules
			and send the result to the relatively tasks
	
	EDIT RECORDS:
	---------
	AUTHOR:                               
	EDIT TIME:	2017/11/18-13:47
	MODIFICATION:	built the frame of remote task, simply send the msg to chassis and holder.
	---------
	AUTHOR:		
	EDIT TIME:	
	MODIFICATION:	
	---------
*/
#include "can.h"
#include "task_holder.h"
#include "motor.h"


#define ITP_CAN1_RX0_PREEMPTION 2
#define ITP_CAN1_RX0_SUB 	3
#define ITP_CAN1_TX_PREEMPTION 	2
#define ITP_CAN1_TX_SUB 	4
#define ID_f 			0x0111

#define ITP_CAN2_RX0_PREEMPTION 2
#define ITP_CAN2_RX0_SUB 	3
#define ITP_CAN2_TX_PREEMPTION 	2
#define ITP_CAN2_TX_SUB 	4
#define	ID_f 			0x0111



void CAN1_Configuration(uint16_t IdHigh,uint16_t IdLow)
{
	CAN1_GPIO_Configuration();
	CAN1_NVIC_Configuration();
	CAN1_mode_Configuration(IdHigh,IdLow);
}

void CAN1_GPIO_Configuration()
{
	GPIO_InitTypeDef gpio;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);
	
	gpio.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_11;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&gpio);
}

void CAN1_NVIC_Configuration()
{
	NVIC_InitTypeDef nvic;
	nvic.NVIC_IRQChannel = CAN1_RX0_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority =5;
	nvic.NVIC_IRQChannelSubPriority = 5; 
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	
	nvic.NVIC_IRQChannel = CAN1_TX_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 5;
	nvic.NVIC_IRQChannelSubPriority = 5;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
}

void CAN1_mode_Configuration(uint16_t IdHigh, uint16_t IdLow)
{
	CAN_InitTypeDef can;
	CAN_FilterInitTypeDef can_filter;
	
	CAN_DeInit(CAN1);
	CAN_StructInit(&can);
	
	can.CAN_TTCM=DISABLE;
	can.CAN_ABOM=DISABLE;
	can.CAN_AWUM=DISABLE;
	can.CAN_NART=DISABLE;
	can.CAN_RFLM=DISABLE;
	can.CAN_TXFP=ENABLE;
	can.CAN_Mode=CAN_Mode_Normal;
	can.CAN_SJW=CAN_SJW_1tq;
	can.CAN_BS1=CAN_BS1_9tq;
	can.CAN_BS2=CAN_BS2_4tq;
	can.CAN_Prescaler=3;//???42/(1+9+4)/3=1Mbps
	CAN_Init(CAN1,&can);
	
	can_filter.CAN_FilterNumber=0;
	can_filter.CAN_FilterMode=CAN_FilterMode_IdMask;
	can_filter.CAN_FilterScale=CAN_FilterScale_32bit;

	can_filter.CAN_FilterIdHigh = IdHigh ;
	can_filter.CAN_FilterIdLow = IdLow ;
	can_filter.CAN_FilterMaskIdHigh=0x0000;
	can_filter.CAN_FilterMaskIdLow=0x0000;
	can_filter.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0 ;
	can_filter.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&can_filter);
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
	CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);
}

void	CAN1_TX_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN1, CAN_IT_TME) != RESET)
	{
		CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
	}
}

void can2_Configuration(uint16_t IdHigh,uint16_t IdLow)
{
	can2_GPIO_Configuration();
	can2_NVIC_Configuration();
	can2_mode_Configuration(IdHigh,IdLow);
}

void can2_GPIO_Configuration()//CAN_TX  CAN_RX  gpio???
{
	GPIO_InitTypeDef gpio;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	
//	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2);
//	GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2);

//	gpio.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_12;
//	gpio.GPIO_Mode = GPIO_Mode_AF;
//	gpio.GPIO_Speed = GPIO_Speed_100MHz;
//	gpio.GPIO_OType = GPIO_OType_PP;
//	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_CAN2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_CAN2);

	gpio.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	
	GPIO_Init(GPIOB,&gpio);
}

void can2_NVIC_Configuration(void)
{
 	NVIC_InitTypeDef nvic;
	
	nvic.NVIC_IRQChannel =CAN2_RX0_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority =7;
	nvic.NVIC_IRQChannelSubPriority =7;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	
	nvic.NVIC_IRQChannel = CAN2_TX_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 4;
	nvic.NVIC_IRQChannelSubPriority = 4;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
}

void can2_mode_Configuration(uint16_t IdHigh,uint16_t IdLow)
{
	CAN_InitTypeDef can;
	CAN_FilterInitTypeDef can_filter;
	
	CAN_DeInit(CAN2);
	CAN_StructInit(&can);
	
	can.CAN_TTCM=DISABLE;
	can.CAN_ABOM=DISABLE;
	can.CAN_AWUM=DISABLE;
	can.CAN_NART=DISABLE;
	can.CAN_RFLM=DISABLE;
	can.CAN_TXFP=ENABLE;
	can.CAN_Mode=CAN_Mode_Normal;
	can.CAN_SJW=CAN_SJW_1tq;
	can.CAN_BS1=CAN_BS1_9tq;
	can.CAN_BS2=CAN_BS2_4tq;
	can.CAN_Prescaler=3;
	CAN_Init(CAN2,&can);
	
	can_filter.CAN_FilterNumber=14;//???0
	can_filter.CAN_FilterMode=CAN_FilterMode_IdMask;//??????????
	can_filter.CAN_FilterScale=CAN_FilterScale_32bit;//??????32?
	can_filter.CAN_FilterIdHigh = IdHigh ;//???ID???
	can_filter.CAN_FilterIdLow = IdLow ;//???ID???
	can_filter.CAN_FilterMaskIdHigh=0x0000;//????16????????
	can_filter.CAN_FilterMaskIdLow=0x0000;///????16????????
	can_filter.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0 ;//??????FIFO0;#elif			(MCU_TYPE == HOLDER)
//	can_filter.CAN_FilterIdHigh = 0x0000 ;//过滤的ID的高位
//	can_filter.CAN_FilterIdLow = 0x0000 ;//过滤的ID的低位
//	can_filter.CAN_FilterMaskIdHigh=0x0000;//过滤器高16位每一位必须匹配
//	can_filter.CAN_FilterMaskIdLow=0x0000;///过滤器低16位每一位必须匹配
//	can_filter.CAN_FilterFIFOAssignment=0 ;//过滤器关联到FIFO0;
	can_filter.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&can_filter);
	
	CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);
	CAN_ITConfig(CAN2, CAN_IT_TME, ENABLE);
}

void	CAN2_TX_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN2,CAN_IT_TME) != RESET)
	{		
		CAN_ClearITPendingBit(CAN2,CAN_IT_TME);
	}
}





void	can_set(CanTxMsg* msg, U32 id, U8* data)
{
	size_t	i;
	
	msg->StdId	= id;
	msg->IDE	= CAN_ID_STD;
	msg->RTR	= CAN_RTR_DATA;
	msg->DLC	= 8;
	for(i=0; i<8; i++)
	{
		msg->Data[i]	= data[i];
	}
}

void	can_set_id(CanTxMsg* msg, U32 id)
{
	msg->StdId	= id;
	msg->IDE	= CAN_ID_STD;
	msg->RTR	= CAN_RTR_DATA;
	msg->DLC	= 8;
}





#define	CIRCLE_FASTEST	1000


void	circle_count(p_mcircle pmc, U16 angle, S16 speed)
{
	if(	(angle < CIRCLE_FASTEST)	   &&
		(pmc->angle > 8192-CIRCLE_FASTEST) &&
		(speed > 0)){
		pmc->circle++;
	}else if((angle > 8192-CIRCLE_FASTEST)	&&
		(pmc->angle < CIRCLE_FASTEST)	&&
		(speed < 0)){
		pmc->circle--;
	}
	pmc->angle = angle;
}

mcircle_t	motor_circle[2][8] = {0};

void	circle_count_simple(p_mcircle pmc, U16 angle)
{
	if(	(angle < CIRCLE_FASTEST)	   &&
		(pmc->angle > 8192-CIRCLE_FASTEST)){
		pmc->circle++;
	}else if((angle > 8192-CIRCLE_FASTEST)	&&
		(pmc->angle < CIRCLE_FASTEST)){
		pmc->circle--;
	}
	pmc->angle = angle;
}

mcircle_t	motor_circle_simple[2][8] = {0};

volatile int16_t V_C_HL,V_C_HR,V_C_BL,V_C_BR;//实际速度，C,current
volatile int16_t angle_HL,angle_HR,angle_BL,angle_BR;//机械角度

volatile int16_t supply_read_speed;
volatile U16 supply_read_angle;

S16	shoot_speed = 0;

volatile int16_t rise_speed[2];
volatile int16_t guard_move_speed[2];
volatile int16_t strech_speed;

volatile S16	liftf_read;
volatile S16	liftb_read;

void CAN1_RX0_IRQHandler(void)
{
	CanRxMsg	RxMessage;
        
	if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
		CAN_ClearFlag(CAN1, CAN_FLAG_FMP0);
		
		CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
		//htest_id	= data.StdId;
		//htest_angle	= (U32)(data.Data[0]<<8) | (data.Data[1]);
		
		if((RxMessage.StdId >= 0x201) && (RxMessage.StdId <= 0x208))
			circle_count(&(motor_circle[0][RxMessage.StdId-0x201]), \
				RxMessage.Data[0]<<8|RxMessage.Data[1],	\
				RxMessage.Data[2]<<8|RxMessage.Data[3]);
		
		
		
		if(RxMessage.StdId == 0x203)
		{
			strech_speed = RxMessage.Data[2]<<8|RxMessage.Data[3];
		}
		if(RxMessage.StdId == 0x205)
		{
			supply_read_angle = RxMessage.Data[0]<<8|RxMessage.Data[1];
			supply_read_speed = RxMessage.Data[2]<<8|RxMessage.Data[3];
			
			//shoot_handle(supply_read_angle, supply_read_speed);
			shoot_speed = supply_read_speed;
		}
		if(RxMessage.StdId == 0x201)
			rise_speed[0] = (RxMessage.Data[2]<<8)|RxMessage.Data[3];
		if(RxMessage.StdId == 0x202)
			rise_speed[1] = (RxMessage.Data[2]<<8)|RxMessage.Data[3];
		if(RxMessage.StdId == 0x207)
			guard_move_speed[0] = (RxMessage.Data[2]<<8)|RxMessage.Data[3];
		if(RxMessage.StdId == 0x208)
			guard_move_speed[1] = (RxMessage.Data[2]<<8)|RxMessage.Data[3];
		
	}
}


S16	drag_angle;

void	CAN2_RX0_IRQHandler(void)
{
	size_t		i;
	CanRxMsg	RxMessage;
	
	
	if(CAN_GetITStatus(CAN2, CAN_IT_FMP0) != RESET)
	{
		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
		CAN_ClearFlag(CAN2, CAN_FLAG_FMP0); 
		
		CAN_Receive(CAN2,CAN_FIFO0,&RxMessage);
		
		if ((RxMessage.IDE==CAN_ID_STD)&&(RxMessage.DLC==8))
		{
			for(i = 0;i < 4;i++)
			{
				if(RxMessage.StdId == Driver_ID[i])
				{
					chassis_V_C[i] = RxMessage.Data[2]<<8|RxMessage.Data[3];
					//*********死区处理**********
					remote_0_protec(&chassis_V_C[i],10);
					break;
				}
			}
			
			if((RxMessage.StdId >= 0x201) && (RxMessage.StdId <= 0x208))
				circle_count(&(motor_circle[1][RxMessage.StdId-0x201]), \
					RxMessage.Data[0]<<8|RxMessage.Data[1],	\
					RxMessage.Data[2]<<8|RxMessage.Data[3]);
			
		if((RxMessage.StdId >= 0x201) && (RxMessage.StdId <= 0x208))
			circle_count_simple(&(motor_circle_simple[1][RxMessage.StdId-0x201]), \
				((U16)(RxMessage.Data[0]<<8))|((U16)RxMessage.Data[1]));
		
			if(RxMessage.StdId == 0x205){
				holder_yaw_angle  = motor_circle_simple[1][4].angle
							+ 8192 * motor_circle_simple[1][4].circle;
				drag_angle = (int16_t)((RxMessage.Data[0])<<8)|(int16_t)(RxMessage.Data[1]);
			}
				/*
				holder_yaw_angle  = motor_circle[1][4].angle
							+ 8192 * motor_circle[1][4].circle;*/
			if(RxMessage.StdId == 0x206)
				holder_pitch_angle  = ((((int16_t)RxMessage.Data[0])<<8) |
							(int16_t)(RxMessage.Data[1]))
							+ 8192 * motor_circle_simple[1][5].circle;
			
			
		}
	}
}
















