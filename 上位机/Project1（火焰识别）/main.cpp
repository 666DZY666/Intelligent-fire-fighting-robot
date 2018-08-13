//2017工程设计——智能灭火机器人系统—上位机（综合）：火焰识别+机器归位

//火焰识别：RGB—>RGB+HSI（—>预处理-平滑滤波|中值滤波—>形态学处理-膨胀|腐蚀（图像去噪、分割—解决红黄之间无法识别，实现区域连通））
//机器归位：上位机+下位机（MPU6050）
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core/core.hpp"  
#include "highgui/highgui.hpp"  
#include "imgproc/imgproc.hpp"  
#include<iostream>
#include "opencv2/objdetect/objdetect.hpp"  //调用OpenCV2.4.9函数库
//申请工作空间
using namespace cv;  
using namespace std;
//检测阈值宏定义
  //火焰检测
#define RT  80     //90~135  
#define ST  45     //45~65  
  //机器检测
#define BT  75	   //>80
#define ST1 0.01
//变量定义
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
Mat frame;         //定义一个Mat类，用于存储每一帧图像
HANDLE hComm;
OVERLAPPED m_ov;
COMSTAT comstat;
DWORD m_dwCommEvents;
//函数声明
int WriteChar(char* m_szWriteBuffer,DWORD m_nToSend);
bool openport(char *portname);
bool setupdcb(int rate_arg);
bool setuptimeout(DWORD ReadInterval, DWORD ReadTotalMultiplier,DWORD ReadTotalconstant, DWORD WriteTotalMultiplier,DWORD WriteTotalconstant);
void closeport();
void ReceiveChar();
void Uart_Init(char *portname,int dcb);
static void onMouse( int event, int x, int y, int flags, void* );
static void ShowHelpText();
//主函数
void main()
{	
	char f=1;
	char f0=0;
	VideoCapture cap;
	cap.open(0);								   //打开摄像头，获取视频信号
	Uart_Init("com7",9600);						   //串口初始化（波特率、使能等）
	ShowHelpText();
	namedWindow( "智能灭火机器人系统—上位机", 0 );
	setMouseCallback( "智能灭火机器人系统—上位机", onMouse, 0 );//计算（限制区域）矩形框
	while(1)
	{
		cap>>frame;								   //读取当前帧，即将cap读到的视频帧放入frame(Mat类)
		if(frame.empty())
		{
			break;								   //break用于循环结构，break跳出的是while内循环,if有几层没有关系
		}	
		frame.copyTo(image);
		imwrite("截屏图.png",frame,vector<int>(9));
		waitKey(1);
		IplImage* img = cvLoadImage("截屏图.png", 1);
		color=0;
		Mat imge=Mat(img);

		ReadFile(hComm,&RXBuff,1,&BytesRead,&m_ov);//(启动时)上、下位机通信确认(串口成功接收)
        if(RXBuff=='a')
			f=0;

		ReadFile(hComm,&RXBuff,1,&BytesRead,&m_ov);//(灭火后)上、下位机通信确认（灭火成功）
        if(RXBuff=='b')
			f=1;

		ReadFile(hComm,&RXBuff,1,&BytesRead,&m_ov);//(灭火后)上、下位机通信确认（灭火成功）
        if(RXBuff=='d')
			f0=1;

		ReadFile(hComm,&RXBuff,1,&BytesRead,&m_ov);//(灭火后)上、下位机通信确认（灭火成功）
        if(RXBuff=='e')
			f0=0;

		if(time>0)								   //火焰识别—RGB+HSI（色度|饱和度|亮度）
		{
			rectangle(image,juxin1,Scalar(255,0,0),3,8,0);//画（限制区域）矩形框
			Mat A=frame(Rect(juxin1.x,juxin1.y,juxin1.width,juxin1.height));
			a=&IplImage(A);
			for ( i = 0; i < a->height; i+=8)			  //矩形框图像遍历（行扫描）
			{
				for ( j = 1; j < a->width-1; j+=4)
 				{
					int B = ((uchar *)(a->imageData + i*a->widthStep))[j*a->nChannels + 0]; //分离RGB三原色
					int G = ((uchar *)(a->imageData + i*a->widthStep))[j*a->nChannels + 1]; 
					int R = ((uchar *)(a->imageData + i*a->widthStep))[j*a->nChannels + 2]; 
					
					int maxValue = max(max(B,G),R);											//RGB—>HSI
					int minValue = min(min(B,G),R);  
					
					double S = (1-3.0*minValue/(R+G+B));									//饱和度分量（转换公式）
					
					if(R > RT && R >= G && G >= B && S > ST1 && S >((255 - R) * ST/RT))    //（3个）静态颜色判据：1、R>=G>=B;2、R>=Rt;3、S>=((255 - R) * ST/RT));（4、S>=A）
					{  
						frame.at<uchar>(i,j) = 255;  
						Pt1.x=j+juxin1.x;
						Pt1.y=i+juxin1.y;
						Pt2.x=j+juxin1.x+8;
						Pt2.y=i+juxin1.y+16;
						rectangle(image,Pt1,Pt2,Scalar(255,255,0),2,8,0);						//画火焰锁定框
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
		if(time>3)								   //机器归位检测
		{
			rectangle(image,juxin4,Scalar(0,255,0),3,8,0);//画（限制区域）矩形框
			Mat D=frame(Rect(juxin4.x,juxin4.y,juxin4.width,juxin4.height));
			e=&IplImage(D);
			for ( i = 0; i < e->height; i+=8)			  //矩形框图像遍历（行扫描）
			{
				for ( j = 1; j < e->width-1; j+=4)
 				{
					int B = ((uchar *)(e->imageData + i*e->widthStep))[j*e->nChannels + 0]; //分离RGB三原色
					int G = ((uchar *)(e->imageData + i*e->widthStep))[j*e->nChannels + 1]; 
					int R = ((uchar *)(e->imageData + i*e->widthStep))[j*e->nChannels + 2]; 

					int maxValue = max(max(B,G),R);  
					int minValue = min(min(B,G),R);  
  
					double S = (1-3.0*minValue/(R+G+B));
					
					if(B > BT && G < 50 && R < 50 && S > ST1)								//检测车体蓝色：RGB+HSI(S)
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
		imshow( "智能灭火机器人系统—上位机", image );
	}
}

static void ShowHelpText()
{
	cout << "\n 2017工程设计\n(中澳)电子11组\n\n智能灭火机器人系统——上位机";
}
bool openport(char *portname)		  //打开串口
{     
	WCHAR wszClassName[256];
    memset(wszClassName,0,sizeof(wszClassName));
    MultiByteToWideChar(CP_ACP,0,portname,strlen(portname)+1,wszClassName,sizeof(wszClassName)/sizeof(wszClassName[0]));
    hComm = CreateFile(wszClassName, //串口号
    GENERIC_READ | GENERIC_WRITE,    //允许读写
    0,								 //通讯设备必须以独占方式打开
    0,								 //无安全属性
    OPEN_EXISTING,				     //通讯设备已存在
    FILE_FLAG_OVERLAPPED,			 //异步 I/O
    NULL);							 //通讯设备不能用模板打开
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
bool setupdcb(int rate_arg)		 //设置 DCB
{
	DCB dcb;
    int rate= rate_arg;
    memset(&dcb,0,sizeof(dcb));
    if(!GetCommState(hComm,&dcb))//获取当前 DCB 配置
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
//计算（限制区域）矩形框
static void onMouse( int event, int x, int y, int flags, void* )
{
	if(juxin_flag)  
    {  
		if(time==0)
	    {
			juxin1.x=MIN(origin.x,x);				//不一定要等鼠标弹起才计算矩形框，而应该在鼠标按下开始到弹起这段时间实时计算所选矩形框  
			juxin1.y=MIN(origin.y,y);  
			juxin1.width=abs(x-origin.x);			//算矩形宽度和高度  
			juxin1.height=abs(y-origin.y);  
			juxin1&=Rect(0,0,frame.cols,frame.rows);//保证所选矩形框在视频显示区域之内
	    }
	    if(time==1)
	    {
			juxin2.x=MIN(origin.x,x);				//不一定要等鼠标弹起才计算矩形框，而应该在鼠标按下开始到弹起这段时间实时计算所选矩形框  
			juxin2.y=MIN(origin.y,y);  
			juxin2.width=abs(x-origin.x);			//算矩形宽度和高度  
			juxin2.height=abs(y-origin.y);  
			juxin2&=Rect(0,0,frame.cols,frame.rows);//保证所选矩形框在视频显示区域之内
	    }
	    if(time==2)
	    {
			juxin3.x=MIN(origin.x,x);				//不一定要等鼠标弹起才计算矩形框，而应该在鼠标按下开始到弹起这段时间实时计算所选矩形框  
			juxin3.y=MIN(origin.y,y);  
			juxin3.width=abs(x-origin.x);			//算矩形宽度和高度  
			juxin3.height=abs(y-origin.y);  
			juxin3&=Rect(0,0,frame.cols,frame.rows);//保证所选矩形框在视频显示区域之内
	    }
		if(time==3)
	    {
			juxin4.x=MIN(origin.x,x);				//不一定要等鼠标弹起才计算矩形框，而应该在鼠标按下开始到弹起这段时间实时计算所选矩形框  
			juxin4.y=MIN(origin.y,y);  
			juxin4.width=abs(x-origin.x);			//算矩形宽度和高度  
			juxin4.height=abs(y-origin.y);  
			juxin4&=Rect(0,0,frame.cols,frame.rows);//保证所选矩形框在视频显示区域之内
	    }
		if(time>3)
			time=0;
    }  
    if(event==CV_EVENT_LBUTTONDOWN)  
    {  
        juxin_flag=true;//鼠标按下的标志赋真值  
        origin=Point(x,y);//保存下来单击是捕捉到的点 
		if(time==0)
			juxin1=Rect(x,y,0,0);//这里一定要初始化，宽和高为(0,0)是因为在opencv中Rect矩形框类内的点是包含左上角那个点的，但是不含右下角那个点  
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
