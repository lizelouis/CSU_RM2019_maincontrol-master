#include "usart.h"

#include "makos_print.h"
#include "makos_resolver.h"

//#include "stm32f4xx_conf.h"


USART_TypeDef*		pserial;

U32	serial_init_flag = 0;

void	serial_init(U32	num, U32 baudrate)
{
	USART_InitTypeDef	USART_InitStructure;
	GPIO_InitTypeDef	GPIO_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	if(serial_init_flag)
		printf("serial_init() can't be excuted twice\r\n");
	else
		serial_init_flag = 1;
	
	if(num == 0)
		while(1);
	if(num == 1)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
			
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);//?????
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
		
		GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_9 | GPIO_Pin_10;   //????
		GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		pserial	= USART1;
		
		NVIC_InitStructure.NVIC_IRQChannel	= USART1_IRQn;
		
		uif1	= serial_handler;
	}
	if(num == 2)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);//?????
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
		
		GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_2 | GPIO_Pin_3;   //????
		GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		pserial	= USART2;
		
		NVIC_InitStructure.NVIC_IRQChannel	= USART2_IRQn;
		
		uif2	= serial_handler;
	}
	if(num == 3)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);
		
		GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_10 | GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		
		pserial	= USART3;
		
		NVIC_InitStructure.NVIC_IRQChannel	= USART3_IRQn;
		
		uif3	= serial_handler;
	}
	if(num == 4)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
		
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_UART4);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_UART4);
		
		GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_0 | GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		pserial	= UART4;
		
		NVIC_InitStructure.NVIC_IRQChannel	= UART4_IRQn;
		
		uif4	= serial_handler;
	}
	if(num == 5)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
		
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);
		
		GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_2;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
		
		pserial	= UART5;
		
		NVIC_InitStructure.NVIC_IRQChannel	= UART5_IRQn;
		
		uif5	= serial_handler;
	}
	
	USART_InitStructure.USART_BaudRate		= baudrate;
	USART_InitStructure.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode			= USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity		= USART_Parity_No;
	USART_InitStructure.USART_StopBits		= USART_StopBits_1;
	USART_InitStructure.USART_WordLength		= USART_WordLength_8b;
	USART_Init(pserial, &USART_InitStructure);
	
	USART_Cmd(pserial, ENABLE);
	
	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		= 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd			= ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void	serial_handler(void)
{
	USART_ClearITPendingBit(pserial, USART_IT_RXNE);
	if(pipe_resolver.length < 32)
		pipe_write(&pipe_resolver, pserial->DR);
}

static
void	emptyf(void)
{
	;
}

voidf	uif1 = emptyf;
voidf	uif2 = emptyf;
voidf	uif3 = emptyf;
voidf	uif4 = emptyf;
voidf	uif5 = emptyf;

#include "irq_dead_check.h"

IRQ_CHECK_DEFINE(usart1);
void	USART1_IRQHandler(void)
{
	IRQ_CHECK(usart1);
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		uif1();
	}
}

IRQ_CHECK_DEFINE(usart2);
void	USART2_IRQHandler(void)
{
	IRQ_CHECK(usart2);
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		uif2();
	}
}


IRQ_CHECK_DEFINE(usart3);
void	USART3_IRQHandler(void)
{
	IRQ_CHECK(usart3);
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		uif3();
	}
}


IRQ_CHECK_DEFINE(usart4);
void	UART4_IRQHandler(void)
{
	IRQ_CHECK(usart4);
	if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
		uif4();
	}
}

IRQ_CHECK_DEFINE(usart5);
void	UART5_IRQHandler(void)
{
	IRQ_CHECK(usart5);
	if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(UART5, USART_IT_RXNE);
		uif5();
	}
}




int	fputc(int ch, FILE *f)//自定义printf重定向
{
	if(system_param.use_print && print_flag){
		pipe_write(&ptcb_curr->pipe_print, (U8)ch);
	}else if(record_flag){
		if(record_write < 512)
			record_buff[record_write++] = (U8)ch;
	}else{
		while(((pserial->SR)&0X40)==0);
		pserial->DR	= (u8)ch;
	}
	return ch;
}


#pragma import(__use_no_semihosting)


struct __FILE 
{ 
	int handle; 

};

typedef	struct __FILE	FILE;

FILE __stdout;       

_sys_exit(int x) 
{ 
	x = x; 
} 


//*******************uart1 use for wave_show*******************
void	usart1_init(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART2);//?????
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART2);
             
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;   //????
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	USART_InitStructure.USART_BaudRate			= 115200;
	USART_InitStructure.USART_HardwareFlowControl		= USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode				= USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity			= USART_Parity_No;
	USART_InitStructure.USART_StopBits			= USART_StopBits_1;
	USART_InitStructure.USART_WordLength			= USART_WordLength_8b;
	USART_Init(USART1 , &USART_InitStructure);
		
	USART_Cmd(USART1 , ENABLE);
}

void usart3_send_char(char ch)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	USART_SendData(USART1,(u8)ch);	
}

void usart3_send_string(u8 *buff, u32 len)
{
    while(len--)
    {
        usart3_send_char(*buff);
        buff++;
    }
}
