/*
	FILE NAME:	task_remote.c
	DESCRIPTION:	the task dispose remote messages by the control rules
			and send the result to the relatively tasks
	
	EDIT RECORDS:
	---------
	AUTHOR:		FAN YAOWEN
	EDIT TIME:	2017/11/6-21:25
	MODIFICATION:	built the frame of remote task, simply send the msg to chassis and holder.
	---------
	AUTHOR:		FAN YAOWEN
	EDIT TIME:	2018/3/30
	MODIFICATION:	rewrote the shit code of receiver, add and change the romoting logic.
	---------
*/

#include "task_remote.h"

#include "task_init.h"

#include "dbus.h"

#define	CHASSIS_FOLLOW_ENABLE

#define	SP_F	(400U)
#define	SP_R	(SP_F)
#define	SP_C	(SP_F)

#define	SPH_F	(600U)
#define	SPH_R	(SPH_F)
#define	SPH_C	(SPH_F)

#define	SPL_F	(200U)
#define	SPL_R	(SPL_F)
#define	SPL_C	(SPL_F)


msg_t	chassis_msg;
msg_t	holder_msg;
msg_t	shoot_msg;
msg_t	lift_msg;
msg_t	view_msg;
msg_t	bullet_msg;
msg_t	clip_msg;
msg_t	drag_msg;
msg_t	give_msg;

msg_t	remote_msg;

void	remote_send_msg_init(void)
{
	msg_init(&remote_msg,	2,	sizeof(remote_data_t));
	msg_init(&chassis_msg,	4,	sizeof(chassis_ctrl_t));
	msg_init(&holder_msg,	4,	sizeof(holder_ctrl_t));
	msg_init(&shoot_msg,	2,	sizeof(shoot_ctrl_t));
	msg_init(&bullet_msg,	2,	sizeof(bullet_ctrl_t));
	msg_init(&clip_msg,	2,	sizeof(clip_ctrl_t));
	msg_init(&lift_msg,	2,	sizeof(lift_ctrl_t));
	msg_init(&drag_msg,	2,	sizeof(drag_ctrl_t));
	msg_init(&give_msg,	1,	sizeof(give_ctrl_t));
	msg_init(&view_msg,	2,	sizeof(view_ctrl_t));
}


void	remote_data_dispose(p_remote_data tg)
{
	tg->JR_LR	= (U16)((remote_buff[0]		| (remote_buff[1]<<8))				& 0x07ff);
	tg->JR_UD	= (U16)(((remote_buff[1]>>3)	| (remote_buff[2]<<5))				& 0x07ff);
	tg->JL_LR	= (U16)(((remote_buff[2]>>6)	| (remote_buff[3]<<2)	| (remote_buff[4]<<10))	& 0x07ff);
	tg->JL_UD	= (U16)(((remote_buff[4]>>1)	| (remote_buff[5]<<7))				& 0x07ff);
	tg->SL		= (U8)	((remote_buff[5]>>6)							& 0x03);
	tg->SR		= (U8)	((remote_buff[5]>>4)							& 0x03);
	
	tg->MX		= (U16)(remote_buff[6]		| (remote_buff[7]<<8));
	tg->MY		= (U16)(remote_buff[8]		| (remote_buff[9]<<8));
	tg->MCL		= (U8)	remote_buff[12];
	tg->MCR		= (U8)	remote_buff[13];
	
	tg->KEY		= (U16)(remote_buff[14]		| (remote_buff[15]<<8));
}

U32	dbus_error_mark = 0;

void	remote_handle(void)
{
	irq_close();
	if(dbus_buff_checked())
	{
		p_remote_data	data = msg_get_write_last(&remote_msg);
		remote_data_dispose(data);
		msg_write_finish_last(&remote_msg);
		if(dbus_error_mark == 1)
		{
			dbus_error_mark = 0;
			PRINT_RECORD(">");
		}
	}
	else
	{
		dbus_error_mark = 1;
		PRINT_RECORD("<");
	}
	irq_restore();
}

static
void	memcopy(void* target, void* source, U32 size)
{
	U32	i;
	U8	*pt = target;
	U8	*ps = source;
	for(i=0; i<size; i++){
		*pt	= *ps;
		pt++;
		ps++;
	}
}

