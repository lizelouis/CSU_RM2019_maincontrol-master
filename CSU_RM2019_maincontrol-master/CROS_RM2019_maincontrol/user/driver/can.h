#ifndef	_CAN_H
#define	_CAN_H

#include "makos_includes.h"


#define Control_ID				  	0x200//Ö÷¿Ø


extern U16 Driver_ID_HL;
extern U16 Driver_ID_HR;
extern U16 Driver_ID_BL;
extern U16 Driver_ID_BR;

volatile extern int16_t V_C_HL,V_C_HR,V_C_BL,V_C_BR;
volatile extern int16_t angle_HL,angle_HR,angle_BL,angle_BR;//»úÐµ½Ç¶È

extern
S16	shoot_speed;

extern
volatile int16_t strech_speed;

typedef	struct	_mcircle_t
{
	S32	circle;
	U32	angle;
}mcircle_t, *p_mcircle;

extern
mcircle_t	motor_circle[2][8];

void CAN1_Configuration(uint16_t IdHigh,uint16_t IdLow);
void CAN1_GPIO_Configuration(void);
void CAN1_NVIC_Configuration(void);
void CAN1_mode_Configuration(uint16_t IdHigh,uint16_t IdLow);

void can2_Configuration(uint16_t IdHigh,uint16_t IdLow);
void can2_GPIO_Configuration(void);
void can2_NVIC_Configuration(void);
void	can2_mode_Configuration(uint16_t IdHigh,uint16_t IdLow);


void	can_set(CanTxMsg* msg, U32 id, U8* data);
void	can_set_id(CanTxMsg* msg, U32 id);

S32	can_get_circle(U32 can, U32 id);



#endif
