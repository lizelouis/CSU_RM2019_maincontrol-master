#ifndef _MAKOS_MEMORY_H
#define _MAKOS_MEMORY_H

#include "makos_type.h"
#ifndef NULL
#define NULL 0
#endif

//����2���ڴ��
#define SRAMIN	 0		//�ڲ��ڴ��
#define SRAMEX   1		//�ⲿ�ڴ��


#define SRAMBANK 	2	//����֧�ֵ�SRAM����.	


//mem1�ڴ�����趨.mem1��ȫ�����ڲ�SRAM����.
#define MEM1_BLOCK_SIZE			32  	  						//�ڴ���СΪ32�ֽ�
#define MEM1_MAX_SIZE			100*1024  						//�������ڴ� 100K
#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE/MEM1_BLOCK_SIZE 	//�ڴ���С,�ڴ������

//mem2�ڴ�����趨.mem2���ڴ�ش����ⲿSRAM����
#define MEM2_BLOCK_SIZE			32  	  						//�ڴ���СΪ32�ֽ�
#define MEM2_MAX_SIZE			960 *1024  						//�������ڴ�960K
#define MEM2_ALLOC_TABLE_SIZE	MEM2_MAX_SIZE/MEM2_BLOCK_SIZE 	//�ڴ���С,�ڴ������


//�ڴ���������   ����ָ��
struct malloc_cortol_struct
{
	void (*init)(U8);					//��ʼ��
	U8 (*perused)(U8);		  	    	//�ڴ�ʹ����
	U8 	*membase[SRAMBANK];				//�ڴ�� ����SRAMBANK��������ڴ�
	U16 *memmap[SRAMBANK]; 				//�ڴ����״̬��
	U8  memrdy[SRAMBANK]; 				//�ڴ�����Ƿ����,0 - δ����,1 - ����
};
extern struct malloc_cortol_struct malloc_cortol;	 //��mallco.c���涨��

void my_mem_set(void *s,U8 c,U32 num)  ;	//�����ڴ�
void my_mem_cpy(void *des,void *src,U32 len)  ;//�����ڴ�     
void my_mem_init(U8 memx);				//�ڴ�����ʼ������(��/�ڲ�����)
U32 my_mem_malloc(U8 memx,U32 size);	//�ڴ����(�ڲ�����)
U8 my_mem_free(U8 memx,U32 offset);		//�ڴ��ͷ�(�ڲ�����)
U8 my_mem_perused(U8 memx);				//����ڴ�ʹ����(��/�ڲ�����) 
////////////////////////////////////////////////////////////////////////////////
//�û����ú���
void myfree(U8 memx,void *paddr) ;  			//�ڴ��ͷ�(�ⲿ����)
void *mymalloc(U8 memx,U32 size);			//�ڴ����(�ⲿ����)
void *myrealloc(U8 memx,void *paddr,U32 size) ;//���·����ڴ�(�ⲿ����)


#endif
