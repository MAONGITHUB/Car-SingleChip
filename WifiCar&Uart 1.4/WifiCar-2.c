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

//--------------------字符串枚举-----------------
//enum symbolstruct{w,s,a,d} sign;
int sign=0;

unsigned char Buff[2];     //串口缓存
unsigned char order[5]={'q','w','s','a','d'};


//--------------------动作函数-----------------
void Action()		
{
	switch(sign)
	{
		case 1:
		{
			P1=0xfe;
			Delayms(2000);
			P1=0xff;
			break;
		}
		case 2:
		{
			P1=0xfd;
			Delayms(2000);
			P1=0xff;
			break;
		}
		case 3:
		{
			P1=0xfb;
			Delayms(2000);
			P1=0xff;
			break;
		}
		case 4:
		{
			P1=0xf7;
			Delayms(2000);
			P1=0xff;
			break;
		}
		default:
			break;
	}
}

/*void StandingAction(sign)
{
	switch(sign)
	{
		case w:P1=0xfe;
		case s:P1=0xfd;
		case a:P1=0xfb;
		case d:P1=0xf7;
	}
} */

//---------------------动作组判定及信号识别-----------

void Verify()
{	
	if(Buff[0] == order[0])
	{
		if (Buff[1] == order[1]){sign=1;Action();}
			else if (Buff[1] == order[2]) {sign=2;Action();}
				else if (Buff[1] == order[3]) {sign=3;Action();}
					else if (Buff[1] == order[4]) {sign=4;Action();}
						//else Buff=0;
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
	P1 = 0xff;
	if(RI)
	{
		for(i = 0; i < 2; i++)
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

