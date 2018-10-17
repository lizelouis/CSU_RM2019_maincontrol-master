#include "iic.h"
#include "stdio.h"

#include "makos_includes.h"

#define SDA_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PE5ÊäÈëÄ£Ê½
#define SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //PE5Êä³öÄ£Ê½	
void I2C_RESET(void)
{
	unsigned char i;
	SDA_IN();
	if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9))
	{
		for(i = 0;i<9;i++)
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_8);
			if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9))
				GPIO_SetBits(GPIOB,GPIO_Pin_8);
			else
				break;
		}
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9))
		{
			SDA_OUT();
			GPIO_ResetBits(GPIOB,GPIO_Pin_9);
			GPIO_SetBits(GPIOB,GPIO_Pin_8);
			GPIO_SetBits(GPIOB,GPIO_Pin_9);
		}
	}
	else
	{
		IIC_Config();
	}
	IIC_Config();
}



unsigned char I2C_Err = 0;
u32 ulTimeOut_Time;
void IIC_Config()
{
	I2C_InitTypeDef I2C_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_ClocksTypeDef   rcc_clocks;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_I2C1);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	I2C_DeInit(I2C1);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	//I2C_InitStructure.I2C_OwnAddress1 = 0x00;//Ö÷»ú ÎÞÓÃ
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;//ack enable 
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed =100000;//100k
	
	I2C_Cmd(I2C1,ENABLE);
	I2C_Init(I2C1,&I2C_InitStructure);
	I2C_AcknowledgeConfig(I2C1, ENABLE);   
}

void	iic_delay(void)//1.5us
{
	size_t	i;
	for(i=50; i; i--);
}

#define	IIC_TIME_LIMIT(check, handle, error)\
	do{\
		size_t	i;\
		size_t	flag = 0;\
		for(i = 0; i < 3000; i++){\
			if(!check){\
				flag = 1;\
				break;\
			}\
			/*iic_delay();*/\
		}\
		if(!flag){\
			handle();\
			printf("$");\
			error = 1;\
		}else{\
			error = 0;\
		}\
	}while(0)

#define	IIC_ERROR_CHECK(check, num)\
	do{\
		IIC_TIME_LIMIT(check, I2C_RESET, error_mark);\
		if(error_mark){\
			printf("%d", num);\
			task_delay_ms(1);\
			goto	restart;\
		}\
	}while(0)


size_t	iic_loop[10] = {0};

void I2C_ByteWrite(uint8_t REG_Address,uint8_t REG_data)
{
	
	size_t	error_mark = 0;
	
	restart:
	error_mark = 0;
	
  I2C_GenerateSTART(I2C1,ENABLE);
	
	//IIC_ERROR_CHECK(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));

  I2C_Send7bitAddress(I2C1,SlaveAddress,I2C_Direction_Transmitter);

	//IIC_ERROR_CHECK(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  I2C_SendData(I2C1,REG_Address);

	//IIC_ERROR_CHECK(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  I2C_SendData(I2C1,REG_data);

	//IIC_ERROR_CHECK(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  I2C_GenerateSTOP(I2C1,ENABLE);
  
  /*
	size_t	error_mark = 0;
	
	restart:
	error_mark = 0;
	
  I2C_GenerateSTART(I2C1,ENABLE);
	
	IIC_ERROR_CHECK(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT), 11);

  I2C_Send7bitAddress(I2C1,SlaveAddress,I2C_Direction_Transmitter);

	IIC_ERROR_CHECK(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED), 12);

  I2C_SendData(I2C1,REG_Address);

	IIC_ERROR_CHECK(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED), 13);

  I2C_SendData(I2C1,REG_data);

  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED), 14);

  I2C_GenerateSTOP(I2C1,ENABLE);
  */
}


	
uint8_t I2C_ByteRead(uint8_t REG_Address)
{
	/*
	uint8_t REG_data;
	size_t	error_mark = 0;
	restart:
	error_mark = 0;

	iic_loop[0]=0;
  while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY))
	  iic_loop[0]++;

  I2C_GenerateSTART(I2C1,ENABLE);

	iic_loop[1]=0;
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))
	  iic_loop[1]++;

  I2C_Send7bitAddress(I2C1,SlaveAddress,I2C_Direction_Transmitter);

	iic_loop[2]=0;
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	  iic_loop[2]++;

  I2C_Cmd(I2C1,ENABLE);

  I2C_SendData(I2C1,REG_Address);

  
	iic_loop[3]=0;
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	  iic_loop[3]++;
  
	//IIC_ERROR_CHECK(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED), 3);

  I2C_GenerateSTART(I2C1,ENABLE);

	iic_loop[4]=0;
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))
	  iic_loop[4]++;

  I2C_Send7bitAddress(I2C1,SlaveAddress,I2C_Direction_Receiver);

	iic_loop[5]=0;
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	  iic_loop[5]++;

  I2C_AcknowledgeConfig(I2C1,DISABLE);

  I2C_GenerateSTOP(I2C1,ENABLE);

	iic_loop[6]=0;
  while(!(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)));
	  iic_loop[6]++;

  REG_data=I2C_ReceiveData(I2C1);

  return REG_data;
  */
  
	uint8_t REG_data;
	size_t	error_mark = 0;
	
	restart:
	error_mark = 0;
	
	IIC_ERROR_CHECK(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY), 1);
 // while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));

  I2C_GenerateSTART(I2C1,ENABLE);

	IIC_ERROR_CHECK(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT), 2);
 // while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));

  I2C_Send7bitAddress(I2C1,SlaveAddress,I2C_Direction_Transmitter);

	IIC_ERROR_CHECK(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED), 3);
  //while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  I2C_Cmd(I2C1,ENABLE);

  I2C_SendData(I2C1,REG_Address);

	IIC_ERROR_CHECK(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED), 4);
 // while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  I2C_GenerateSTART(I2C1,ENABLE);

	IIC_ERROR_CHECK(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT), 5);
  //while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));

  I2C_Send7bitAddress(I2C1,SlaveAddress,I2C_Direction_Receiver);

	IIC_ERROR_CHECK(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED), 6);
 // while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

  I2C_AcknowledgeConfig(I2C1,DISABLE);

  I2C_GenerateSTOP(I2C1,ENABLE);

	IIC_ERROR_CHECK(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED), 7);
  //while(!(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)));

  REG_data=I2C_ReceiveData(I2C1);

  return REG_data;
  
}

void MPU6050_Init()
{
	I2C_ByteWrite(MPU_Remove_Sleep, 0x00);//µçÔ´¸´Î»
	I2C_ByteWrite(MPU_GYRO_Smple_DIV,0x13);//·ÖÆµÊý
	I2C_ByteWrite(MPU_LOW_Pass,0x04);
	I2C_ByteWrite(MPU_GYRO_Config, 0x00);
	I2C_ByteWrite(MPU_ACC_Config,0x00);
}


unsigned int GetData(unsigned char REG_Address)
{
	char H,L;
	H=I2C_ByteRead(REG_Address);
	L=I2C_ByteRead(REG_Address+1);
	return (H<<8) + L;   //ºÏ³ÉÊý¾Ý
}
