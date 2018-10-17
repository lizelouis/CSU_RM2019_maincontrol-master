#ifndef _TASK_HOLDER_H
#define _TASK_HOLDER_H

#include "makos_includes.h"


extern
S16	holder_yaw_angle;
extern
S16	holder_pitch_angle;


void	task_holder(void* param);



void	RM6025_set_data_init(CanTxMsg* data);
U32	RM6025_set_yaw(S16 yaw);
U32	RM6025_set_pitch(S16 pitch);
U32	RM6025_set_yaw_pitch(S16 yaw, S16 pitch);

#endif
