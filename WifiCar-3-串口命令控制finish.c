#include<reg52.h>
#include<string.h>

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
unsigned char order[5]={'w','s','a','d','q'};		//q是动作锁定


//--------------------动作函数-----------------
void Action()		//瞬时动作
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

void StandingAction()		//运动保持
{
	switch(sign)
	{
		case 1:
		{
			P1=0xfe;
			break;
		}
		case 2:
		{
			P1=0xfd;
			break;
		}
		case 3:
		{
			P1=0xfb;
			break;
		}
		case 4:
		{
			P1=0xf7;
			break;
		}
	}
} 

//---------------------动作组判定及信号识别-----------

void Verify()
{		
		if (Buff[0] == order[0])		//第一个指令检测
		{
			sign=1;
			if (Buff[1] == order[4])		//第二个指令相同，实现运动保持
				StandingAction();
			else Action();		//否则为瞬时动作
		}

		else if (Buff[0] == order[1]) 
		{
			sign=2;
			if (Buff[1] == order[4])
					StandingAction();
			else Action();
		}

			else if (Buff[0] == order[2]) 
			{
				sign=3;
				if (Buff[1] == order[4])
					StandingAction();
				else Action();
			}

				else if (Buff[0] == order[3]) 
				{
					sign=4;
					if (Buff[1] == order[4])
						StandingAction();
					else Action();
				}
		memset(Buff,0,2);		//Buff数组置0，避免干扰
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
				if(WaitTime > 2000)		//原始值65500，这个地方和反应速度有关
				{
					//i=2;		//神技啊！！通过延时检测直接跳出for循环	 （不行啊）
					break;		//超时跳出while循环，即跳出中断及寄存器sbuf检测
				}
			}
			WaitTime = 0;

			Buff[i] = SBUF;		//SBUF串口寄存器,两个,99H,物理上相互独立
			RI = 0;		 //中断回置0
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

