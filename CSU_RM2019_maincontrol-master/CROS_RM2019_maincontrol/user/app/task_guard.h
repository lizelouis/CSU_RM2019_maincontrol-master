#ifndef _TASK_GUARD_H
#define _TASK_GUARD_H

#include "supply_motor.h"
#include "makos_includes.h"

extern supply_pid_struct guard_move_pid[2];

void guard_move_init(void);
void task_guard_move(void* param);

#endif
