#include <reg51.h>
#include <string.h>     //strlen()要用到
#include <intrins.h>

int sending=0;		//sending用于缓解串口数据发送较慢的情况	

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

//------------Chip初始化-----------------
void Init()		//串口设置
{			  
	SCON = 0x50;         //工作方式1
	PCON = 0;        //波特率不加倍
	TMOD = 0x20;
	TH1 = 0xfd;     //11.0592晶振，9600波特率
	TL1 = TH1;
	TR1 = 1;
	REN = 1;
	ES = 1;         //允许串口中断
	TI = 0;         //这两条在多个中断中一定要写，不然不能进入串口中断
	EA = 1;
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
		while(sending);
		//_nop_();       //微妙级延时函数(指令),12MHz延时1us
		SBUF = SBuff[2];
		sending=1;
		while(sending);
}

//------------串口发送--中断回置及缓和串口阻塞-----------
void Send_interrupt(void) interrupt 4			 
{
	if(RI)    //收到数据，误认接收使能回置
	{
		RI=0;   //清中断请求
	}
	else      //发送完一字节数据
	{
		TI=0;			//发送中断回置
		sending=0;	//清除正在发送标志
	}
}

void main()
{
	Init();
	Send('8','5');
	while(1){}		//不让主函数循环执行
}