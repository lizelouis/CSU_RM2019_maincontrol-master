#ifndef	_IIC_SOFT_H
#define	_IIC_SOFT_H

#include  "stm32f4xx.h"

//void delay_ms(u16 ms);

void	IIC_INIT(void);

#define BITBAND(addr, bitnum)		((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)			*((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)		MEM_ADDR(BITBAND(addr, bitnum))

#define GPIOB_ODR_Addr    (GPIOB_BASE+0x14)
#define GPIOB_IDR_Addr    (GPIOB_BASE+0x10)

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)

#define SDA_IN()  {GPIOB->MODER&=0XFFFF3FFF;}
#define SDA_OUT() {GPIOB->MODER&=0XFFFF3FFF;GPIOB->MODER|=0x00007000;}

#define IIC_SCL    PBout(6)
#define IIC_SDA    PBout(7)
#define READ_SDA   PBin(7)


void I2C1_InitConfiguration(void);
void I2C1_Send_Byte(u8 txd);
u8 I2C1_Read_Byte(unsigned char ack);
void I2C1_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 I2C1_Read_One_Byte(u8 daddr,u8 addr);
unsigned char I2C1_Readkey(unsigned char I2C_Addr);
unsigned char I2C1_ReadOneByte(unsigned char I2C_Addr,unsigned char addr);
unsigned char I2C1_writeByte(unsigned char dev, unsigned char reg, unsigned char data);
u8 I2C1_writeBytes(u8 dev, u8 reg, u8 length, u8* data);
u8 I2C1_writeBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data);
u8 I2C1_writeBit(u8 dev,u8 reg,u8 bitNum,u8 data);
u8 I2C1_readBytes(u8 dev, u8 reg, u8 length, u8 *data);
u8 I2C1_readByte(u8 dev, u8 reg);

void I2C_writeByte(u8 dev, u8 reg, u8 data);
void I2C_writeBytes(u8 dev, u8 reg, u8 length, u8* data);
void I2C_readByte(u8 dev, u8 reg, u8* data);
void I2C_readBytes(u8 dev, u8 reg, u8 length, u8 *data);

void I2C_writeBit(u8 dev, u8 reg, u8 bitnum, u8 data);
void I2C_writeBits(u8 dev, u8 reg, u8 bitStart, u8 length, u8 data);
void I2C_readBit(u8 dev, u8 reg, u8 bitnum, u8* data);
void I2C_readBits(u8 dev, u8 reg, u8 bitStart, u8 length, u8 *data);

void I2C_writeWord(u8 dev, u8 reg, u16 data);


#endif
