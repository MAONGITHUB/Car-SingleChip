#include <reg51.h>
#include <string.h>		//strlen()要用到
#include <intrins.h>
#include <stdio.h>
//#include <math.h> 		//几次方 pow(数值,几次方)
//#include <stdlib.h>		//atoi()要用到，

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
unsigned char wei[6]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf};		//数码管位置选择

sbit duanx=P2^6;    //段选信号的锁存器控制
sbit weix=P2^7;   //位选信号的锁存器控制
//unsigned char RBuff[8];		//={'1','2','3','4','q','9','w','6','5'};     //串口缓存
char SBuff[4];
unsigned int num[6];
int display_ctr=0;
int sending=0;		//sending用于缓解串口数据发送较慢的情况
int p=0;

//------------静态循环队列（数组）（无头指针）定义-----------------
int maxsize=8;
typedef struct Queue	//存储结构
{
	char elem[maxsize];
	int rear=-1;
};

void EnQueue(struct Queue *Q,char e)
{
	Q.rear = (Q.rear+1) % maxsize;
	Q.elem[Q.rear] = e;
}

char SerQueue(struct Queue *Q,int r)		// r为所要查找的当前位置
{		//查看队列元素时若位置为负数，取余有编译器及语言差异，保守起见自个写
	if (r<0)
		return Q[maxsize+1+r];
	else
		return Q[r];
}

struct Queue RBuff;

//------------延时函数-----------------
void Delayms( unsigned int z )
{
	unsigned int x;
	unsigned int y;
	for( x = z; x > 0; x-- )
	{
		for( y = 110; y > 0; y-- );
	}
}

//----------初始化-------------
void Init()
{
					//串口设置
	SCON = 0x50;  		 //工作方式1
	PCON = 0;	  	 //波特率不加倍
	TMOD = 0x20;
	TH1 = 0xfd;	   	//11.0592晶振，9600波特率
	TL1 = TH1;
	TR1 = 1;
	REN = 1;
	ES = 1;			//允许串口中断
	TI = 0;			//这两条在多个中断中一定要写，不然不能进入串口中断
	EA = 1;
}

//------------P0口重置--------------------------------
void ResetP0()
{
  	P0=0xff;
}

//------------数码管动态显示--------------------------------
void Display(int x )
{								//P0.0~0.7控制573锁存器
	int y,k;
  	while(display_ctr && !RI)
 	{
 		for(k=0;k<x;k++)
  		{
  			duanx=0;
  			weix=0;
  			y=num[k];
  			P0=number[y];    	 //选择显示数字,共阴
  			duanx=1;
  			duanx=0;

	  		ResetP0();		
  			P0=wei[k];			//数码管位置选择,P0.0~0.5，由左至右，操作共阳，实际共阴
  			weix=1;
 	 		weix=0;

 	 		Delayms(2);
 	 	}		
  	}
}

//------------字符转整型数字--------------------------------
void char_to_int()
{
	int i=0,n=0,j=0;
	n=strlen(RBuff);		//Rbuff是一个字符数组
	while(i<6 && j<n )		//取输入的前6而为数字的数输出
	{						//atoi是将字符【串】转换为数字啊
		if (RBuff[j]>='0' && RBuff[j]<='9')
		{
			num[i]=RBuff[j]-'0';		//num[i]=atoi(c);	
			i++;
		}
		j++;
	}
	memset(RBuff,0,12);
	display_ctr=1;
	Delayms(1000);
	Display(i);		//strlen()不能计算满额数组的长度，必须小于
}

//------------串口发送--发送函数-----------------
void Send(char type,char cmd)
{ 
	char SBuff[4];	  //构建数据包，放前面去了
	char *p;
	int Send_Count=0;
	*p = SBuff;
	SBuff[0]=0XFF;		//前置校验码
	SBuff[1]=type;
	SBuff[2]=cmd;
	SBuff[3]=0XFF;		//后置校验码
	 
	
		SBUF = SBuff[1];  //发送
		sending=1;	//设置正在发送标志
		while(1)
		{
			if (sending==0) break;
		}
		//_nop_();       //微妙级延时函数(指令),12MHz延时1us
		SBUF = SBuff[2];
		sending=1;
		while(1)
		{
			if (sending==0) break;
		}
}

//------------串口接收--------------------------------
void Receive() 
{
	P1 = 0xff;
	display_ctr=0;
	EnQueue(RBuff,SBUF);		//SBUF串口寄存器,两个地址一样99H,但物理上相互独立不会覆盖
}

//------------中断回置及缓和串口阻塞-----------
void Send_interrupt(void) interrupt 4 using 1
//中断0 外部中断0；中断1 定时器1中断；中断2 外部中断1；中断3 定时器2中断；中断4 串口中断
//using 0 是第0组寄存器。using 1 是第1组寄存器。using 2 是第2组寄存器。using 3 是第3组寄存器。			 
{
	if(TI)      //TI在逻辑时序上先于RI触发
	{
		sending=0;	//清除正在发送标志
		TI=0;			//发送中断回置
	}
	if(RI)    //收到数据
	{
		ES=0;	//关闭串口中断
		Receive();
		RI=0;   //清中断请求
		ES=1;	//打开串口中断
		if(SerQueue(RBuff,RBuff.rear) == '@')
		{
			Send(Serqueue(RBuff,(RBuff.rear-1)),Serqueue(RBuff,(RBuff.rear-2)));
		}
	}
}
	

void main(void)
{
	Init();
	while(1){}		//不让主函数循环执行
}