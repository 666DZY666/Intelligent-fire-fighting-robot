#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 
#include "gprs.h" 

u8  m=0;
u8	a[]="AT+CMGS=\"15038018554\"\r\n";
u8	b0[]="!!!fire fire fire!!!";
u8	b1[]="!!return!!!";


int main(void)
{			
	delay_init();	     //��ʱ��ʼ�� 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
	uart2_init(9600);  
	GSM_Init();	
	while(1)
	{
		if(USART_RX_STA==1)                      //�ж����ߴ����Ƿ����
		{
			m=USART_ReceiveData(USART1);
			USART_RX_STA=0;
			
			switch(m)                            //�ж����ߴ��ڽ������
			{
				case '0':m=0;GSM_Send(a,b0);delay_ms(1000);GSM_Send(a,b0);delay_ms(1000);GSM_Send(a,b0);break;
				case '1':m=0;GSM_Send(a,b0);delay_ms(1000);GSM_Send(a,b0);delay_ms(1000);GSM_Send(a,b0);break;
				case '2':m=0;GSM_Send(a,b0);delay_ms(1000);GSM_Send(a,b0);delay_ms(1000);GSM_Send(a,b0);break;
				case '3':m=0;GSM_Send(a,b1);delay_ms(1000);GSM_Send(a,b1);delay_ms(1000);GSM_Send(a,b1);break;
				default :m=0;break;
			}
	    }
	}
}