remote_data_t	task_remote_buff;
void	task_remote(void* param)
{
	p_remote_data	data;
	
	dbus_init(1, remote_handle);
	
	task_insert_full(remote_test, NULL, 4, 400);
	
	while(1)
	{
		data	= msg_get_read_try(&remote_msg);
		if(data)
		{
			memcopy(&task_remote_buff, data, sizeof(remote_data_t));
			msg_read_finish(&remote_msg);
			switch(FD.car_id)
			{
				case CAR_ID_SOLDIER:
					remote_chassis(&task_remote_buff);
					remote_holder_shoot(&task_remote_buff);
					break;
				case CAR_ID_HERO:
					remote_chassis(&task_remote_buff);
					remote_holder_shoot(&task_remote_buff);
					remote_lift_bullet_view(&task_remote_buff);
					break;
				case CAR_ID_ENGINEER:
					remote_chassis(&task_remote_buff);
					remote_lift_bullet_view(&task_remote_buff);
					remote_drag_give(&task_remote_buff);
					break;
				default:
					PRINT_ERROR("unknown car id\r\n");
			}
			//PRINT("%d\r\n", system_clock%100);
		}
		task_delay_ms(0);
	}
}

U32	mode_flag = CHASSIS_NOHEAD;

void	remote_chassis(p_remote_data data)
{
	p_chassis_ctrl	pcc;
	if(data->SL	== RP_S_UP)//use joystick only
	{
		pcc	= msg_get_write_try(&chassis_msg);
		if(pcc	!= NULL)
		{
			pcc->front	= (S32)((S32)data->JL_UD - 1024);
			pcc->right	= (S32)((S32)data->JL_LR - 1024);
			pcc->clockwise	= (S32)((S32)data->JR_LR - 1024);
			#ifdef	CHASSIS_FOLLOW_ENABLE
			pcc->mode	= CHASSIS_FOLLOW;
			#else
			pcc->mode	= CHASSIS_NORMAL;
			#endif
			msg_write_finish(&chassis_msg);
		}
	}
	if(data->SL	== RP_S_DOWN)//use keyboard only
	{
		if(K_CHK(R))
			mode_flag	= CHASSIS_FOLLOW;
		else if(K_CHK(F))
			mode_flag	= CHASSIS_NOHEAD;
		pcc	= msg_get_write_try(&chassis_msg);
		if(pcc	!= NULL)
		{
			S32	dir;
			KEY_IF(dir, W, S, 1);
			KEY_SPEED(pcc->front, dir, SP_F, SPH_F, SPL_F);
			KEY_IF(dir, D, A, 1);
			KEY_SPEED(pcc->right, dir, SP_R, SPH_R, SPL_R);
			KEY_IF(dir, E, Q, 1);
			KEY_SPEED(pcc->clockwise, dir, SP_C, SPH_C, SPL_C);
			pcc->mode	= mode_flag;
			
			msg_write_finish(&chassis_msg);
		}
	}
}

S16	remote_mouse_x	= 0;
S16	remote_mouse_y	= 0;

U32	shoot_flag	= SHOOT_HOLD;
U32	shoot_single	= 0;

