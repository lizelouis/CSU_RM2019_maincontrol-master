#ifndef	_TIM3_4_H
#define	_TIM3_4_H

#include "makos_type.h"


void TIM3_init(void);
void TIM4_init(void);

void	give_motor_open(U32 ratio);
void	give_motor_close(U32 ratio);
void	give_motor_stop(void);

S32	give_motor_circle(void);


#endif
