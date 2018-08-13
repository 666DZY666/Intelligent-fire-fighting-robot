/*
	�ϡ���λ������ͨ�ţ����ڣ�|ѭ�������IO�������ȼ�������>ǰ�ߣ���ѡ�������á��ⲿ�жϣ�
*/
//���ߣ����գ�ģ�顪��UART1��TX��PA9��RX��PA10������
//��������ٶȴ�С|����TIM3(CH3��PB0��CH4��PB1) PWM|PC6��7��8��9
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

/**********��************/
//���
#define AVERAGE_TIMES 10
#define speed1		  410      		//�������������ٶ�
#define speed2		  420      		//Ѳ��
#define speed3	    460      		//���Ҽ����ת
#define speed4      510      		//ǰ����
#define speed5      500      		//��ת������

//ѭ��
	//ǰ��
#define speed6   420 
	//Сת
#define speed7   550 //������
#define speed8   0
#define t1       100  //����
	//��ת
#define speed9   600 //������
#define speed10  440
#define t3       1000
	//ʮ��·��
#define speed11  700  //������
#define speed12  520
#define t4		   520 
	//���
#define t5       830
#define speed13  550//������
#define speed14  430
	//�ƶ�
#define t2       20  

/**********����************/
u8    m=0,s=0,q=0;	
char  f=0,z=1;
int   i=0;
float pitch,roll,yaw;	//ŷ����
float angle=0;

/**********ѭ������غ�������************/
void zhidong1(void);
void zhidong2(void);
void zhidong3(void);
void dazuo(void);
void dayou(void);
//void jiaocha(void);

int main(void)
{			
	delay_init();
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	        //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 	//����1��ʼ��Ϊ9600bps
//	uart2_init(1000000);  //����2��ʼ��Ϊ1000000bps(���Ĭ��ͨ������Ϊ1000000bps)
	IO_Init();			    //IO���
	LED_Init();
	TIM1_PWM_Init(899,0);   //���װ��
	TIM4_PWM_Init(899,0);   //TIM4-CH3|4-PB8|9���ж����������Ƶ��PWMƵ��=72000/(899+1)=80Khz
	Adc_Init();			    //������
	MPU_Init();				//MPU6050��ʼ����ע�⣺TIM3��CH3|4Ӱ��IIC������ͬʱʹ�ã�����
	while(mpu_dmp_init());	//MPU6050��DMP��ʼ��
	delay_ms(800);
	while(1)
	{	
		//�ϡ���λ������ͨ��(����)
		if(USART_RX_STA==1)                      //�ж����ߴ����Ƿ����
		{
			m=USART_ReceiveData(USART1);
			USART_RX_STA=0;
			
			switch(m)                            //�ж����ߴ��ڽ������
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
			//���
			if((!f1)||(!f2)||(!f3)||f4||f5||f6||f7||f8||f9)
			{
					//��⵽����	
					if((!f1)||(!f2)||(!f3))	         						//ǰ
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
									MPU_Init();									//MPU6050���γ�ʼ��
									while(mpu_dmp_init());	//MPU6050��DMP��ʼ��
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
					else if(f4||f5||f6)										//�� 
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
					else if(f7||f8||f9)  									//��                        	  
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
					//δ��⵽���棬Ѳ��
					else  
					{ 
						fire(0);
						move(speed2,speed2);  
					}    				   
			}
			//ѭ��
			else
			{
				switch(v&0x00F0)
				{
					//ֱ��
					case	0x0000:zhidong1();f=7;move(speed6,speed6);break;
					//ʮ�ֽ���
	//				case  0x0090:zhidong2();move(0,0);delay_ms(200);jiaocha();break;
					//��
					case	0x0020:zhidong2();f=1;move(speed7,-speed8);	delay_ms(t1);break;//С��
					case	0x0010:zhidong2();move(0,0);delay_ms(150);f=2;dayou();break;//����|ֱ��
	//				case	0x0030:zhidong2();move(-50,-50);delay_ms(200);f=2;move(speed13,-speed14);for(i=0;(i<t5)&&(!((v&0x00F0)==0x0020));i++)delay_ms(1);break;//��-���
					//��
					case	0x0040:zhidong2();f=4;move(-speed8,speed7);	delay_ms(t1);break;//С��
					case	0x0080:zhidong2();move(0,0);delay_ms(150);f=5;dazuo();break;//����|ֱ��
	//				case	0x00C0:zhidong2();move(-50,-50);delay_ms(200);f=5;move(-speed14,speed13);for(i=0;(i<t5)&&(!((v&0x00F0)==0x0040));i++)delay_ms(1);break;//��-���
					default:move(420,420); break;
				}
			}
		}
	}
}

//ѭ������غ�������
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