void	remote_holder_shoot(p_remote_data data)
{
	p_holder_ctrl	phc;
	p_shoot_ctrl	psc;
	
	if(data->SL	== RP_S_UP)//use joystick only
	{
		phc	= msg_get_write_try(&holder_msg);
		if(phc	!= NULL)
		{
			S32	temp;
			#ifdef	CHASSIS_FOLLOW_ENABLE
			temp		= data->JR_LR;
			temp		= temp - 1024;
			temp		= temp*3/2;
			phc->mouse_x	= temp;
			#else
			phc->mouse_x	= 0;
			#endif
			temp		= data->JR_UD;
			temp		= temp - 1024;
			temp		= temp*3/2;
			phc->mouse_y	= temp;
			msg_write_finish(&holder_msg);
		}
		
		if(data->SR == RP_S_MID){
			if(shoot_single == 0){
				shoot_flag	= SHOOT_SINGLE;
				shoot_single	= 1;
			}else{
				shoot_flag	= SHOOT_HOLD;}
		}else	if(data->SR == RP_S_UP){
			shoot_flag	= SHOOT_MULTI;
		}else{
			shoot_flag	= SHOOT_HOLD;
			shoot_single	= 0;
		}
		psc	= msg_get_write_try(&shoot_msg);
		if(psc	!= NULL){
			psc->shoot	= shoot_flag;
			msg_write_finish(&shoot_msg);
		}
	}
	if(data->SL	== RP_S_DOWN)//use keyboard only
	{
		if(!K_CHK(B))//in order to ignore the float when reseting mouse 
		{
			remote_mouse_x	+= (S16)data->MX*3;
			remote_mouse_y	-= (S16)data->MY*3;
			
			LIMIT_SYM(remote_mouse_x, 1000);
			LIMIT_SYM(remote_mouse_y, 1000);
		}
		else
		{
			if(K_CHK(SHIFT)){
				remote_mouse_x	= 0;
				remote_mouse_y	= 0;
			}
		}
		
		phc	= msg_get_write_try(&holder_msg);
		if(phc	!= NULL)
		{
			phc->mouse_x	= remote_mouse_x;
			phc->mouse_y	= remote_mouse_y;
			msg_write_finish(&holder_msg);
		}
		
		if(data->MCL){
			if(shoot_single == 0){
				shoot_flag	= SHOOT_SINGLE;
				shoot_single	= 1;
			}else{
				shoot_flag	= SHOOT_HOLD;}
		}else{
			shoot_single		= 0;
			if(data->MCR){
				shoot_flag	= SHOOT_MULTI;
			}else{
				shoot_flag	= SHOOT_HOLD;}
		}
		psc	= msg_get_write_try(&shoot_msg);
		if(psc	!= NULL){
			psc->shoot	= shoot_flag;
			msg_write_finish(&shoot_msg);
		}
	}
}


U32	liftf_flag	= LIFT_DOWN;
U32	liftb_flag	= LIFT_DOWN;
U32	strech_flag	= STRECH_IN;
U32	strech_mark	= 0;
U32	clip_flag	= CLIP_OPEN;
U32	view_flag	= VIEW_FRONT;
U32	view_mark	= 0;

void	remote_lift_bullet_view(p_remote_data data)
{
	p_lift_ctrl	plift;
	p_bullet_ctrl	pbullet;
	p_view_ctrl	pview;
	p_clip_ctrl	pclip;
	
	if(data->SL == RP_S_DOWN)//use keyboard only
	{
		if(K_CHK(Z)){
			if(K_CHK(SHIFT)){
				liftf_flag	= LIFT_UP;
			}else if(K_CHK(CTRL)){
				liftf_flag	= LIFT_DOWN;}}
		if(K_CHK(X)){
			if(K_CHK(SHIFT)){
				liftb_flag	= LIFT_UP;
			}else if(K_CHK(CTRL)){
				liftb_flag	= LIFT_DOWN;}}
		plift	= msg_get_write_try(&lift_msg);
		if(plift != NULL){
			plift->front	= liftf_flag;
			plift->back	= liftb_flag;
			msg_write_finish(&lift_msg);
		}
		
		if(K_CHK(C)){
			if(strech_mark == 0){
				if(K_CHK(SHIFT)){
					if(strech_flag < STRECH_OUT)
						strech_flag++;
				}else if(K_CHK(CTRL)){
					if(strech_flag > STRECH_IN)
						strech_flag--;}}
			strech_mark = 1;
		}else{
			strech_mark = 0;}
		pbullet	= msg_get_write_try(&bullet_msg);
		if(pbullet != NULL){
			pbullet->strech	= strech_flag;
			msg_write_finish(&bullet_msg);
		}
		
		if(K_CHK(V)){
			if(K_CHK(SHIFT)){
				clip_flag	= CLIP_CLOSE;
			}else if(K_CHK(CTRL)){
				clip_flag	= CLIP_OPEN;}}
		pclip	= msg_get_write_try(&clip_msg);
		if(pclip != NULL){
			pclip->clip	= clip_flag;
			msg_write_finish(&clip_msg);
		}
		
		if(K_CHK(G)){
			if(view_mark == 0){
				view_flag	= (view_flag+1)&(~0x00000002);}//toggle the bit0
				view_mark	= 1;
		}else{
			view_mark	= 0;}
		pview = msg_get_write_try(&view_msg);
		if(pview != NULL){
			pview->view	= view_flag;
			msg_write_finish(&view_msg);
		}
	}
}


