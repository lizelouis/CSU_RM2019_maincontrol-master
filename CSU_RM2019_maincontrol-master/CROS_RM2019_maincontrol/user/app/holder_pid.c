#include "holder_pid.h"
#include "iic.h"
extern float gyro[4];

void	holder_pid_init_absolute(PID_AbsoluteType* PID,\
		float kp, float ki, float kd, float errlimit)
{
	PID->kp		= kp;
	PID->ki		= ki;
	PID->kd		= kd;
	PID->errLim 	= errlimit;
	PID->errNow= 0;
	PID->errP= 0;
	PID->errI= 0;
	PID->errD= 0;
	PID->errOld= 0;
	PID->ctrOut= 0;
}

void	holder_pid_init_increment(PID_IncrementType* PID,\
		float kp, float ki, float kd, float out)
{
	PID->ki		= ki;
	PID->kp		= kp;
	PID->kd		= kd;
	PID->ctrOut	= out;
}



void PID_Init(PID_AbsoluteType* PID)//位置环
{
	PID->ki = 0.002;//0.005
	PID->kp = 1.2;//0.4
	PID->kd = 0;//16
	PID->errLim = 2000;
	PID->errNow= 0;
	PID->errP= 0;
	PID->errI= 0;
	PID->errD= 0;
	PID->errOld= 0;
	PID->ctrOut= 0;
}
void PID_INIT(PID_IncrementType* PID)//速度环
{
	 PID->kp = 3;     //比例系数   10v7
   PID->ki = 0.005;     //积分系数  0.01 0.01 0.004
   PID->kd = 0;     //微分系数 12 4
   PID->ctrOut = 0;//控制输出
}
void PID_INIT_P(PID_IncrementType* PID)//速度环
{
	PID->kp = 0.3;
	PID->ki = 0.0045;
	PID->kd = 0;
	PID->ctrOut = 1400;
}
void PID_Init_P(PID_AbsoluteType* PID)//位置环
{
	PID->ki = 0.0055;//0.005
	PID->kp = 0.5;//0.4
	PID->kd = 0;//16
	PID->errLim = 2000;
	PID->errNow= 0;
	PID->errP= 0;
	PID->errI= 0;
	PID->errD= 0;
	PID->errOld= 0;
	PID->ctrOut= 0;
}

#include "makos_type.h"

int16_t PID_Update_Yaw(int16_t Target,int16_t Current,PID_AbsoluteType* PID)
{
	int16_t temp;
	S32	t = (S32)Target;
	S32	c = (S32)Current;
	
	PID->errNow = t - c;
  if(PID->kp      < 0)    PID->kp      = -PID->kp;
  if(PID->ki      < 0)    PID->ki      = -PID->ki;
  if(PID->kd      < 0)    PID->kd      = -PID->kd;
  if(PID->errLim < 0)    PID->errLim = -PID->errLim;

  PID->errP = PID->errNow;  //读取现在的误差，用于kp控制

  PID->errI += PID->errNow; //误差积分，用于ki控制

  if(PID->errLim != 0)	   //微分上限和下限
  {
   if(     PID->errI >  PID->errLim)    PID->errI =  PID->errLim;
   else if(PID->errI <  -PID->errLim)    PID->errI = -PID->errLim;
  }
 
  PID->errD = PID->errNow - PID->errOld;//误差微分，用于kd控制

  PID->errOld = PID->errNow;	//保存现在的误差
 
  PID->ctrOut = PID->kp * PID->errP + PID->ki * PID->errI + PID->kd * PID->errD;//计算绝对式PID输出
	
	return PID->ctrOut;
}

int16_t PID_IncrementMode_Yaw(int16_t Target,int16_t Current,PID_IncrementType* PID)
{
 float dErrP, dErrI, dErrD;
	S16	out;
	S32	dout;
	
 PID->errNow = Target - Current;
 if(PID->kp < 0)    PID->kp = -PID->kp;
 if(PID->ki < 0)	PID->ki = -PID->ki;
 if(PID->kd < 0)    PID->kd = -PID->kd;

 dErrP = PID->errNow - PID->errOld1;

 dErrI = PID->errNow;

 dErrD = PID->errNow - 2 * PID->errOld1 + PID->errOld2;


 /*增量式PID计算*/
 PID->dCtrOut = PID->kp * dErrP + PID->ki * dErrI + PID->kd * dErrD;
	
 PID->errOld2 = PID->errOld1; //二阶误差微分
 PID->errOld1 = PID->errNow;  //一阶误差微分

	dout = (S32)PID->dCtrOut;
 if(dout > 800) dout = 800;
	else if(dout < -800) dout = -800;
	PID->dCtrOut = (float)dout;
 
  PID->ctrOut += PID->dCtrOut;
	out = (S16)((S32)PID->ctrOut);
	if(out >3000) out = 3000;
	else if(out < -3000) out = -3000;
	PID->ctrOut = (float)((S32)out);
 return out;
}



int16_t  KalmanFilter(const float ResrcData, float ProcessNiose_Q, float MeasureNoise_R, float InitialPrediction)
{
	float R = MeasureNoise_R;//测量噪声
	float Q = ProcessNiose_Q;//过程噪声

	static        float x_last;

	float x_mid = x_last;
	float x_now;

	static        float p_last;

	float p_mid;
	float p_now;
	float kg;

	x_mid = x_last; //x_last=x(k-1|k-1),x_mid=x(k|k-1)
	p_mid = p_last + Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=噪声
	kg = p_mid / (p_mid + R); //kg为kalman filter，R为噪声
	x_now = x_mid + kg*(ResrcData - x_mid);//估计出的最优值

	p_now = (1 - kg)*p_mid;//最优值对应的covariance

	p_last = p_now; //更新covariance值
	x_last = x_now; //更新系统状态值

	return x_now;
}
