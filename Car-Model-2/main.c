#include<reg52.h>
#include<string.h>

unsigned int num[7];
unsigned int display_ctr=0;     // 数码管显示循环控制
unsigned int sending=0,new_signal=0;        //sending用于缓解串口数据发送较慢的情况,new_signal用于判断是否有新数据进入
unsigned int sign=0;
unsigned char Action_Buff='q';

#include"digital_led.h"     //调用数码管部分
extern struct Queue RBuff;     //digital_led.h
extern void InitQueue(struct Queue *Q);     //循环队列初始化
extern unsigned char SerQueue(struct Queue *Q,int r);       // r为所要查找的当前位置
extern void char_to_int();     //字符转整型数字并输出


struct Queue RBuff;     //Rbuff是一个静态循环队列/字符数组

//----------初始化-------------
void Init()
{
	//串口设置
	SCON = 0x50;	//工作方式1
	PCON = 0;	//波特率不加倍
	TMOD = 0x20;
	TH1 = 0xfd;	//11.0592晶振，9600波特率
	TL1 = TH1;
	TR1 = 1;
	REN = 1;        //REN地址的值为控制串口数据接收的允许或者禁止
	ES = 1;	//允许串口中断
	TI = 0;     //这两条在多个中断中一定要写，不然不能进入串口中断
	EA = 1;
}

void main(void)
{
	Init();
	InitQueue(&RBuff);
	while(1)  //不让主函数循环执行并且循环检测（查询方式）
	{
		if(SerQueue(&RBuff,RBuff.rear) == '@' && new_signal)
		{
			char_to_int();
		}
	}       
}