#ifndef _MAK_PID_H
#define _MAK_PID_H

#include "makos_type.h"


typedef	struct	_mpid_t
{
	float	p;
	float	i;
	float	d;
	float	a;
	float	v;
	float	s;
	float	slimit;
	float	sband;
}mpid_t, *p_mpid;


void	mpid_init(p_mpid pid, float p, float i, float d);

float	mpid_v(p_mpid pid, float vin, float vread);


#endif
