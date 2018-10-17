/*
	FILE NAME:	mak_pid.c
	DESCRIPTION:	
	
	EDIT RECORDS:
	---------
	AUTHOR:		FAN YAOWEN
	EDIT TIME:	2018/5/2
	MODIFICATION:	
*/

#include "mak_pid.h"


void	mpid_init(p_mpid pid, float p, float i, float d)
{
	pid->p		= p;
	pid->i		= i;
	pid->d		= d;
	pid->a		= 0;
	pid->v		= 0;
	pid->s		= 0;
	pid->slimit	= 0;
	pid->sband	= 0;
}

void	mpid_init_pro(p_mpid pid, float slimit, float sband)
{
	pid->slimit	= slimit;
	pid->sband	= sband;
}

float	mpid_v(p_mpid pid, float vin, float vread)
{
	float	output = 0;
	float	vgap = vin - vread;
	
	pid->a	= vgap - pid->v;
	pid->v	= vgap;
	if(!pid->slimit || IN_RANGE(vgap, pid->slimit)){
		pid->s	+= vgap;
	}
	
	if(pid->slimit){
		LIMIT(pid->s, pid->slimit, -pid->slimit);
	}
	
	output	= pid->v*pid->p + pid->s*pid->i + pid->a*pid->d;
	
	return	output;
}

