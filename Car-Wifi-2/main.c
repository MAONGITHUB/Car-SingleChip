#include <stc89c52.h>
#include <string.h>

sbit key1=P3^4;

unsigned int xdata num[7];
unsigned int display_ctr=0;     // 数码管显示循环控制
unsigned int sending=0,new_signal=0;        //sending用于缓解串口数据发送较慢的情况,new_signal用于判断是否有新数据进入
unsigned int sign=0;
unsigned char Action_Buff='q';

#include"digital_led.h"     //调用数码管部分
extern struct Queue RBuff;     //digital_led.h
extern void InitQueue(struct Queue *Q);     //循环队列初始化
extern unsigned char SerQueue(struct Queue *Q,int r);       // r为所要查找的当前位置
extern void char_to_int();     //字符转整型数字并输出

#include"ESP8266.h"
extern void Init_ESP8266();


struct Queue RBuff;     //Rbuff是一个静态循环队列/字符数组

//------------Chip初始化-----------------
void Init()
{
	//AUXR &= 0xFB;		//定时器2时钟为Fosc/12,即12T,T2在预分频禁止时默认为12分频
	//AUXR |= 0x01;		//串口1选择定时器2为波特率发生器,已经在T2CON设置R/TCLK

	RCAP2H = 0xff;	//T2高8位重载值
	RCAP2L = 0xdc;	//T2低8位重载值
	TH2 = RCAP2H;	//T2初值
	TL2 = RCAP2L;	//T2初值     
		
	PCON = 0;       //波特率不加倍	
	T2CON = 0x34;	//T2控制设置,在R/TCLK确定T2作为串口波特率产生器 TF2 EXF2 (RCLK) (TCLK) EXEN2 (TR2) C/T2 CP/RL2
	T2MOD = 0x01;	//T2模式设置, D7 D6 D5 D4 D3 D2 T2OE(输出到P1.0) DCEN(向下计数允许)
	SCON = 0x50;	    //串口工作方式1,8位数据,可变波特率
	REN = 1;	//启用串口接收

	/*--------------------Timer1--------------------------------*/
	TMOD = 0x10;	//定时器1,工作模式1(16位)
	TH1=(65536-46080)/256;		//所记次数应为46080,计时器每隔50000us即50ms发起一次中断
	TL1=(65536-46080)%256;		//46080=50000*11.0592/12
	TR1 = 1;	//打开定时器1

	/*-------------------interrupt_control----------------------*/
	ES = 0;//先关闭串口中断,WIFI模块初始化后再打开
	TI = 0;//串口输出中断需先置0避免引起冲突
	EA = 1;//开启总中断
}

void main(void)
{
	Init();
	Delayms(2000);
	//Init_ESP8266();
	TI = 1;
	ES=1;
	InitQueue(&RBuff);
	while(1)  //不让主函数循环执行并且循环检测（查询方式）
	{
		if(SBUF== '@' && new_signal)	//if(SerQueue(&RBuff,RBuff.rear) == '@' && new_signal)
		{
			char_to_int();
		}
		if(key1 == 0)
		{
			P1=0x00;
			Init_ESP8266();
			P1=0xff;
		}
	}       
}