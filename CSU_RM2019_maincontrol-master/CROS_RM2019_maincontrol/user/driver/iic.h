#ifndef __IIC_H__
#define __IIC_H__
#include "stm32f4xx.h"
#define ACC_XOUT_H    0x3b
#define ACC_XOUT_L    0X3C
#define ACC_YOUT_H    0x3D
#define ACC_YOUT_L    0X3E
#define ACC_ZOUT_H    0x3F
#define ACC_ZOUT_L    0X40
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define SlaveAddress 0XD0
#define MPU_Remove_Sleep 0x6B
#define MPU_GYRO_Smple_DIV  0x19
#define MPU_LOW_Pass        0x1a
#define MPU_GYRO_Config     0x1b
#define MPU_ACC_Config      0x1c



void IIC_Config(void);
uint8_t I2C_ByteRead(uint8_t REG_Address);
void I2C_ByteWrite(uint8_t REG_Address,uint8_t REG_data);
void MPU6050_Init(void);
unsigned int GetData(unsigned char REG_Address);


#endif
