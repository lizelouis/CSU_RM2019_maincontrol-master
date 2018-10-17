#ifndef _USART_H
#define _USART_H

#include "makos_includes.h"

extern	USART_TypeDef*		pserial;

void	serial_init(U32 num, U32 baudrate);

void	usart1_init(void);

void usart3_send_char(char ch);
void usart3_send_string(u8 *buff, u32 len);


void	serial_handler(void);

extern	voidf
	uif1, uif2, uif3, uif4, uif5;

#endif