U32	drag_flag	= DRAG_UP;
U32	drag_down_mark	= 0;
U32	give_flag	= DOOR_STOP;

void	remote_drag_give(p_remote_data data)
{
	p_drag_ctrl	pdrag;
	p_give_ctrl	pgive;
	
	if(data->SL == RP_S_DOWN)//use keyboard only
	{
		if(K_CHK(R)){
			if(K_CHK(SHIFT) && !drag_down_mark){
				if(drag_flag < DRAG_DOWN)
					drag_flag++;
				drag_down_mark = 1;
			}else if(K_CHK(CTRL)){
				drag_flag	= DRAG_UP;
			}
		}else{
			drag_down_mark = 0;
		}
		pdrag	= msg_get_write_try(&drag_msg);
		if(pdrag != NULL){
			pdrag->drag	= drag_flag;
			msg_write_finish(&drag_msg);
		}
		
		if(K_CHK(F)){
			if(K_CHK(SHIFT)){
				give_flag	= DOOR_OPEN;
			}else if(K_CHK(CTRL)){
				give_flag	= DOOR_CLOSE;
			}
		}else{
				give_flag	= DOOR_STOP;
		}
		pgive	= msg_get_write_try(&give_msg);
		if(pgive != NULL){
			pgive->door	= give_flag;
			msg_write_finish(&give_msg);
		}
	}
}


U32	receiver_print		= 0;
U32	receiver_print_mark	= 0;
void	receiver_control(void * p_param)
{
	U32 param = TO_U32(p_param);
	
	if(param <= 2)
		receiver_print	= param;
	else if(param == 3)
	{
		remote_mouse_x	= 0;
		remote_mouse_y	= 0;
	}
}

void	remote_test(void* param)
{
	resolve_add("receiver", receiver_control);
	
	while(1)
	{
		if(receiver_print && (system_clock%1000 < 50) && receiver_print_mark)
		{
			remote_data_t	rdata;
			
			if(dbus_buff_checked())
			{
				remote_data_dispose(&rdata);
				PRINT("chx: %d %d %d %d\r\n", rdata.JR_LR, rdata.JR_UD, rdata.JL_LR, rdata.JL_UD);
				PRINT("s1s2: %d %d\r\n", rdata.SL, rdata.SR);
				PRINT("wasd: %d %d %d %d\r\n",	KEY_CHECK(rdata.KEY,W), KEY_CHECK(rdata.KEY,A),\
								KEY_CHECK(rdata.KEY,S), KEY_CHECK(rdata.KEY,D));
				PRINT("c/s: %d %d\r\n",		KEY_CHECK(rdata.KEY,CTRL), KEY_CHECK(rdata.KEY,SHIFT));
				PRINT("qerfg: %d %d %d %d %d\r\n",	KEY_CHECK(rdata.KEY,Q), KEY_CHECK(rdata.KEY,E),\
							KEY_CHECK(rdata.KEY,R), KEY_CHECK(rdata.KEY,F), KEY_CHECK(rdata.KEY,G));
				PRINT("zxcvb: %d %d %d %d %d\r\n",	KEY_CHECK(rdata.KEY,Z), KEY_CHECK(rdata.KEY,X),\
							KEY_CHECK(rdata.KEY,C), KEY_CHECK(rdata.KEY,V), KEY_CHECK(rdata.KEY,B));
				PRINT("xy: %d %d\r\n", rdata.MX, rdata.MY);
				PRINT("x/y: %d %d\r\n", remote_mouse_x, remote_mouse_y);
			}
			else
				PRINT("remote_buff wrong\r\n");
			if(receiver_print == 2)
				receiver_print	= 0;
			receiver_print_mark	= 0;
		}
		if(system_clock%1000>100)
			receiver_print_mark	= 1;
		
		task_delay_ms(50);
	}
}










