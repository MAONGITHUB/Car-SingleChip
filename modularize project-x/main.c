#include<reg52.h>
#include<string.h>

unsigned int num[7];
unsigned int display_ctr=0;     // 数码管显示循环控制
unsigned int sending=0,new_signal=0;        //sending用于缓解串口数据发送较慢的情况,new_signal用于判断是否有新数据进入

#include"digital_led.h"     //调用数码管部分
extern struct Queue RBuff;     //digital_led.h
extern void InitQueue(struct Queue *Q);     //循环队列初始化
extern unsigned char SerQueue(struct Queue *Q,int r);       // r为所要查找的当前位置
extern void char_to_int();     //字符转整型数字并输出

//------------静态循环队列（数组）（无头指针）定义-----------------
#define maxsize 20  //有效字符数字若大于等于maxsize会出现某个位的数码管无法显示，即队列溢出，静态数组的弊端啊
typedef struct Queue        //存储结构
{
	char elem[maxsize];
	int rear;
};

void InitQueue(struct Queue *Q)     //初始化,这里由于是直接写数据类型Queue,还没写具体的变量,只能用指针*Q没办法取地址符&Q
{
	memset(RBuff.elem,'z',8);       //填充RBuff空字符,即整型数字0,以便统计新输入的字符
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

struct Queue RBuff;     //Rbuff是一个字符数组

//----------初始化-------------
void Init()
{
	//串口设置
	SCON = 0x50;   //工作方式1
	PCON = 0;      //波特率不加倍
	TMOD = 0x20;
	TH1 = 0xfd;    //11.0592晶振，9600波特率
	TL1 = TH1;
	TR1 = 1;
	REN = 1;
	ES = 1;     //允许串口中断
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