#ifndef _TASK_CAR_RISE_H
#define _TASK_CAR_RISE_H

#include "supply_motor.h"
#include "makos_includes.h"



extern supply_pid_struct rise_pid[2];


void car_rise_init(void);
//void car_rise(u16 param);


void task_car_rise_hero(void* param);
void task_car_rise_engineer(void* param);


#endif
