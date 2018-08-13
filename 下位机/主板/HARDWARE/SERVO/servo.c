#include "servo.h"
#include "delay.h"
int a[11];
int b[11];
int c[9];
int d[8];

void SetServoLimit( char id,unsigned short int cw_limit,unsigned short int ccw_limit) //舵机模式切换
{ 
	u16 i;
	unsigned short int temp_ccw=0;
	unsigned short int temp_cw=0;
	unsigned char temp_ccw_h=0;
	unsigned char temp_ccw_l=0;
	unsigned char temp_cw_h=0;
	
	unsigned char temp_cw_l=0;
	unsigned char temp_sum1=0;
	if(ccw_limit>1023)
	{
		temp_ccw=1023;
	}
	else
	{
		temp_ccw=ccw_limit;
	}
	if(cw_limit>1023)
	{
		temp_cw=1023;
	}
	else
	{
		temp_cw=cw_limit;
	}
	temp_ccw_h=(unsigned char)(temp_ccw>>8);
	temp_ccw_l=(unsigned char)temp_ccw;
	temp_cw_h=(unsigned char)(temp_cw>>8);
	temp_cw_l=(unsigned char)temp_cw;
	
	a[0]=0xff;
	a[1]=0xff;
	a[2]=id;
	a[3]=7;
	a[4]=0x03;
	a[5]=0x06;
	a[6]=temp_cw_l;
	a[7]=temp_cw_h;
	a[8]=temp_ccw_l;
	a[9]=temp_ccw_h;
	temp_sum1=id+7+0x03+0x06+temp_cw_l+temp_cw_h+temp_ccw_l+temp_ccw_h;
	temp_sum1=~temp_sum1;
	a[10]=temp_sum1;
	for(i=0;i<11;i++)
	{
	  USART_SendData(USART2,a[i]);//向串口2发送数据
	  delay_ms(2);
	  while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
	}
}
	
		
void SetServoPosition( char id,unsigned short int position,unsigned short int velocity) //舵机模式设置
{
	u16 k=0;
	unsigned short int temp_velocity=0;
	unsigned short int temp_position=0;
	unsigned char temp_velocity_h=0;
	unsigned char temp_velocity_l=0;
	unsigned char temp_position_h=0;
	unsigned char temp_position_l=0;
	unsigned char temp_sum=0;
	if(velocity>1023)
	{
		temp_velocity=1023;
	}
	else
	{
		temp_velocity=velocity;
	}
	if(position>1023)
	{
		temp_position=1023;
	}
	else
	{
		temp_position=position;
	}
	temp_velocity_h=(unsigned char)(temp_velocity>>8);
	temp_velocity_l=(unsigned char)temp_velocity;
	temp_position_h=(unsigned char)(temp_position>>8);
	temp_position_l=(unsigned char)temp_position;

	b[0]=0xff;
	b[1]=0xff;
	b[2]=id;
	b[3]=0x07;
	b[4]=0x03;
	b[5]=0x1e;
	b[6]=temp_position_l;
	b[7]=temp_position_h;
	b[8]=temp_velocity_l;
	b[9]=temp_velocity_h;
	temp_sum=id+7+0x03+30+temp_position_l+temp_position_h+temp_velocity_l+temp_velocity_h;
	temp_sum=~temp_sum;
    b[10]=temp_sum;
	for(k=0;k<11;k++)
	{
		USART_SendData(USART2,b[k]);//向串口1发送数据
		delay_ms(2);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
	}
}

		
void SetServoVelocity(unsigned char id, signed short int velocity) //电机模式设置
{ 
	u16 m=0;
	unsigned char temp_sign = 0;

	unsigned short int temp_velocity = 0; 

	unsigned char temp_value_h = 0; 

	unsigned char temp_value_l = 0; 

	unsigned char temp_sum = 0; 

	if (velocity < 0)
	{
	temp_velocity = -velocity; 

	temp_sign = 1; 

	}
	else
	{
	temp_velocity = velocity;
	temp_sign = 0; 

	}
	if (temp_velocity > 1023)
	{
	temp_velocity = 1023;

	}
	temp_velocity |= (temp_sign << 10);
	temp_value_h = (unsigned char)(temp_velocity >> 8);
	temp_value_l = (unsigned char)temp_velocity;
	
	c[0]=0xff;
	c[1]=0xff;
	c[2]=id;
	c[3]=0x05;
	c[4]=0x03;
	c[5]=0x20;
	c[6]=temp_value_l;
	c[7]=temp_value_h;
	
	temp_sum=id+5+0x03+0x20+temp_value_l+temp_value_h;
	temp_sum = ~temp_sum;
	c[8]=temp_sum;
	for(m=0;m<9;m++)
	{
		USART_SendData(USART2,c[m]);//向串口1发送数据
		delay_ms(2);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
	}

}
	
void read_temp(char id)
{	 
	u16 m;
	u16 temp_sum3;
	d[0]=0xff;
	d[1]=0xff;
	d[2]=id;
	d[3]=0x04;
	d[4]=0x02;
	d[5]=0x2b;
	d[6]=0x01;
  	temp_sum3=id+0x04+0x02+0x2b+0x01;
	temp_sum3=~temp_sum3;
	d[7]=temp_sum3;
	for(m=0;m<8;m++)
	{
		USART_SendData(USART2,d[m]);//向串口1发送数据
		delay_ms(2);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
	}
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
    m=0;
}



