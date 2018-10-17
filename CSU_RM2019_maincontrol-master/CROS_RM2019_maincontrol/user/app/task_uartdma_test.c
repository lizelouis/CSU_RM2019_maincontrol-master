#include "task_uartdma_test.h"
#include "uart_dma.h"
#include "communicate.h"
#include "protocol.h"
#include "judgement_info.h"

/*
ʵ��ƽ̨:
	RM2018�ڶ���
	DMA_UART�շ�����:USART3
	���Դ���:UART5
ʵ������
	TX�����ڲ��Ͻ��ա�D��
	RX:����3�������ݣ�����5���ؽ��յ�������
	˫����RX:�������������һ�δ�����������ж�,������ʾ��ɵ��δ���Ĵ�����
author:hhh
date:9/11/2018
�ײ����ã�����ԭ��DMA

*/

uint8_t send_judge_G = 0;

void send_judge(void * p_param)
{

	//printf("get float = %.2f\r\n",TO_FLOAT(p_param));
	
	//printf("get u32 = %d\r\n",TO_U32(p_param));	
	
	
//	if(param == 1)
//		send_judge_G = 1;
//	else if(param == 0)
//		send_judge_G = 0;
}

#define JUDGE_FIFO_BUFLEN      500

static unpack_data_t judge_unpack_obj;
static uart_dma_rxdata_t judge_rx_obj;
static fifo_s_t  judge_rxdata_fifo;
static uint8_t   judge_rxdata_buf[JUDGE_FIFO_BUFLEN];

void task_uartdma_test()
{
	int16_t i = 0;
	  /* ��ʼ��USART */
  Debug_USART_Config(); 

  /* ����ʹ��DMAģʽ */
  USART_DMA_TX_Config();
	USART_DoubleDMA_RX_Config();
	
  /* judge data fifo init */
  fifo_s_init(&judge_rxdata_fifo, judge_rxdata_buf, JUDGE_FIFO_BUFLEN);	
  /* initial judge data dma receiver object */
  judge_rx_obj.data_fifo = &judge_rxdata_fifo;
  judge_rx_obj.buff_size = READBUFF_SIZE;
  judge_rx_obj.buff[0] = ReadBuff;
  judge_rx_obj.buff[1] = ReadBuff2;		

  /* initial judge data unpack object */
  judge_unpack_obj.data_fifo = &judge_rxdata_fifo;
  judge_unpack_obj.p_header = (frame_header_t *)judge_unpack_obj.protocol_packet;
  judge_unpack_obj.index = 0;
  judge_unpack_obj.data_len = 0;
  judge_unpack_obj.unpack_step = STEP_HEADER_SOF;	
	
	
	USART_SendData(DEBUG_USART,0x11);
	//printf("---uart_dma tx test---");
	printf("-----------uart_dma rx test-----------\r\n");
	
  for(i=0;i<SENDBUFF_SIZE;i++)
  {
    SendBuff[i]	 = 'D';
    
  }

  for(i=0;i<READBUFF_SIZE;i++)
  {
    ReadBuff[i]	 = '0';
    ReadBuff2[i] = '0';
  }
	/* USART3 �� DMA����RX���� */
	USART_DMACmd(DEBUG_USART, USART_DMAReq_Rx, ENABLE);
	/* USART3 �� DMA����TX���� */
	//USART_DMACmd(DEBUG_USART, USART_DMAReq_Tx, ENABLE);
	
	resolve_add("send_judge",send_judge);
	
	while(1)
	{	
		//***************test******************
//		printf("%s\r\n",ReadBuff);
//		printf("%s\r\n",ReadBuff2);//doubleDMA_UART_RX����
//		printf("trans_ok:%d\r\n",trans_ok);
//		printf("DMA_memory_id:%d\r\n",DMA_memory_id);
//		printf("uart_idle_times:%d\r\n",uart_idle_times);		
		//*************************************
		
		if(uart_idle_G == 1)
		{
			uart_idle_G = 0;
			//��DMA������ת����fifo_buff
			dma_buffer_to_unpack_buffer(&judge_rx_obj, UART_IDLE_IT);
//			printf("write_index%d:\r\n",judge_rx_obj.write_index);//��DMA�ĵ���������С���������ֵ
//			printf("read_index%d:\r\n",judge_rx_obj.read_index);
//			printf("buf:%s:\r\n",judge_rx_obj.data_fifo->start_addr);//��fifo�������Ĵ�С�����䳤��
//			printf("fifo_overflow%d:\r\n",fifo_overflow);
			//����fifo_buff������
			unpack_fifo_data(&judge_unpack_obj, DN_REG_ID);
//			printf("active data%s\r\n",judge_unpack_obj.protocol_packet);
//			printf("unpack_step%d\r\n",judge_unpack_obj.unpack_step);
//			printf("index%d\r\n",judge_unpack_obj.index);
			//�����ɹ�������ʾ
//			printf("cmd_ID%d\r\n",cmd_ID);
//			printf("data_Length%d\r\n",data_Length);
//			printf("attacked armor id:%d\r\n",judge_recv_mesg.blood_changed_data.armor_type);//������װ�װ�ID
//			printf("attacked armor id:%d\r\n",judge_recv_mesg.blood_changed_data.hurt_type);//��Ѫԭ��

		}
		else if(uart_DMA_full_G == 1)
		{
			uart_DMA_full_G = 0;
			dma_buffer_to_unpack_buffer(&judge_rx_obj, UART_DMA_FULL_IT);
		}
		
		if(system_clock%1500 < 50)
		{
//			printf("-----------------------------\r\n");
//			printf("cmd_ID            %d\r\n",cmd_ID);
//			printf("data_Length       %d\r\n",data_Length);			
//			printf("attacked armor id:%d\r\n",judge_recv_mesg.blood_changed_data.armor_type);//������װ�װ�ID
//			printf("hurt_type:        %d\r\n",judge_recv_mesg.blood_changed_data.hurt_type);//��Ѫԭ��
//			printf("game_process:     %d\r\n",judge_recv_mesg.game_information.game_process);
//			printf("max_hp:           %d\r\n",judge_recv_mesg.game_information.max_hp);//���Ѫ��
//			printf("remain_hp:        %d\r\n",judge_recv_mesg.game_information.remain_hp);//ʣ��Ѫ��
//			printf("stage_remain_time:%d\r\n",judge_recv_mesg.game_information.stage_remain_time);//����ʣ��ʱ��
//			printf("shooter1_heat:    %d\r\n",judge_recv_mesg.power_heat_data.shooter1_heat);//ǹ��1����
//			printf("card_idx:         %d\r\n",judge_recv_mesg.rfid_data.card_idx);//RFID,ID��
//			printf("pos_data.x:       %.2f\r\n",judge_recv_mesg.robot_pos_data.x);//λ��_x
		}
		
		if(send_judge_G == 1)
		{
			
		}
		
		task_delay_ms(5);
		
	}
	
}
