#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "gprs.h" 
#include "string.h"
int i=0;

char hand(char *ptr)
{
	if(strstr(rec_data,ptr)!=NULL)
	return 1;
	else
	return 0;
}
void clear_rec_data(void)
{
	char i;
	for(i=0;i<strlen(rec_data);i++)
	{
		rec_data[i]='0';
	}
	rec_num=0;
}
void GSM_Init(void)
{
	//握手—保证波特率一致
	Uart2SendData("AT\r\n"); 
	delay_ms(200);//延时
//	while(!hand("OK"))	//确认
//	{
//		clear_rec_data();
//		i++;
//		Uart2SendData("AT\r\n");
//		delay_ms(200);//延时

//		if(i>=5)
//		{
//			break;
//		}
//		else;
//	}
	//删除存储的GSM模块返回数据，便于以后继续判断
//	clear_rec_data();
	//信号质量
	Uart2SendData("AT+CSQ\r\n");
	delay_ms(200);//延时
//	i=0;
	//检测此条指令GSM模块是否执行OK
//	while(!hand("OK")) //确认
//	{
//		clear_rec_data();
//		i++;
//		Uart2SendData("AT+CSQ\r\n");//
//		delay_ms(200);//延时
//		if(i>=5)
//		{
//			break;
//		}
//		else;
//	}
//	clear_rec_data();
//	delay_ms(800);//延时
	//查看是否读到手机卡
	Uart2SendData("AT+CPIN?\r\n");
	delay_ms(200);//延时
//	i=0;
	//检测SIM模块是否收到SIM卡READY
//	while(!hand("READY"))  //确认
//	{
//		clear_rec_data();
//		i++;
//		Uart2SendData("AT+CPIN?\r\n");//是否注册到网络
//		delay_ms(200);//延时
//		if(i>=5)
//		{
//			break;
//		}
//		else;
//	}
	Uart2SendData("AT+COPS?\r\n");//是否注册到网络
	delay_ms(200);//延时
//	i=0;
	//检测是否收到模块返回的CHINA
//	while(!hand("CHINA")) 	   //确认
//	{
//		clear_rec_data();
//		i++;
//		Uart2SendData("AT+COPS?\r\n");//是否注册到网络
//		delay_ms(200);//延时
//		if(i>=5)
//		{
//			break;
//		}
//		else;
//	}
//	clear_rec_data();
//	delay_ms(200);//延时
}
void GSM_Send(u8 *p1,u8 *p2)
{
	//！！！发短信(英文)！！！
//	Uart2SendData("AT+CMMI=2,1\r\n"); //开回显
//	delay_ms(200);//延时
	Uart2SendData("AT+CSCS=\"GSM\"\r\n");
	delay_ms(200);//延时
	Uart2SendData("AT+CMGF=1\r\n"); //方式1
	delay_ms(200);//延时
	  //！！!对方手机号！！!
	Uart2SendData(p1); 
	delay_ms(200);//延时
	  //！！！短息内容！！！
	Uart2SendData(p2);
	delay_ms(200);//延时
	USART_SendData(USART2,0X1A);//发送
	delay_ms(200);//延时
	//发送英文短信结束
	Uart2SendData("AT+CMGD=1\r\n");//删除SIM卡中的内容
	delay_ms(200);//延时
}
