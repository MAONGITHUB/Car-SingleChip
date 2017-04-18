#include<reg52.h>

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
	SCON = 0x50;   //工作方式1
	PCON = 0;	   //波特率不加倍
	TMOD = 0x20;
	TH1 = 0xfd;	   //11.0592晶振，9600波特率
	TL1 = TH1;
	TR1 = 1;
	REN = 1;
	ES = 1;		//允许串口中断
	TI = 0;		//这两条在多个中断中一定要写，不然不能进入串口中断
	EA = 1;
}

//--------------------动作函数-----------------
void Action()		
{
	P1 = 0xf0;
	Delayms(1500);
	P1 = 0x0f;
	Delayms(1000);

}

//---------------------标识码判定----------------
unsigned char Buff[3];     //串口缓存
unsigned char Order[3] = {'1','2','3'};

void Verify()		
{
	if(Buff[0] == Order[0])
	{
		 if(Buff[1] == Order[1])
		 {
			 if(Buff[2] == Order[2])
			 {
				Action();
			 }
		 }
	}
}

//------------串口接收--------------------------------
unsigned int WaitTime = 0; //串口等待接收的时间计数
void Receive() interrupt 4 using 1 		
//中断0 外部中断0；中断1 定时器1中断；中断2 外部中断1；中断3 定时器2中断；中断4 串口中断
//using 0 是第0组寄存器。using 1 是第1组寄存器。using 2 是第2组寄存器。using 3 是第3组寄存器。
{
	unsigned char i;
	WaitTime = 0;
	P1 = 0;
	if(RI)
	{
		for(i = 0; i < 3; i++)
		{
			while(!RI)		//循环检测
			{		// 防止因为发送端出错而造成卡死
				WaitTime++;
				if(WaitTime > 65500)
				{
					break;
				}
			}
			WaitTime = 0;

			Buff[i] = SBUF;		//SBUF串口寄存器,两个
			RI = 0;
		}
		Verify();
	}
}

void main(void)
{
	Init();

	while(1)
	{
		
	}
}

