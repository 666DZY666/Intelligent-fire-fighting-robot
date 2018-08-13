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
	delay_init();	     //延时初始化 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(9600);	 //串口初始化为9600
	uart2_init(9600);  
	GSM_Init();	
	while(1)
	{
		if(USART_RX_STA==1)                      //判断无线串口是否接收
		{
			m=USART_ReceiveData(USART1);
			USART_RX_STA=0;
			
			switch(m)                            //判断无线串口接收情况
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
