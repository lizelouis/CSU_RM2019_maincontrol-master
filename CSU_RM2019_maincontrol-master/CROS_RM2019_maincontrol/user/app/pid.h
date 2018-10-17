#ifndef _PID_H
#define _PID_H

#include "makos_includes.h"

#define KF_Q 10
#define KF_R 10

#define max(a, b)			(a>b? a:b)
#define min(a, b)			(a<b? a:b)
#define range(x, a, b)		(min(max(x, a), b))//a-��Сֵ   b-���ֵ

typedef struct 
{
	float limit;
	float kp;
	float ki;
	float kd;
	float e0;
	float e1;
	float e2;
	float out;
} PID_IncreType;//����ʽPID

typedef struct 
{
	float limit;	//????
	float Kp;		
	float Ki;
	float Kd;
	float eSum;
	float e0;		
	float e1;		
} PID_AbsoluteType;//λ��ʽPID

extern PID_AbsoluteType Pid_speed[4];//�ٶȻ�
extern PID_IncreType Pid_current[4];//������
extern PID_AbsoluteType PID_follow;

float PID_Update_Incre(PID_IncreType* PID,float tar,float cur);
float PID_Update_Absolute2(PID_AbsoluteType* PID,float tar,float cur);
void PID_Struct_Init(PID_IncreType* PID,float kp,float ki,float kd,float out_max);
void PID_speed_Init(float kp,float ki,float kd,float out_max);
void PID_current_Init(float kp,float ki,float kd,float out_max);
void PID_Struct_Init_abs(PID_AbsoluteType* PID,float kp,float ki,float kd,float out_max);

//�������˲�
double KalmanFilter_HL(const float ResrcData, float ProcessNiose_Q, float MeasureNoise_R, float InitialPrediction);
double KalmanFilter_HR(const float ResrcData, float ProcessNiose_Q, float MeasureNoise_R, float InitialPrediction);
double KalmanFilter_BL(const float ResrcData, float ProcessNiose_Q, float MeasureNoise_R, float InitialPrediction);
double KalmanFilter_BR(const float ResrcData, float ProcessNiose_Q, float MeasureNoise_R, float InitialPrediction);


#define N 100
//��ֵ�˲�
s16 filter(s16 *data,s16 delay_time);

//��ֵ�˲�
s16 filter_centrol(s16 *data,s16 delay_time); 

#endif
