#include "task_led.h"

#include "led.h"

#include "flash_data.h"

MLED_DEFINE(led1, C, 13, 1);
MLED_DEFINE(led2, D, 2, 1);
MLED_DEFINE(led3, B, 4, 1);

U8	ss[] = "hahahaha";
U8*	sp = ss;
U32	uu = 12345;
float	ff = -123.456;
void	mak_print_string(U8* data);

extern
volatile S16	liftf_read;
extern
volatile S16	liftb_read;

extern
size_t	iic_loop[10];

void	task_led_sys(void* param)
{
	p_mled	led_sys;
	p_mled	led_user;
	switch(FD.pcb_type)
	{
		case	PCB_RECTANGLE:
			led_sys	= &led1;
			led_user = &led3;
			break;
		case	PCB_SQUARE:
			led_sys	= &led1;
			led_user = &led2;
			break;
		default:
			task_suspend(ptcb_curr);
	}
	
	mled_init(led_sys);
	mled_init(led_user);
	
	task_insert(task_led_user, led_user, 4);
	
	while(1)
	{
		mled_toggle(led_sys);
		//MAK_PRINT3("\r\nstring: %s - u32: %d - float: %f\r\n", U8*, ss, U32, 654321, float, 654.321);
		//MAK_PRINT3("%s - %d - %f\r\n", U8*, sp, U32, uu, float, ff);
		//PRINT("201: %d, 202: %d\r\n", liftf_read, liftb_read);
		//PRINT("iic: %d, %d, %d, %d, %d, %d, %d\r\n", iic_loop[0], iic_loop[1], iic_loop[2], iic_loop[3], iic_loop[4], iic_loop[5], iic_loop[6]);
		task_delay_ms(1000);
	}
}


U32	led_user_period	= 1200;

void	led_user_control(U32 ms)
{
	led_user_period = ms;
}

void	task_led_user(void* param)
{
	p_mled	led_user = param;
	resolve_add("led_user", led_user_control);
	
	while(1)
	{
		if((led_user_period > 50) && (led_user_period < 10000))
		{
			mled_toggle(led_user);
			task_delay_ms(led_user_period);
		}
		else if(led_user_period == 0)
		{
			mled_off(led_user);
			task_delay_ms(1000);
		}
		else
		{
			PRINT("user led period should bewteen 50 and 10000(ms).\r\n");
			led_user_period	= 0;
		}
		
	}
}

