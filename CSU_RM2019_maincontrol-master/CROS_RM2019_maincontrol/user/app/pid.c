/*
	FILE NAME:	pid and calman.c
	DESCRIPTION:	the task dispose remote messages by the control rules
			and send the result to the relatively tasks
	
	EDIT RECORDS:
	---------
	AUTHOR:		the Seniors 
	EDIT TIME:	2017/11/18-13:47
	MODIFICATION:	built the frame of remote task, simply send the msg to chassis and holder.
	---------
	AUTHOR:		
	EDIT TIME:	
	MODIFICATION:	
	---------
*/
#include "pid.h"
//******************************************
PID_AbsoluteType Pid_speed[4];//�ٶȻ�
PID_IncreType Pid_current[4];//������
PID_AbsoluteType PID_follow;

//PID���£����������PID�ṹ�壬Ŀ��ֵ����ǰֵ��
//����ֵΪPID���
float PID_Update_Incre(PID_IncreType* PID,float tar,float cur)
{
	float ep,ei,ed;

	PID->e0 =tar - cur;  //��ȡ���ڵ�������kp����
	
	ep = PID->e0 - PID->e1;
	ei = PID->e0;
	ed = PID->e0 - 2*PID->e1 + PID->e2;
	
	PID->out += PID->kp*ep + PID->ki*ei + PID->kd*ed;
	
	PID->e2 = PID->e1;
	PID->e1 = PID->e0;	
	
	PID->out = range(PID->out, -PID->limit, PID->limit);//�����޷�
	
	return PID->out;
}

float PID_Update_Absolute2(PID_AbsoluteType* PID,float tar,float cur)
{
	float pe, ie, de;
	float out=0;
	
	PID->e0 = tar - cur;

	PID->eSum += PID->e0;
	
	de = PID->e0 - PID->e1;

	pe = PID->e0;
	ie = PID->eSum;

	PID->e1 = PID->e0;
	
	if((tar < 25) && (tar > -25))
	{
		PID->eSum = 0;
	}
	
	out = pe*(PID->Kp) + ie*(PID->Ki) + de*(PID->Kd);
	
	//????
	out = range(out, -PID->limit, PID->limit);
	return out;
}

void PID_Struct_Init(PID_IncreType* PID,float kp,float ki,float kd,float out_max)
{
	PID->kp = kp;//����
	PID->ki = ki;//����
	PID->kd = kd;//΢��
	PID->e0 = 0;
	PID->e1 = 0;
	PID->e2 = 0;
	PID->limit = out_max;
	PID->out = 0;
}

void PID_Struct_Init_abs(PID_AbsoluteType* PID,float kp,float ki,float kd,float out_max)
{
	PID->Kp = kp;//����
	PID->Ki = ki;//����
	PID->Kd = kd;//΢��
	PID->e0 = 0;
	PID->e1 = 0;
	PID->eSum = 0;
	PID->limit = out_max;
}


void PID_speed_Init(float kp,float ki,float kd,float out_max)
{
	u32 i;
	for(i = 0;i < 4; i++)
	{
		PID_Struct_Init_abs(&Pid_speed[i],kp,ki,kd,out_max);
	}
}

void PID_current_Init(float kp,float ki,float kd,float out_max)
{
	u32 i;
	for(i = 0;i < 4; i++)
	{
		PID_Struct_Init(&Pid_current[i],kp,ki,kd,out_max);
	}	
}

/*******************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------*/
/*      �������˲�
Q:����������Q���󣬶�̬��Ӧ��죬�����ȶ��Ա仵
R:����������R���󣬶�̬��Ӧ�����������ȶ��Ա��
*/

double KalmanFilter_HL(const float ResrcData, float ProcessNiose_Q, float MeasureNoise_R, float InitialPrediction)
{
	float R = MeasureNoise_R;
	float Q = ProcessNiose_Q;

	static        float x_last;

	float x_mid = x_last;
	float x_now;

	static        float p_last;

	float p_mid;
	float p_now;
	float kg;

	x_mid = x_last; //x_last=x(k-1|k-1),x_mid=x(k|k-1)
	p_mid = p_last + Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=����
	kg = p_mid / (p_mid + R); //kgΪkalman filter��RΪ����
	x_now = x_mid + kg*(ResrcData - x_mid);//���Ƴ�������ֵ

	p_now = (1 - kg)*p_mid;//����ֵ��Ӧ��covariance

	p_last = p_now; //����covarianceֵ
	x_last = x_now; //����ϵͳ״ֵ̬

	return x_now;
}


