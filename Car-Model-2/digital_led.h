#ifndef _digital_led_h_
#define _digital_led_h_


#include <reg52.h>
#include <string.h>     //strlen()要用到
#include <intrins.h>
#include <stdio.h>

#include"delay_uart.h"        //调用延迟_串口模块


 /*
 atof()     将字符串转换为双精度浮点型值
 atoi()     将字符串转换为整型值
 atol()     将字符串转换为长整型值
 strtod()   将字符串转换为双精度浮点型值，并报告不能被转换的所有剩余数字
 strtol()   将字符串转换为长整值，并报告不能被转换的所有剩余数字
 strtoul()  将字符串转换为无符号长整型值，并报告不能被转换的所有剩余数字
 */

//sbit led0=P0^0;sbit led1=P0^1;sbit led2=P0^2;sbit led3=P0^3;sbit led4=P0^4;sbit led5=P0^5;

unsigned char number[16]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71}; //数字0~F,小数点为P0.7(单独显示0x80)
unsigned char wei[6]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf};       //数码管位置选择

sbit duanx=P2^6;    //段选信号的锁存器控制
sbit weix=P2^7;   //位选信号的锁存器控制


//------------静态循环队列（数组）（无头指针）定义-----------------
#define maxsize 10  //有效字符数字若大于等于maxsize会出现某个位的数码管无法显示，即队列溢出，静态数组的弊端啊
typedef struct Queue        //存储结构
{
	char elem[maxsize];
	int rear;
};

void InitQueue(struct Queue *Q)     //初始化,这里由于是直接写数据类型Queue,还没写具体的变量,只能用指针*Q没办法取地址符&Q
{
	memset(Q->elem,'z',8);       //填充RBuff空字符,即整型数字0,以便统计新输入的字符
							//填充重置后，需要把指针回置到初始位
	Q->rear=-1;
}

void EnQueue(struct Queue *Q,char e)    //进队列
{
	Q->rear = (Q->rear+1) % maxsize;
	Q->elem[Q->rear] = e;
}

unsigned char SerQueue(struct Queue *Q,int r)       // r为所要查找的当前位置
{       //查看队列元素时若位置为负数，取余有编译器及语言差异，保守起见自个写
	if (r<0)
		return Q->elem[maxsize+1+r];
	else
		return Q->elem[r%maxsize];
}

//------------P0口重置--------------------------------
void ResetP0()
{
	P0=0xff;
}

//------------数码管动态显示--------------------------------
void Display(int x)    //x用于控制数码管显示数量(问题出现这里啊,每次Action后等很久才继续x,x的传参有问题)
{                               //P0.0~0.7控制573锁存器
	int y,k;
	while(display_ctr && !RI)
	{
		for(k=0;k<x;k++)
		{
			duanx=0;
			weix=0;
			y=num[k];
			P0=number[y];        //选择显示数字,共阴
			duanx=1;
			duanx=0;

			ResetP0();
			P0=wei[k];          //数码管位置选择,P0.0~0.5，由左至右，操作共阳，实际共阴
			weix=1;
			weix=0;
			Delayms(2);
		}     
	}
	P0=0x00;		//数码管关闭
	duanx=1,
	duanx=0;
}

//------------串口发送信息选择-----------------
void Sort_send(int len)
{
	int n=0;
	while(n<len)
	{
		Send(num[n]);
		n++;
	}
	display_ctr=1;      //向串口发送完后开启数码管显示
	Delayms(100);
	Display(len);        //strlen()不能计算满额数组的长度，必须小于
}

//------------字符转整型数字并输出--------------------------------
void char_to_int()
{
	int i=0,n=0,k=0;
	int j=RBuff.rear-1;
	n=strlen(RBuff.elem);       //遇到空字符即数字0便停止统计
	while(i<6 && k<n)       //取输入的前6而为数字的数输出(数码管只有6个),若已输入少于
	{                       //atoi是将字符【串】转换为数字啊
		if (SerQueue(&RBuff,j-k)>='0' && SerQueue(&RBuff,j-k)<='9')
		{
			num[i]=SerQueue(&RBuff,j-i)-'0';        //num[i]=atoi(c);   
			i++;        //用于num数组
		}
		k++;        //倒着循环RBuff队列
	}
	InitQueue(&RBuff);      //填充RBuff空字符,即整型数字0,以便统计新输入的字符
	Sort_send(i);
}



#endif
