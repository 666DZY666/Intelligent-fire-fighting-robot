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
	//���֡���֤������һ��
	Uart2SendData("AT\r\n"); 
	delay_ms(200);//��ʱ
//	while(!hand("OK"))	//ȷ��
//	{
//		clear_rec_data();
//		i++;
//		Uart2SendData("AT\r\n");
//		delay_ms(200);//��ʱ

//		if(i>=5)
//		{
//			break;
//		}
//		else;
//	}
	//ɾ���洢��GSMģ�鷵�����ݣ������Ժ�����ж�
//	clear_rec_data();
	//�ź�����
	Uart2SendData("AT+CSQ\r\n");
	delay_ms(200);//��ʱ
//	i=0;
	//������ָ��GSMģ���Ƿ�ִ��OK
//	while(!hand("OK")) //ȷ��
//	{
//		clear_rec_data();
//		i++;
//		Uart2SendData("AT+CSQ\r\n");//
//		delay_ms(200);//��ʱ
//		if(i>=5)
//		{
//			break;
//		}
//		else;
//	}
//	clear_rec_data();
//	delay_ms(800);//��ʱ
	//�鿴�Ƿ�����ֻ���
	Uart2SendData("AT+CPIN?\r\n");
	delay_ms(200);//��ʱ
//	i=0;
	//���SIMģ���Ƿ��յ�SIM��READY
//	while(!hand("READY"))  //ȷ��
//	{
//		clear_rec_data();
//		i++;
//		Uart2SendData("AT+CPIN?\r\n");//�Ƿ�ע�ᵽ����
//		delay_ms(200);//��ʱ
//		if(i>=5)
//		{
//			break;
//		}
//		else;
//	}
	Uart2SendData("AT+COPS?\r\n");//�Ƿ�ע�ᵽ����
	delay_ms(200);//��ʱ
//	i=0;
	//����Ƿ��յ�ģ�鷵�ص�CHINA
//	while(!hand("CHINA")) 	   //ȷ��
//	{
//		clear_rec_data();
//		i++;
//		Uart2SendData("AT+COPS?\r\n");//�Ƿ�ע�ᵽ����
//		delay_ms(200);//��ʱ
//		if(i>=5)
//		{
//			break;
//		}
//		else;
//	}
//	clear_rec_data();
//	delay_ms(200);//��ʱ
}
void GSM_Send(u8 *p1,u8 *p2)
{
	//������������(Ӣ��)������
//	Uart2SendData("AT+CMMI=2,1\r\n"); //������
//	delay_ms(200);//��ʱ
	Uart2SendData("AT+CSCS=\"GSM\"\r\n");
	delay_ms(200);//��ʱ
	Uart2SendData("AT+CMGF=1\r\n"); //��ʽ1
	delay_ms(200);//��ʱ
	  //����!�Է��ֻ��ţ���!
	Uart2SendData(p1); 
	delay_ms(200);//��ʱ
	  //��������Ϣ���ݣ�����
	Uart2SendData(p2);
	delay_ms(200);//��ʱ
	USART_SendData(USART2,0X1A);//����
	delay_ms(200);//��ʱ
	//����Ӣ�Ķ��Ž���
	Uart2SendData("AT+CMGD=1\r\n");//ɾ��SIM���е�����
	delay_ms(200);//��ʱ
}
