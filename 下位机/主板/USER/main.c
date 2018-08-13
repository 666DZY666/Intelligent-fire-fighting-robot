/*
	上、下位机无线通信（串口）|循迹、灭火（IO）—优先级：后者>前者（备选：灭火采用—外部中断）
*/
//无线（接收）模块——UART1（TX—PA9、RX—PA10）接收
//电机——速度大小|方向—TIM3(CH3—PB0、CH4—PB1) PWM|PC6、7、8、9
#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h"
#include "timer.h" 
#include "pwm.h" 
#include "servo.h"
#include "io.h"
#include "adc.h"
#include "myiic.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 

/**********宏************/
//灭火
#define AVERAGE_TIMES 10
#define speed1		  410      		//正面趋近火焰速度
#define speed2		  420      		//巡逻
#define speed3	    460      		//左右检测旋转
#define speed4      510      		//前矫正
#define speed5      500      		//旋转进入轨道

//循迹
	//前进
#define speed6   420 
	//小转
#define speed7   550 //主动轮
#define speed8   0
#define t1       100  //正常
	//大转
#define speed9   600 //主动轮
#define speed10  440
#define t3       1000
	//十字路口
#define speed11  700  //主动轮
#define speed12  520
#define t4		   520 
	//锐角
#define t5       830
#define speed13  550//主动轮
#define speed14  430
	//制动
#define t2       20  

/**********变量************/
u8    m=0,s=0,q=0;	
char  f=0,z=1;
int   i=0;
float pitch,roll,yaw;	//欧拉角
float angle=0;

/**********循迹—相关函数声明************/
void zhidong1(void);
void zhidong2(void);
void zhidong3(void);
void dazuo(void);
void dayou(void);
//void jiaocha(void);

int main(void)
{			
	delay_init();
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	        //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 	//串口1初始化为9600bps
//	uart2_init(1000000);  //串口2初始化为1000000bps(舵机默认通信速率为1000000bps)
	IO_Init();			    //IO检测
	LED_Init();
	TIM1_PWM_Init(899,0);   //灭火装置
	TIM4_PWM_Init(899,0);   //TIM4-CH3|4-PB8|9—行动电机—不分频。PWM频率=72000/(899+1)=80Khz
	Adc_Init();			    //红外测距
	MPU_Init();				//MPU6050初始化—注意：TIM3—CH3|4影响IIC，不能同时使用！！！
	while(mpu_dmp_init());	//MPU6050—DMP初始化
	delay_ms(800);
	while(1)
	{	
		//上、下位机无线通信(串口)
		if(USART_RX_STA==1)                      //判断无线串口是否接收
		{
			m=USART_ReceiveData(USART1);
			USART_RX_STA=0;
			
			switch(m)                            //判断无线串口接收情况
			{
				case '0':USART_SendData(USART1,'a');delay_ms(10);USART_SendData(USART1,'a');delay_ms(10);USART_SendData(USART1,'a');
						 move(-speed5-30,speed5+30);
						 while((yaw>87)||(yaw<82))
						 {
							if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
								q=0;
						 }
					     yaw=0;move(0,0);delay_ms(10);move(speed6,speed6);m=0;s=1;while((v&0x00F0)==0x00);break;
				case '1':USART_SendData(USART1,'a');delay_ms(10);USART_SendData(USART1,'a');delay_ms(10);USART_SendData(USART1,'a');move(speed6,speed6);m=0;s=1;while((v&0x00F0)==0x00);break;
				case '2':USART_SendData(USART1,'a');delay_ms(10);USART_SendData(USART1,'a');delay_ms(10);USART_SendData(USART1,'a'); 
						 move(speed5+30,-speed5-30);
						 while((yaw>87)||(yaw<82))
						 {
							if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
								q=0;
							yaw=0-yaw;
						 }
					     yaw=0;move(0,0);delay_ms(10);move(speed6,speed6);m=0;s=1;while((v&0x00F0)==0x00);break;
				case '3':USART_SendData(USART1,'e');delay_ms(10);USART_SendData(USART1,'e');delay_ms(10);USART_SendData(USART1,'e');move(0,0);s=0;m=0;
				default : m=0;  break;
			}
	  }
		if(s==1)           						 
		{
			//灭火
			if((!f1)||(!f2)||(!f3)||f4||f5||f6||f7||f8||f9)
			{
					//检测到火焰	
					if((!f1)||(!f2)||(!f3))	         						//前
					{
						delay_ms(1);  
						if((!f1)||(!f2)||(!f3))			
						{
							if(!f2)	
							{
								delay_ms(1);
								if(!f2)
								{
									while((a<=2300)&&(b<=2200)&&(c<2200))
										move(speed1,speed1);
									fire(898);
									move(0,0);
									delay_ms(1000);
									delay_ms(1000);
									for(i=0;(i<1000)&&((!f1)||(!f2)||(!f3));i++)
										delay_ms(1);
									for(i=0;(i<1000)&&((!f1)||(!f2)||(!f3));i++)
										delay_ms(1);
									for(i=0;(i<1000)&&((!f1)||(!f2)||(!f3));i++)
										delay_ms(1);
									fire(0);
									USART_SendData(USART1,'b');delay_ms(10);USART_SendData(USART1,'b');delay_ms(10);USART_SendData(USART1,'b');
									delay_ms(500);
									USART_SendData(USART1,'d');delay_ms(10);USART_SendData(USART1,'d');delay_ms(10);USART_SendData(USART1,'d');
									move(-450,-450);
									delay_ms(200);
									move(0,0);
									MPU_Init();									//MPU6050二次初始化
									while(mpu_dmp_init());	//MPU6050—DMP初始化
									move(speed5,-speed5);
									while((yaw>85)||(yaw<78))
									{
										if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
											q=0;
										yaw=0-yaw;
									}
									yaw=0;
									move(0,0);
								}
							}
							else if((!f1)&&(f3)) 		
							{
								delay_ms(1);
								if((!f1)&&(f3))
								{
									move(0,0);
									delay_ms(50);
									move(-speed4,speed4+80);	
								}
								for(i=0;(i<2000)&&f2;i++)
									delay_ms(1);					
							}
							else if((f1)&&(!f3)) 		
							{
								delay_ms(1);
								if((f1)&&(!f3)) 	
								{
									move(0,0);
									delay_ms(50);
									move(speed4,-speed4-80);
								}
								for(i=0;(i<2000)&&f2;i++)
									delay_ms(1);	
							}
						}
					}
					else if(f4||f5||f6)										//右 
					{
						delay_ms(1);
						if(f4||f5||f6)
						{
	//						move(440,440);
	//						delay_ms(50);
							move(0,0);
							delay_ms(100);
							move(speed3,-speed3-50);
						}
						for(i=0;(i<1000)&&f2;i++)
							delay_ms(1);
					}															           
					else if(f7||f8||f9)  									//左                        	  
					{
						delay_ms(1);
						if(f7||f8||f9)  
						{						
	//						move(440,440);
	//						delay_ms(50);
							move(0,0);
							delay_ms(100);						
							move(-speed3-50,speed3);
						}
						for(i=0;(i<1000)&&f2;i++)
							delay_ms(1);
					}
					//未检测到火焰，巡逻
					else  
					{ 
						fire(0);
						move(speed2,speed2);  
					}    				   
			}
			//循迹
			else
			{
				switch(v&0x00F0)
				{
					//直行
					case	0x0000:zhidong1();f=7;move(speed6,speed6);break;
					//十字交叉
	//				case  0x0090:zhidong2();move(0,0);delay_ms(200);jiaocha();break;
					//右
					case	0x0020:zhidong2();f=1;move(speed7,-speed8);	delay_ms(t1);break;//小右
					case	0x0010:zhidong2();move(0,0);delay_ms(150);f=2;dayou();break;//大右|直角
	//				case	0x0030:zhidong2();move(-50,-50);delay_ms(200);f=2;move(speed13,-speed14);for(i=0;(i<t5)&&(!((v&0x00F0)==0x0020));i++)delay_ms(1);break;//右-锐角
					//左
					case	0x0040:zhidong2();f=4;move(-speed8,speed7);	delay_ms(t1);break;//小左
					case	0x0080:zhidong2();move(0,0);delay_ms(150);f=5;dazuo();break;//大左|直角
	//				case	0x00C0:zhidong2();move(-50,-50);delay_ms(200);f=5;move(-speed14,speed13);for(i=0;(i<t5)&&(!((v&0x00F0)==0x0040));i++)delay_ms(1);break;//左-锐角
					default:move(420,420); break;
				}
			}
		}
	}
}

