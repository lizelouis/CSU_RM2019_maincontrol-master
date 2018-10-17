#include "iic_soft.h"

#include "stm32f4xx.h"

#include "MPU6050.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "helper_3dmath.h"

void			IIC_INIT(void)
{
#if				(MCU_TYPE == MAIN)

#elif			(MCU_TYPE == HOLDER)
	I2C1_InitConfiguration();
	MPU6050(0xD0);
	MPUinitialize();
	while(MPUtestConnection()==0)
	delay_ms(500);
	MPUdmpInitialize();
	MPUsetDMPEnabled(TRUE);
#endif
}

void delay_nop(void)
{
  unsigned int i=100; //i=10延时1.5us//这里可以优化速度 ，经测试最低到5还能写入
   while(i--);

}

void I2C1_InitConfiguration(void)
{			
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}

void I2C1_Start(void)
{
	SDA_OUT();
	delay_nop();
	IIC_SDA=1;
	delay_nop();
	IIC_SCL=1;
	delay_nop();
 	IIC_SDA=0;
	delay_nop();
	IIC_SCL=0;
	delay_nop();
}

void I2C1_Stop(void)
{
	SDA_OUT();
	delay_nop();
	IIC_SCL=0;
	delay_nop();
	IIC_SDA=0;
	delay_nop();
	IIC_SCL=1;
	delay_nop();
	IIC_SDA=1;
	delay_nop();
}

u8 I2C1_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();
	delay_nop();
	IIC_SDA=1;
	delay_nop();
	IIC_SCL=1;
	delay_nop();
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>50)
		{
			I2C1_Stop();
			return 1;
		}
	}
	IIC_SCL=0;
	delay_nop();
	return 0;  
} 

void I2C1_Ack(void)
{
	IIC_SCL=0;
	delay_nop();
	SDA_OUT();
	delay_nop();
	IIC_SDA=0;
	delay_nop();
	IIC_SCL=1;
	delay_nop();
	IIC_SCL=0;
	delay_nop();
}

void I2C1_NAck(void)
{
	IIC_SCL=0;
	delay_nop();
	SDA_OUT();
	delay_nop();
	IIC_SDA=1;
	delay_nop();
	IIC_SCL=1;
	delay_nop();
	IIC_SCL=0;
	delay_nop();
}

void I2C1_Send_Byte(u8 txd)
{                        
	u8 t;   
	SDA_OUT();
	delay_nop();
	IIC_SCL=0;
	delay_nop();
	for(t=0;t<8;t++)
	{              
		IIC_SDA=(txd&0x80)>>7;
	delay_nop();
		txd<<=1; 	  
		IIC_SCL=1;
	delay_nop();
		IIC_SCL=0;
	delay_nop();	
	}	 
}

u8 I2C1_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();
	delay_nop();
	for(i=0;i<8;i++ )
	{
		IIC_SCL=0;
	delay_nop();
		IIC_SCL=1;
	delay_nop();
		receive<<=1;
		if(READ_SDA)receive++;
	}					 
	if (ack)
		I2C1_Ack();
	else
		I2C1_NAck(); 
	return receive;
}

unsigned char I2C1_ReadOneByte(unsigned char I2C_Addr,unsigned char addr)
{
	unsigned char res=0;
	
	I2C1_Start();	
	I2C1_Send_Byte(I2C_Addr); 
	res++;
	I2C1_Wait_Ack();
	I2C1_Send_Byte(addr); res++; 
	I2C1_Wait_Ack();	  
	I2C1_Start();
	I2C1_Send_Byte(I2C_Addr+1); res++; 
	I2C1_Wait_Ack();
	res=I2C1_Read_Byte(0);	   
	I2C1_Stop(); 
	return res;
}

u8 I2C1_readBytes(u8 dev, u8 reg, u8 length, u8 *data)
{
	u8 count = 0;
	I2C1_Start();
	I2C1_Send_Byte(dev); 
	I2C1_Wait_Ack();
	I2C1_Send_Byte(reg); 
	I2C1_Wait_Ack();	  
	I2C1_Start();
	I2C1_Send_Byte(dev+1); 
	I2C1_Wait_Ack();
	
	for(count=0;count<length;count++)
	{
		if(count!=length-1)
			data[count]=I2C1_Read_Byte(1); 
		else  
			data[count]=I2C1_Read_Byte(0); 
	}
	I2C1_Stop(); 
	return count;
}