/*-------------------------------------------------------------------------------------------------------------*/
/*      �������˲�
Q:����������Q���󣬶�̬��Ӧ��죬�����ȶ��Ա仵
R:����������R���󣬶�̬��Ӧ�����������ȶ��Ա��
*/

double KalmanFilter_HR(const float ResrcData, float ProcessNiose_Q, float MeasureNoise_R, float InitialPrediction)
{
	float R = MeasureNoise_R;
	float Q = ProcessNiose_Q;

	static        float x_last;

	float x_mid = x_last;
	float x_now;

	static        float p_last;

	float p_mid;
	float p_now;
	float kg;

	x_mid = x_last; //x_last=x(k-1|k-1),x_mid=x(k|k-1)
	p_mid = p_last + Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=����
	kg = p_mid / (p_mid + R); //kgΪkalman filter��RΪ����
	x_now = x_mid + kg*(ResrcData - x_mid);//���Ƴ�������ֵ

	p_now = (1 - kg)*p_mid;//����ֵ��Ӧ��covariance

	p_last = p_now; //����covarianceֵ
	x_last = x_now; //����ϵͳ״ֵ̬

	return x_now;
}


/*-------------------------------------------------------------------------------------------------------------*/
/*      �������˲�
Q:����������Q���󣬶�̬��Ӧ��죬�����ȶ��Ա仵
R:����������R���󣬶�̬��Ӧ�����������ȶ��Ա��
*/

double KalmanFilter_BL(const float ResrcData, float ProcessNiose_Q, float MeasureNoise_R, float InitialPrediction)
{
	float R = MeasureNoise_R;
	float Q = ProcessNiose_Q;

	static        float x_last;

	float x_mid = x_last;
	float x_now;

	static        float p_last;

	float p_mid;
	float p_now;
	float kg;

	x_mid = x_last; //x_last=x(k-1|k-1),x_mid=x(k|k-1)
	p_mid = p_last + Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=����
	kg = p_mid / (p_mid + R); //kgΪkalman filter��RΪ����
	x_now = x_mid + kg*(ResrcData - x_mid);//���Ƴ�������ֵ

	p_now = (1 - kg)*p_mid;//����ֵ��Ӧ��covariance

	p_last = p_now; //����covarianceֵ
	x_last = x_now; //����ϵͳ״ֵ̬

	return x_now;
}


/*-------------------------------------------------------------------------------------------------------------*/
/*      �������˲�
Q:����������Q���󣬶�̬��Ӧ��죬�����ȶ��Ա仵
R:����������R���󣬶�̬��Ӧ�����������ȶ��Ա��
*/

double KalmanFilter_BR(const float ResrcData, float ProcessNiose_Q, float MeasureNoise_R, float InitialPrediction)
{
	float R = MeasureNoise_R;
	float Q = ProcessNiose_Q;

	static        float x_last;

	float x_mid = x_last;
	float x_now;

	static        float p_last;

	float p_mid;
	float p_now;
	float kg;

	x_mid = x_last; //x_last=x(k-1|k-1),x_mid=x(k|k-1)
	p_mid = p_last + Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=����
	kg = p_mid / (p_mid + R); //kgΪkalman filter��RΪ����
	x_now = x_mid + kg*(ResrcData - x_mid);//���Ƴ�������ֵ

	p_now = (1 - kg)*p_mid;//����ֵ��Ӧ��covariance

	p_last = p_now; //����covarianceֵ
	x_last = x_now; //����ϵͳ״ֵ̬

	return x_now;
}

//************��ֵ�˲�***************
s16 filter(s16 *data,s16 delay_time) 
{    
	int  sum = 0;
	u32 count;
	for(count = 0;count < N;count++) 
	{    
		sum += *data;
		while(delay_time)
		{
			delay_time--;
		}
	}
	return (sum/N);
}

//��λֵ�˲�               
s16 filter_centrol(s16 *data,s16 delay_time) 
{ 
  s16 value_buf[N]; 
  s16 count,i,j,temp; 
  for (count = 0;count < N;count++) 
  { 
    value_buf[count] = *data; 
		while(delay_time)
		{
			delay_time--;
		} 
   } 
  for(j = 0;j < N-1;j++) 
  { 
   for(i = 0;i < N-j;i++) 
   { 
    if( value_buf[i] > value_buf[i+1] ) 
    { 
      temp = value_buf[i]; 
			value_buf[i] = value_buf[i+1];
			value_buf[i+1] = temp; 
		} 
	 } 
	} 
	return value_buf[(N-1)/2]; 
}

