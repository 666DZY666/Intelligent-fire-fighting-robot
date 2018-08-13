//2017������ơ���������������ϵͳ����λ�����ۺϣ�������ʶ��+������λ

//����ʶ��RGB��>RGB+HSI����>Ԥ����-ƽ���˲�|��ֵ�˲���>��̬ѧ����-����|��ʴ��ͼ��ȥ�롢�ָ������֮���޷�ʶ��ʵ��������ͨ����
//������λ����λ��+��λ����MPU6050��
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core/core.hpp"  
#include "highgui/highgui.hpp"  
#include "imgproc/imgproc.hpp"  
#include<iostream>
#include "opencv2/objdetect/objdetect.hpp"  //����OpenCV2.4.9������
//���빤���ռ�
using namespace cv;  
using namespace std;
//�����ֵ�궨��
  //������
#define RT  80     //90~135  
#define ST  45     //45~65  
  //�������
#define BT  75	   //>80
#define ST1 0.01
//��������
Point origin;
Point Pt1;
Point Pt;
Point Pt2;
Point Pt3;
Point Pt4;
Point Pt5;
Point Pt6;
Point Pt7;
Point Pt8;
IplImage* a;
IplImage* b;
IplImage* c;
IplImage* e;
Mat matImage;
Mat image;
char ax[20],ay[20];
char RXBuff;
char u=0;
DWORD dwError = 0;
DWORD BytesRead = 0;
Rect juxin1;  
Rect juxin2; 
Rect juxin3; 
Rect juxin4; 
bool juxin_flag=false;
int time=0;
int d[20];
int n=0,i,j,flag=0,k=2,m=0;
int color=0;
Mat frame;         //����һ��Mat�࣬���ڴ洢ÿһ֡ͼ��
HANDLE hComm;
OVERLAPPED m_ov;
COMSTAT comstat;
DWORD m_dwCommEvents;
//��������
int WriteChar(char* m_szWriteBuffer,DWORD m_nToSend);
bool openport(char *portname);
bool setupdcb(int rate_arg);
bool setuptimeout(DWORD ReadInterval, DWORD ReadTotalMultiplier,DWORD ReadTotalconstant, DWORD WriteTotalMultiplier,DWORD WriteTotalconstant);
void closeport();
void ReceiveChar();
void Uart_Init(char *portname,int dcb);
static void onMouse( int event, int x, int y, int flags, void* );
static void ShowHelpText();
//������
void main()
{	
	char f=1;
	char f0=0;
	VideoCapture cap;
	cap.open(0);								   //������ͷ����ȡ��Ƶ�ź�
	Uart_Init("com7",9600);						   //���ڳ�ʼ���������ʡ�ʹ�ܵȣ�
	ShowHelpText();
	namedWindow( "������������ϵͳ����λ��", 0 );
	setMouseCallback( "������������ϵͳ����λ��", onMouse, 0 );//���㣨�������򣩾��ο�
	while(1)
	{
		cap>>frame;								   //��ȡ��ǰ֡������cap��������Ƶ֡����frame(Mat��)
		if(frame.empty())
		{
			break;								   //break����ѭ���ṹ��break��������while��ѭ��,if�м���û�й�ϵ
		}	
		frame.copyTo(image);
		imwrite("����ͼ.png",frame,vector<int>(9));
		waitKey(1);
		IplImage* img = cvLoadImage("����ͼ.png", 1);
		color=0;
		Mat imge=Mat(img);

		ReadFile(hComm,&RXBuff,1,&BytesRead,&m_ov);//(����ʱ)�ϡ���λ��ͨ��ȷ��(���ڳɹ�����)
        if(RXBuff=='a')
			f=0;

		ReadFile(hComm,&RXBuff,1,&BytesRead,&m_ov);//(����)�ϡ���λ��ͨ��ȷ�ϣ����ɹ���
        if(RXBuff=='b')
			f=1;

		ReadFile(hComm,&RXBuff,1,&BytesRead,&m_ov);//(����)�ϡ���λ��ͨ��ȷ�ϣ����ɹ���
        if(RXBuff=='d')
			f0=1;

		ReadFile(hComm,&RXBuff,1,&BytesRead,&m_ov);//(����)�ϡ���λ��ͨ��ȷ�ϣ����ɹ���
        if(RXBuff=='e')
			f0=0;

		if(time>0)								   //����ʶ��RGB+HSI��ɫ��|���Ͷ�|���ȣ�
		{
			rectangle(image,juxin1,Scalar(255,0,0),3,8,0);//�����������򣩾��ο�
			Mat A=frame(Rect(juxin1.x,juxin1.y,juxin1.width,juxin1.height));
			a=&IplImage(A);
			for ( i = 0; i < a->height; i+=8)			  //���ο�ͼ���������ɨ�裩
			{
				for ( j = 1; j < a->width-1; j+=4)
 				{
					int B = ((uchar *)(a->imageData + i*a->widthStep))[j*a->nChannels + 0]; //����RGB��ԭɫ
					int G = ((uchar *)(a->imageData + i*a->widthStep))[j*a->nChannels + 1]; 
					int R = ((uchar *)(a->imageData + i*a->widthStep))[j*a->nChannels + 2]; 
					
					int maxValue = max(max(B,G),R);											//RGB��>HSI
					int minValue = min(min(B,G),R);  
					
					double S = (1-3.0*minValue/(R+G+B));									//���Ͷȷ�����ת����ʽ��
					
					if(R > RT && R >= G && G >= B && S > ST1 && S >((255 - R) * ST/RT))    //��3������̬��ɫ�оݣ�1��R>=G>=B;2��R>=Rt;3��S>=((255 - R) * ST/RT));��4��S>=A��
					{  
						frame.at<uchar>(i,j) = 255;  
						Pt1.x=j+juxin1.x;
						Pt1.y=i+juxin1.y;
						Pt2.x=j+juxin1.x+8;
						Pt2.y=i+juxin1.y+16;
						rectangle(image,Pt1,Pt2,Scalar(255,255,0),2,8,0);						//������������
						if(f==1)
							WriteChar("0",1);
					}
				}
			}
		}
		if(time>1)
		{
			rectangle(image,juxin2,Scalar(255,0,0),3,8,0);
			Mat B=frame(Rect(juxin2.x,juxin2.y,juxin2.width,juxin2.height));
			b=&IplImage(B);
			for ( i = 0; i < b->height; i+=8)
			{
				for ( j = 1; j < b->width-1; j+=4)
 				{
					int B = ((uchar *)(b->imageData + i*b->widthStep))[j*b->nChannels + 0]; // B
					int G = ((uchar *)(b->imageData + i*b->widthStep))[j*b->nChannels + 1]; // G
					int R = ((uchar *)(b->imageData + i*b->widthStep))[j*b->nChannels + 2]; // R
			
					int maxValue = max(max(B,G),R);  
					int minValue = min(min(B,G),R);  
  
					double S = (1-3.0*minValue/(R+G+B));  
  
					if(R > RT && R >= G && G >= B && S > ST1 && S >((255 - R) * ST/RT))  
					{  
						frame.at<uchar>(i,j) = 255;  
						Pt3.x=j+juxin2.x;
						Pt3.y=i+juxin2.y;
						Pt4.x=j+juxin2.x+8;
						Pt4.y=i+juxin2.y+16;
						rectangle(image,Pt3,Pt4,Scalar(255,255,0),2,8,0);
						if(f==1)
							WriteChar("1",1);
					}
				}
			}
		}
		if(time>2)
		{
			rectangle(image,juxin3,Scalar(255,0,0),3,8,0);
			Mat C=frame(Rect(juxin3.x,juxin3.y,juxin3.width,juxin3.height));
			c=&IplImage(C);
			for ( i = 0; i < c->height; i+=8)
			{
				for ( j = 1; j < c->width-1; j+=4)
 				{
					int B = ((uchar *)(c->imageData + i*c->widthStep))[j*c->nChannels + 0]; // B
					int G = ((uchar *)(c->imageData + i*c->widthStep))[j*c->nChannels + 1]; // G
					int R = ((uchar *)(c->imageData + i*c->widthStep))[j*c->nChannels + 2]; // R
			
					int maxValue = max(max(B,G),R);  
					int minValue = min(min(B,G),R);  
  
					double S = (1-3.0*minValue/(R+G+B));  
  
					if(R > RT && R >= G && G >= B && S > ST1 && S >((255 - R) * ST/RT)) 
					{  
						frame.at<uchar>(i,j) = 255;  
						Pt5.x=j+juxin3.x;
						Pt5.y=i+juxin3.y;
						Pt6.x=j+juxin3.x+8;
						Pt6.y=i+juxin3.y+16;
						rectangle(image,Pt5,Pt6,Scalar(255,255,0),2,8,0);
						if(f==1)
							WriteChar("2",1);
					}
				}
			}
		}
		if(time>3)								   //������λ���
		{
			rectangle(image,juxin4,Scalar(0,255,0),3,8,0);//�����������򣩾��ο�
			Mat D=frame(Rect(juxin4.x,juxin4.y,juxin4.width,juxin4.height));
			e=&IplImage(D);
			for ( i = 0; i < e->height; i+=8)			  //���ο�ͼ���������ɨ�裩
			{
				for ( j = 1; j < e->width-1; j+=4)
 				{
					int B = ((uchar *)(e->imageData + i*e->widthStep))[j*e->nChannels + 0]; //����RGB��ԭɫ
					int G = ((uchar *)(e->imageData + i*e->widthStep))[j*e->nChannels + 1]; 
					int R = ((uchar *)(e->imageData + i*e->widthStep))[j*e->nChannels + 2]; 

					int maxValue = max(max(B,G),R);  
					int minValue = min(min(B,G),R);  
  
					double S = (1-3.0*minValue/(R+G+B));
					
					if(B > BT && G < 50 && R < 50 && S > ST1)								//��⳵����ɫ��RGB+HSI(S)
					{  
						frame.at<uchar>(i,j) = 255;  
						Pt7.x=j+juxin4.x;
						Pt7.y=i+juxin4.y;
						Pt8.x=j+juxin4.x+8;
						Pt8.y=i+juxin4.y+16;
						rectangle(image,Pt7,Pt8,Scalar(0,255,255),2,8,0);
						if(f0==1)
							WriteChar("3",1);
					}
				}
			}
		}
		imshow( "������������ϵͳ����λ��", image );
	}
}