u8 I2C1_writeBytes(u8 dev, u8 reg, u8 length, u8* data)
{
	u8 count = 0;
	I2C1_Start();
	I2C1_Send_Byte(dev);	 
	I2C1_Wait_Ack();
	I2C1_Send_Byte(reg); 
	I2C1_Wait_Ack();	  
	for(count=0;count<length;count++)
	{
		I2C1_Send_Byte(data[count]); 
		I2C1_Wait_Ack(); 
	 }
	I2C1_Stop(); 
	return 1; 
}

u8 I2C1_readByte(u8 dev, u8 reg)
{
	return I2C1_ReadOneByte(dev, reg);
}

unsigned char I2C1_writeByte(unsigned char dev, unsigned char reg, unsigned char data)
{
	unsigned char b;
	b = I2C1_writeBytes(dev, reg, 1, &data);
  return b;
}
/*
u8 I2C1_writeBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
{
	u8 b;
	if (I2C1_readByte(dev, reg, &b) != 0) 
	{
		u8 mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);	
		data <<= (8 - length);
		data >>= (7 - bitStart);
		b &= mask;
		b |= data;
		return I2C1_writeByte(dev, reg, b);
	} 
	else 
	{
		return 0;
	}
}
*/
/*
u8 I2C1_writeBit(u8 dev, u8 reg, u8 bitNum, u8 data)
{
	u8 b;
	I2C1_readByte(dev, reg, &b);
	b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
	return I2C1_writeByte(dev, reg, b);
}
*/

void I2C_writeByte(u8 dev, u8 reg, u8 data)
{
	I2C1_writeBytes(dev, reg, 1,&data);
}

void I2C_writeBytes(u8 dev, u8 reg, u8 length, u8* data)
{
	I2C1_writeBytes(dev, reg, length, data);
}
	
void I2C_readByte(u8 dev, u8 reg, u8* data)
{
	I2C1_readBytes(dev, reg, 1, data);
}

void I2C_readBytes(u8 dev, u8 reg, u8 length, u8* data)
{
	I2C1_readBytes(dev, reg, length, data);
}

void I2C_writeBit(u8 dev, u8 reg, u8 bitnum, u8 data)
{
	u8 b;
	I2C_readByte(dev, reg, &b);
	b = (data != 0) ? (b | (1 << bitnum)) : (b & ~(1 << bitnum));
	I2C_writeByte(dev, reg, b);
}

void I2C_writeBits(u8 dev, u8 reg, u8 bitStart, u8 length, u8 data)
{
  u8 tmp,mask;
  I2C_readByte(dev, reg, &tmp);
  mask = ((1 << length) - 1) << (bitStart - length + 1);
  data <<= (bitStart - length + 1); 
  data &= mask; 
  tmp &= ~(mask); 
  tmp |= data; 
  I2C_writeByte(dev, reg, tmp);	
}

void I2C_readBit(u8 dev, u8 reg, u8 bitnum, u8* data)
{
	u8 tmp;
	I2C_readByte(dev, reg, &tmp);
	*data = tmp & (1<<bitnum);
}

void I2C_readBits(u8 dev, u8 reg, u8 bitStart, u8 length, u8 *data)
{
  u8 mask,tmp;
  I2C_readByte(dev, reg, &tmp); 
  mask = ((1 << length) - 1) << (bitStart - length + 1);
  tmp &= mask;
  tmp >>= (bitStart - length + 1);
	*data = tmp;
}

void I2C_writeWord(u8 dev, u8 reg, u16 data)
{
//	u8 count = 0;
	I2C1_Start();
	I2C1_Send_Byte(dev);	 
	I2C1_Wait_Ack();
	I2C1_Send_Byte(reg); 
	I2C1_Wait_Ack();
	I2C1_Send_Byte(data>>8); 
	I2C1_Wait_Ack();
	I2C1_Send_Byte(data<<8); 
	I2C1_Wait_Ack();
	I2C1_Stop(); ;
}


