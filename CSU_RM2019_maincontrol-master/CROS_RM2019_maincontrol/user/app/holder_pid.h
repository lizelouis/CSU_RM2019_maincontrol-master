#ifndef __PID_H__
#define __PID_H__
#include "stm32f4xx.h"

typedef struct 
{
	float kp;
	float ki;
	float kd;
	float errILim_up;//��������
  float errILim_down;//��������
  float errLim;
	float errNow;
	float errOld;
	float errP;
	float errI;
	float errD;
	float ctrOut;
} PID_AbsoluteType;
typedef struct 
{
 /*PID�㷨�ӿڱ��������ڸ��û���ȡ���޸�PID�㷨������*/
 float kp;     //����ϵ��
 float ki;     //����ϵ��
 float kd;     //΢��ϵ��
 
 float errNow; //��ǰ�����
 float dCtrOut;//�����������
 float  ctrOut;//�������
 
 /*PID�㷨�ڲ���������ֵ�����޸�*/
 float errOld1;
 float errOld2;
 
}PID_IncrementType;


extern
PID_AbsoluteType	pid_holder_x_position;
extern
PID_IncrementType	pid_holder_x_speed;
extern
PID_IncrementType	pid_holder_y_speed;
extern
PID_AbsoluteType	pid_holder_y_position;


void	holder_pid_init_absolute(PID_AbsoluteType* PID,\
		float kp, float ki, float kd, float errlimit);
void	holder_pid_init_increment(PID_IncrementType* PID,\
		float kp, float ki, float kd, float out);


void PID_INIT(PID_IncrementType* PID);
void PID_Init_P(PID_AbsoluteType* PID);
void PID_INIT_P(PID_IncrementType* PID);
void PID_Init(PID_AbsoluteType* PID);
int16_t  PID_IncrementMode_Yaw(int16_t Target,int16_t Current,PID_IncrementType* PID);
int16_t PID_Update_Yaw(int16_t Target,int16_t Current,PID_AbsoluteType* PID);
int16_t  KalmanFilter(const float ResrcData, float ProcessNiose_Q, float MeasureNoise_R, float InitialPrediction);
#endif
