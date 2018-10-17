#include "makos_includes.h"

#include "usart.h"
#include "flash_data.h"

#include "task_init.h"
#include "task_led.h"
#include "task_fl_test.h"
#include "task_judge.h"

#include <stdlib.h>
#include <locale.h>
#include "oled.h"

void	delay_short(void)
{
	U32	i;
	for(i=30000000; i; i--);
}

int	main(void)
{
	#define	S	CAR_ID_SOLDIER
	#define	H	CAR_ID_HERO
	#define	E	CAR_ID_ENGINEER
	#define	G	CAR_ID_GUARD
	FD_set(20180502, S, PCB_RECTANGLE);
	#undef	S
	#undef	H
	#undef	E
	#undef	G
	
	FD_check();
	
	switch(FD.pcb_type)
	{
		case PCB_RECTANGLE:	serial_init(3, 115200);	break;
		case PCB_SQUARE:	serial_init(2, 9600);	break;
		default:		while(1);
	}
	delay_short();
	printf("\r\nlalala\r\n");
	printf("%.2f\r\n",1.1);
	//RTT_printf("RTT_printf tset\r\n");
	
	FD_print();
	
	makos_init();
	
	//OLED_Init();
	//OLED_Clear(); 
	
//	while(1)
//	{
//	}
	
	task_create(task_init, NULL, 1);
	
	task_create(task_led_sys, NULL, 0);
	
	//task_create(task_fl_test, NULL, 3);
	
	makos_run();

	PRINT_ERROR("go back to main\r\n");
	
	//RTT_printf("hellow");

}