//循迹—相关函数定义
void zhidong1(void)
{
	if(f)
	{
		switch(f)
		{
			case 	7:break;
			case	1:f=0;move(-speed8,speed7);delay_ms(t2);break;
			case	2:f=0;move(-speed10,speed9);delay_ms(t2);break;
//			case	3:f=0;move(0,0);delay_ms(10);break;
			case	4:f=0;move(speed7,-speed8);delay_ms(t2);break;
			case	5:f=0;move(speed9,-speed10);delay_ms(t2);break;
//			case	6:f=0;move(0,0);delay_ms(10);break;
			default:break;
		}
	}
}
void zhidong2(void)
{
	if(f)
	{
		switch(f)
		{
			case	7:f=0;move(-speed6,-speed6);delay_ms(t2);break;
			case	2:f=0;move(-speed10,speed9);delay_ms(t2);break;
			case	5:f=0;move(speed9,-speed10);delay_ms(t2);break;
			default:break;
		}
	}
}
void dazuo(void)
{
	move(-speed10,speed9);
	for(i=0;(i<t3)&&(!((v&0x00F0)==0x0040));i++)
		delay_ms(1);
}
void dayou(void)
{
	move(speed9,-speed10);
	for(i=0;(i<t3)&&(!((v&0x00F0)==0x0020));i++)
		delay_ms(1);
}
//void jiaocha(void)
//{
//		switch(z)
//		{
//			case	1:z=2;f=5;move(-speed12,speed11);delay_ms(500);for(i=0;(i<t4)&&(!((v&0x00F0)==0x0040));i++)delay_ms(1);break;
//			case	2:z=1;f=2;move(speed11,-speed12);delay_ms(500);for(i=0;(i<t4)&&(!((v&0x00F0)==0x0020));i++)delay_ms(1);break;
//		}
//}

