/*
	FILE NAME:	task_remote.h
	DESCRIPTION:	the task dispose remote messages by the control rules
			and send the result to the relatively tasks
	
	EDIT RECORDS:
	---------
	AUTHOR:		FAN YAOWEN
	EDIT TIME:	2017/11/6-21:26
	MODIFICATION:	built the frame of remote task, simply send the msg to chassis and holder.
	---------
	AUTHOR:		
	EDIT TIME:	
	MODIFICATION:	
	---------
*/

#ifndef _TASK_REMOTE_H
#define _TASK_REMOTE_H

#include "makos_includes.h"


#define	RP_S_UP		(1U)
#define	RP_S_MID	(3U)
#define	RP_S_DOWN	(2U)

#define RP_KEY_W	(1U << 0)
#define RP_KEY_S	(1U << 1)
#define RP_KEY_A	(1U << 2)
#define RP_KEY_D	(1U << 3)
#define RP_KEY_Q	(1U << 6)
#define RP_KEY_E	(1U << 7)
#define RP_KEY_SHIFT	(1U << 4)
#define RP_KEY_CTRL	(1U << 5)
#define RP_KEY_R	(1U << 8)
#define RP_KEY_F	(1U << 9)
#define RP_KEY_G	(1U << 10)
#define RP_KEY_Z	(1U << 11)
#define RP_KEY_X	(1U << 12)
#define RP_KEY_C	(1U << 13)
#define RP_KEY_V	(1U << 14)
#define RP_KEY_B	(1U << 15)

#define	RP_KEY(key)		(CAT(RP_KEY_, key))
#define	KEY_CHECK(key,check)	((key&(RP_KEY(check)))?1:0)
#define	K_CHK(check)		KEY_CHECK(data->KEY, check)

#define	KEY_IF(target, key1, key2, value)		\
	do{						\
		if(data->KEY & RP_KEY(key1)){		\
			if(data->KEY &  RP_KEY(key2))	\
				target	= 0;		\
			else				\
				target	= (value);	\
		}else{					\
			if(data->KEY &  RP_KEY(key2))	\
				target	= -(value);	\
			else				\
				target	= 0;		\
		}					\
	}while(0)

#define	KEY_SPEED(target, dir, sp, sph, spl)		\
	do{						\
		if(dir == 0){				\
			target = 0;			\
		}else if(dir<0){			\
			if(K_CHK(SHIFT)){		\
				target	= -sph;		\
			}else if(K_CHK(CTRL)){		\
				target	= -spl;		\
			}else{				\
				target	= -sp;		\
			}				\
		}else{					\
			if(K_CHK(SHIFT)){		\
				target	= sph;		\
			}else if(K_CHK(CTRL)){		\
				target	= spl;		\
			}else{				\
				target	= sp;		\
			}				\
		}					\
	}while(0)


typedef	struct	_remote_data_t
{
	U16	JR_LR;
	U16	JR_UD;
	U16	JL_LR;
	U16	JL_UD;
	U8	SL;
	U8	SR;
	
	U16	MX;
	U16	MY;
	U8	MCL;
	U8	MCR;
	
	U16	KEY;
}remote_data_t, *p_remote_data;

extern	S16	remote_mouse_x;
extern	S16	remote_mouse_y;

extern
msg_t	remote_msg;

void	remote_send_msg_init(void);

void	task_remote(void* param);

void	remote_chassis(p_remote_data data);
void	remote_holder_shoot(p_remote_data data);
void	remote_lift_bullet_view(p_remote_data data);
void	remote_drag_give(p_remote_data data);


void	remote_test(void* param);



#define	CHASSIS_NORMAL	((U32)(0x96300000))
#define	CHASSIS_FOLLOW	((U32)(0x67890001))
#define	CHASSIS_NOHEAD	((U32)(0x67890002))
typedef	struct	_chassis_ctrl_t
{
	S32	front;
	S32	right;
	S32	clockwise;
	U32	mode;
}chassis_ctrl_t, *p_chassis_ctrl;
extern	msg_t	chassis_msg;

typedef	struct	_holder_ctrl_t
{
	S32	mouse_x;
	S32	mouse_y;
}holder_ctrl_t, *p_holder_ctrl;
extern	msg_t	holder_msg;

#define	LIFT_HOLD	((U32)(0x67890000))
#define	LIFT_UP		((U32)(0x67890001))
#define	LIFT_DOWN	((U32)(0x67890002))
typedef	struct	_lift_ctrl_t
{
	U32	front;
	U32	back;
}lift_ctrl_t, *p_lift_ctrl;
extern	msg_t	lift_msg;

#define	VIEW_FRONT	((U32)(0x96300000))
#define	VIEW_BACK	((U32)(0x96300001))
typedef	struct	_view_ctrl_t
{
	U32	view;
}view_ctrl_t, *p_view_ctrl;
extern	msg_t	view_msg;

#define	STRECH_IN	((U32)(0x67830000))
#define	STRECH_MID	((U32)(0x67830001))
#define	STRECH_OUT	((U32)(0x67830002))
typedef	struct	_bullet_ctrl_t
{
	U32	strech;
}bullet_ctrl_t, *p_bullet_ctrl;
extern	msg_t	bullet_msg;

#define	CLIP_OPEN	((U32)(0x67810000))
#define	CLIP_CLOSE	((U32)(0x67810001))
typedef	struct	_clip_ctrl_t
{
	U32	clip;
}clip_ctrl_t, *p_clip_ctrl;
extern	msg_t	clip_msg;

#define	SHOOT_HOLD	((U32)(0x78900000))
#define	SHOOT_SINGLE	((U32)(0x78900001))
#define	SHOOT_MULTI	((U32)(0x78900002))
typedef	struct	_shoot_ctrl_t
{
	U32	shoot;
}shoot_ctrl_t, *p_shoot_ctrl;
extern	msg_t	shoot_msg;

#define	DRAG_UP		((U32)(0x67880000))
#define	DRAG_MID	((U32)(0x67880001))
#define	DRAG_DOWN	((U32)(0x67880002))
typedef	struct	_drag_ctrl_t
{
	U32	drag;
}drag_ctrl_t, *p_drag_ctrl;
extern	msg_t	drag_msg;

#define	DOOR_STOP	((U32)(0x35790000))
#define	DOOR_OPEN	((U32)(0x35790001))
#define	DOOR_CLOSE	((U32)(0x35790002))
typedef	struct	_give_ctrl_t
{
	U32	door;
}give_ctrl_t, *p_give_ctrl;
extern	msg_t	give_msg;








#endif
