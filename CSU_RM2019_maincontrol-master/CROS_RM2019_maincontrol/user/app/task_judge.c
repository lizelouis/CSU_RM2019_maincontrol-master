#include "task_judge.h"
#include "communicate.h"
#include "protocol.h"
#include "makos_includes.h"


//void judge_init(U8 *buff, U32 size)
//{
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

//  GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);

//  DMA_DeInit(DMA1_Stream0);

//  {
//    GPIO_InitTypeDef gpio;
//    gpio.GPIO_Pin = GPIO_Pin_2;
//    gpio.GPIO_Mode = GPIO_Mode_AF;
//    gpio.GPIO_OType = GPIO_OType_PP;
//    gpio.GPIO_Speed = GPIO_Speed_100MHz;
//    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
//    GPIO_Init(GPIOD, &gpio);
//  }
//  {
//    USART_InitTypeDef usart;
//    USART_DeInit(UART5);
//    usart.USART_BaudRate = 115200;
//    usart.USART_WordLength = USART_WordLength_8b;
//    usart.USART_StopBits = USART_StopBits_1;
//    usart.USART_Parity = USART_Parity_No;
//    usart.USART_Mode = USART_Mode_Rx;
//    usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//    USART_Init(UART5, &usart);
//  }
//  USART_Cmd(UART5, ENABLE);
//  {
//    DMA_InitTypeDef dma;
//    dma.DMA_Channel = DMA_Channel_4;
//    dma.DMA_PeripheralBaseAddr = (uint32_t) & (UART5->DR);
//    dma.DMA_Memory0BaseAddr = (uint32_t)buff;
//    dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
//    dma.DMA_BufferSize = size;
//    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
//    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//    dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//    dma.DMA_Mode = DMA_Mode_Circular;
//    dma.DMA_Priority = DMA_Priority_VeryHigh;
//    dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
//    dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
//    dma.DMA_MemoryBurst = DMA_MemoryBurst_Single;
//    dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//    DMA_Init(DMA1_Stream0, &dma);
//  }

//  DMA_ITConfig(DMA1_Stream0, DMA_IT_TC | DMA_IT_TE | DMA_IT_FE, ENABLE);
//  USART_DMACmd(UART5, USART_DMAReq_Rx, ENABLE);

//  USART_ClearITPendingBit(UART5, USART_IT_RXNE);
//  DMA_Cmd(DMA1_Stream0, ENABLE);
//}


void	judge_init()
{
	USART_InitTypeDef	USART_InitStructure;
	GPIO_InitTypeDef	GPIO_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	
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
		
	USART_InitStructure.USART_BaudRate		= 115200;
	USART_InitStructure.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode			= USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity		= USART_Parity_No;
	USART_InitStructure.USART_StopBits		= USART_StopBits_1;
	USART_InitStructure.USART_WordLength		= USART_WordLength_8b;
	USART_Init(USART3, &USART_InitStructure);
	
	
	NVIC_InitStructure.NVIC_IRQChannel	= USART3_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		= 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd			= ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);	
	
	USART_Cmd(USART3, ENABLE);	
	USART_ClearITPendingBit(USART3, USART_IT_IDLE);
	
}



//#define UART_RX_DMA_SIZE       1024

//uint8_t judge_dma_rxbuff[2][UART_RX_DMA_SIZE];
//uint8_t pc_dma_rxbuff[2][UART_RX_DMA_SIZE];

//extern u32 uart_idle_G;


//static unpack_data_t judge_unpack_obj;
//static uart_dma_rxdata_t judge_rx_obj;

void task_judge(void *param)
{
	//USART_Config();
	//judge_init();
  while (1)
  {
		
//    if (uart_idle_G == 1)
//    {
//      uart_idle_G = 0;
//      //dma_buffer_to_unpack_buffer(&judge_rx_obj, UART_IDLE_IT);
//      //unpack_fifo_data(&judge_unpack_obj, DN_REG_ID);
//    }
    task_delay_ms(1);
  }
}


//void USART3_IRQHandler()
//{

//	if (USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
//	{
//		USART_SendData(USART3,0x133);
//		USART_ClearITPendingBit(USART3, USART_IT_IDLE);
//	}
//	
//}