static void ShowHelpText()
{
	cout << "\n 2017�������\n(�а�)����11��\n\n������������ϵͳ������λ��";
}
bool openport(char *portname)		  //�򿪴���
{     
	WCHAR wszClassName[256];
    memset(wszClassName,0,sizeof(wszClassName));
    MultiByteToWideChar(CP_ACP,0,portname,strlen(portname)+1,wszClassName,sizeof(wszClassName)/sizeof(wszClassName[0]));
    hComm = CreateFile(wszClassName, //���ں�
    GENERIC_READ | GENERIC_WRITE,    //�����д
    0,								 //ͨѶ�豸�����Զ�ռ��ʽ��
    0,								 //�ް�ȫ����
    OPEN_EXISTING,				     //ͨѶ�豸�Ѵ���
    FILE_FLAG_OVERLAPPED,			 //�첽 I/O
    NULL);							 //ͨѶ�豸������ģ���
    if(hComm == INVALID_HANDLE_VALUE)
    {
		CloseHandle(hComm);
        return FALSE;
    }
    else
        return true;
}
void closeport()
{
    CloseHandle(hComm);
}
bool setupdcb(int rate_arg)		 //���� DCB
{
	DCB dcb;
    int rate= rate_arg;
    memset(&dcb,0,sizeof(dcb));
    if(!GetCommState(hComm,&dcb))//��ȡ��ǰ DCB ����
		return FALSE;			 // set DCB to configure the serial port
    dcb.DCBlength = sizeof(dcb); /* ---------- Serial Port Config ------- */
    dcb.BaudRate = rate;
    dcb.Parity = NOPARITY;
    dcb.fParity = 0;
    dcb.StopBits = ONESTOPBIT;
    dcb.ByteSize = 8;
    dcb.fOutxCtsFlow = 0;
    dcb.fOutxDsrFlow = 0;
    dcb.fDtrControl = DTR_CONTROL_DISABLE;
    dcb.fDsrSensitivity = 0;
    dcb.fRtsControl = RTS_CONTROL_DISABLE;
    dcb.fOutX = 0;
    dcb.fInX = 0;
    /* ----------------- misc parameters ----- */
    dcb.fErrorChar = 0;
    dcb.fBinary = 1;
    dcb.fNull = 0;
    dcb.fAbortOnError = 0;
    dcb.wReserved = 0;
    dcb.XonLim = 2;
    dcb.XoffLim = 4;
    dcb.XonChar = 0x13;
    dcb.XoffChar = 0x19;
    dcb.EvtChar = 0; // set DCB
    if(!SetCommState(hComm,&dcb))
		return false;
    else
        return true;
}                     
bool setuptimeout(DWORD ReadInterval, DWORD ReadTotalMultiplier,DWORD ReadTotalconstant, DWORD WriteTotalMultiplier, DWORD WriteTotalconstant)
{
	COMMTIMEOUTS timeouts;
    timeouts.ReadIntervalTimeout=ReadInterval;
    timeouts.ReadTotalTimeoutConstant=ReadTotalconstant;
    timeouts.ReadTotalTimeoutMultiplier=ReadTotalMultiplier;
    timeouts.WriteTotalTimeoutConstant=WriteTotalconstant;
    timeouts.WriteTotalTimeoutMultiplier=WriteTotalMultiplier;
    if(!SetCommTimeouts(hComm, &timeouts))
		return false;
    else
        return true;
}
void ReceiveChar( )
{
	BOOL bRead = TRUE;
    BOOL bResult = TRUE;
    while(1) 
	{
		bResult = ClearCommError(hComm, &dwError, &comstat);
        if (comstat.cbInQue == 0)
			continue;
        if (bRead) 
		{
			bResult = ReadFile(hComm,&RXBuff,1,&BytesRead,&m_ov); 
            printf("%c",RXBuff);
            if (!bResult) 
			{
				switch (dwError = GetLastError()) 
				{
					case ERROR_IO_PENDING:bRead = FALSE;break;
                    default: break;
                }
            }
            else 
			{
				bRead = TRUE;
            } // close if (bRead)
            if (!bRead) 
			{
				bRead = TRUE;
                bResult = GetOverlappedResult(hComm,&m_ov,&BytesRead,TRUE);  
            }
        }
    }
}
int WriteChar(char* m_szWriteBuffer,DWORD m_nToSend)
{
	BOOL bWrite = TRUE;
    BOOL bResult = TRUE;
    DWORD BytesSent = 0;
    //  ResetEvent(m_hWriteEvent);
    if (bWrite) 
	{
		m_ov.Offset = 0;
        m_ov.OffsetHigh = 0;
        // Clear buffe r
        bResult = WriteFile(hComm,m_szWriteBuffer,m_nToSend,&BytesSent,&m_ov );     
        if (!bResult) 
		{
			DWORD dwError = GetLastError();
            switch (dwError) 
			{
				case ERROR_IO_PENDING:BytesSent = 0;bWrite = FALSE;break;
                default:break;
            }
         }
    } // end if(bWrite)
    if (!bWrite) 
	{
		bWrite = TRUE;
        bResult = GetOverlappedResult(hComm,&m_ov,&BytesSent,TRUE); 
        if (!bResult) 
		{
			printf("GetOverlappedResults() in WriteFile()");
        }                                                                 
    } 
    if (BytesSent != m_nToSend) 
	{
		printf("WARNING: WriteFile() error.. Bytes Sent: %d; Message Length:%d\n", BytesSent, strlen((char*)m_szWriteBuffer));
    }
    return true;
}
void Uart_Init(char *portname,int dcb)
{
    if(openport(portname));
//		printf("open comport success\n");
    if(setupdcb(dcb));
//       printf("setupDCB success\n");
    if(setuptimeout(0,0,0,0,0));
//        printf("setuptimeout success\n");
    PurgeComm(hComm,PURGE_RXCLEAR|PURGE_TXCLEAR| PURGE_RXABORT|PURGE_TXABORT);
}
//���㣨�������򣩾��ο�
static void onMouse( int event, int x, int y, int flags, void* )
{
	if(juxin_flag)  
    {  
		if(time==0)
	    {
			juxin1.x=MIN(origin.x,x);				//��һ��Ҫ����굯��ż�����ο򣬶�Ӧ������갴�¿�ʼ���������ʱ��ʵʱ������ѡ���ο�  
			juxin1.y=MIN(origin.y,y);  
			juxin1.width=abs(x-origin.x);			//����ο�Ⱥ͸߶�  
			juxin1.height=abs(y-origin.y);  
			juxin1&=Rect(0,0,frame.cols,frame.rows);//��֤��ѡ���ο�����Ƶ��ʾ����֮��
	    }
	    if(time==1)
	    {
			juxin2.x=MIN(origin.x,x);				//��һ��Ҫ����굯��ż�����ο򣬶�Ӧ������갴�¿�ʼ���������ʱ��ʵʱ������ѡ���ο�  
			juxin2.y=MIN(origin.y,y);  
			juxin2.width=abs(x-origin.x);			//����ο�Ⱥ͸߶�  
			juxin2.height=abs(y-origin.y);  
			juxin2&=Rect(0,0,frame.cols,frame.rows);//��֤��ѡ���ο�����Ƶ��ʾ����֮��
	    }
	    if(time==2)
	    {
			juxin3.x=MIN(origin.x,x);				//��һ��Ҫ����굯��ż�����ο򣬶�Ӧ������갴�¿�ʼ���������ʱ��ʵʱ������ѡ���ο�  
			juxin3.y=MIN(origin.y,y);  
			juxin3.width=abs(x-origin.x);			//����ο�Ⱥ͸߶�  
			juxin3.height=abs(y-origin.y);  
			juxin3&=Rect(0,0,frame.cols,frame.rows);//��֤��ѡ���ο�����Ƶ��ʾ����֮��
	    }
		if(time==3)
	    {
			juxin4.x=MIN(origin.x,x);				//��һ��Ҫ����굯��ż�����ο򣬶�Ӧ������갴�¿�ʼ���������ʱ��ʵʱ������ѡ���ο�  
			juxin4.y=MIN(origin.y,y);  
			juxin4.width=abs(x-origin.x);			//����ο�Ⱥ͸߶�  
			juxin4.height=abs(y-origin.y);  
			juxin4&=Rect(0,0,frame.cols,frame.rows);//��֤��ѡ���ο�����Ƶ��ʾ����֮��
	    }
		if(time>3)
			time=0;
    }  
    if(event==CV_EVENT_LBUTTONDOWN)  
    {  
        juxin_flag=true;//��갴�µı�־����ֵ  
        origin=Point(x,y);//�������������ǲ�׽���ĵ� 
		if(time==0)
			juxin1=Rect(x,y,0,0);//����һ��Ҫ��ʼ������͸�Ϊ(0,0)����Ϊ��opencv��Rect���ο����ڵĵ��ǰ������Ͻ��Ǹ���ģ����ǲ������½��Ǹ���  
		if(time==1)
			juxin2=Rect(x,y,0,0);
		if(time==2)
			juxin3=Rect(x,y,0,0);
		if(time==3)
			juxin4=Rect(x,y,0,0);
    }  
    else if(event==CV_EVENT_LBUTTONUP)  
    {  
		juxin_flag=false;  
		Pt=Point(x,y);
		time++;
    }  
}
