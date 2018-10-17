//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : HuangKai
//  生成日期   : 2014-0101
//  最近修改   : 
//  功能描述   : OLED 4接口演示例程(51系列)
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              D0   接PD6（SCL）
//              D1   接PD7（SDA）
//              RES  接PD4
//              DC   接PD5
//              CS   接PD3               
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//Copyright(C) 中景园电子2014/3/16
//All rights reserved
//******************************************************************************/

#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 

//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   

#if OLED_MODE==1
//向SSD1106写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat,u8 cmd)
{
	DATAOUT(dat);	    
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		   
	OLED_CS_Clr();
	OLED_WR_Clr();	 
	OLED_WR_Set();
	OLED_CS_Set();	  
	OLED_DC_Set();	 
} 	    	    
#else
//向SSD1106写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat,u8 cmd,u8 type)
{	
	u8 i;			  
	if(!type)
	{
		dat = ~dat;
	}
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();	
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_CS_Set();
	OLED_DC_Set();   	  
} 
#endif
	void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD,1);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD,1);
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD,1); 
}   	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD,1);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD,1);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD,1);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD,1);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD,1);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD,1);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD,1);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD,1);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD,1);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA,1); 
	} //更新显示
}


//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 type)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(SIZE ==16)
		{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
				OLED_WR_Byte(F8X16[c*16+i],OLED_DATA,type);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
				OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA,type);
		}
		else 
		{	
			OLED_Set_Pos(x,y+1);
			for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA,type);				
		}
}
//m^n函数
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size,u8 type)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',type);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',type); 
	}
} 
//显示一个字符号串
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 type)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{	
		if(!type)
			OLED_ShowChar(x,y,chr[j],0);
		else
			OLED_ShowChar(x,y,chr[j],1);
		x+=6;
		if(x>122){x=0;y+=2;}
			j++;
	}
}

/*
??:?????
??:
*/
void OLED_PrintVar(uint8_t x, uint8_t y, double data, uint8_t Length, uint8_t num)
{
  uint8_t StrTemp[20] = {0};
  uint8_t Site = 0;
  int bit = 0;
  int32_t datatemp = 0;
   
  while(Site < num)
  {
    data *= 10;
    Site++;
  }
  datatemp = (int32_t)data;
  
  Site = 0;
  if(datatemp < 0)
  {
    StrTemp[Site] = '-';
    datatemp = -datatemp;
    Site++;   
  }
  else if(datatemp == 0)
  {
    StrTemp[Site] = '0';
    Site++;
  }
  
  bit = 0;
  int32_t d = datatemp;
  while(d > 0)
  {
    d /= 10;
    bit++;                   /////?????????
  }
  
  uint8_t dotsite = 0; ////?????
  if(bit <= num)
  {
    StrTemp[Site] = '0';
    Site++;
    dotsite = 0;
    bit = num;
  }
  else
  {
    dotsite = bit - num;
   // bit;
  }
  
  StrTemp[Site + bit + 1] = '\0';
  while(bit >= 0)
  {
    if(bit == dotsite)
    {
      StrTemp[Site + bit] = '.';
    }
    else
    {
      StrTemp[Site + bit] = datatemp % 10 + '0';
      datatemp /= 10;
    }
    bit--;
  }
  OLED_ShowString(x, y, StrTemp, 1);
}

//显示汉字
void OLED_ShowCHinese(u8 x,u8 y,u8 no,u8 type)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA,type);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA,type);
				adder+=1;
      }					
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[],u8 type)
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA,type);	    	
	    }
	}
} 


//初始化SSD1306					    
void OLED_Init(void)
{ 	
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOG, ENABLE);	 //使能PC,D,G端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_3|GPIO_Pin_8;	 //PD3,PD6推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		 //推挽输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  //初始化GPIOD3,6
 	GPIO_SetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_3|GPIO_Pin_8);	//PD3,PD6 输出高

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_6;	 //PE4,PE6推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		 //推挽输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOE, &GPIO_InitStructure);	  
 	GPIO_SetBits(GPIOE,GPIO_Pin_4|GPIO_Pin_6);	//PE4,PE6 输出高

	
 #if OLED_MODE==1
 
 	GPIO_InitStructure.GPIO_Pin =0xFF; //PC0~7 OUT推挽输出
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOC,0xFF); //PC0~7输出高

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //PG13,14,15 OUT推挽输出
 	GPIO_Init(GPIOG, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOG,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);						 //PG13,14,15 OUT  输出高

 #else
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;				 //PC0,1 OUT推挽输出
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1);						 //PC0,1 OUT  输出高

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				 //PG15 OUT推挽输出	  RST
 	GPIO_Init(GPIOG, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOG,GPIO_Pin_15);						 //PG15 OUT  输出高


 #endif

 
  OLED_RST_Set();
	delay_ms(100);
	OLED_RST_Clr();
	delay_ms(100);
	OLED_RST_Set(); 
					  
	OLED_WR_Byte(0xAE,OLED_CMD,1);//--turn off oled panel
	OLED_WR_Byte(0x00,OLED_CMD,1);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD,1);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD,1);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD,1);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD,1); // Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD,1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC8,OLED_CMD,1);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_Byte(0xA6,OLED_CMD,1);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD,1);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD,1);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD,1);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD,1);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD,1);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD,1);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD,1);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD,1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD,1);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD,1);
	OLED_WR_Byte(0xDB,OLED_CMD,1);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD,1);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD,1);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD,1);//
	OLED_WR_Byte(0x8D,OLED_CMD,1);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD,1);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD,1);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD,1);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD,1);//--turn on oled panel
	
	OLED_WR_Byte(0xAF,OLED_CMD,1); /*display ON*/ 
	OLED_Clear();
	OLED_Set_Pos(0,0); 	
}  





























