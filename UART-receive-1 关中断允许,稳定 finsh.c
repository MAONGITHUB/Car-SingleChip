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

///------------串口接收--------------------------------
void Receive() 
{
	P1 = 0xff;
	display_ctr=0;
	if(p>12)		//让RBuff成为循环数组，从头覆盖，避免溢出
	{
		p=0;
	}
	else
	{
		RBuff[p] = SBUF;		//SBUF串口寄存器,两个,99H,物理上相互独立
		p++;		
	}
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
	if(RI)    //收到数据，误认接收使能回置
	{
		ES=0;	//关闭串口中断
		Receive();
		RI=0;   //清中断请求
		ES=1;	//打开串口中断
	}
}
	

void main(void)
{
	Init();
	while(1){}		//不让主函数循环执行
}