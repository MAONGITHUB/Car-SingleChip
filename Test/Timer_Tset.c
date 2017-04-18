#include<reg52.h>

#define uchar unsigned char		//uchar看作unsigned char

sbit dula=P2^6;
sbit wela=P2^7;
unsigned char j,k,c1,c0,m,n=255;
unsigned char pp;
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};

void delay(unsigned char i)
{
  for(j=i;j>0;j--)
    for(k=125;k>0;k--);
}

void display(uchar a,uchar b)
{
   P0=table[a];
   dula=1;
   dula=0;
   
   P0=0xef;
   wela=1;
   wela=0;
   delay(5);
   
   P0=table[b];
   dula=1;
   dula=0;
   
   P0=0xdf;
   wela=1;
   wela=0;
   delay(5);
}

void main()
{
	TMOD=0x01;		 //模式设置，00000001，可见采用的是定时器0，工作与模式1（M1=0，M0=1）。
	TR0=1;			 //打开定时器
	TH0=(65536-46080)/256;// 由于晶振为11.0592,故所记次数应为46080，计时器每隔50000微秒=50ms发起一次中断。
	TL0=(65536-46080)%256;//46080的来历，为50000*11.0592/12
	//ET0=1;			 //开定时器0中断
//	EA=1;			 //开总中断
	while(1)
	{
		if(TF0==1)			//单片机一直在查询定时器0的溢出标志位TF0状态
		{
			TF0=0;
			pp++;
			TH0=(65536-46080)/256;
			TL0=(65536-46080)%256;
		}
		if(pp==20)		//pp=20即中断20次即20个20ms即1s
		{	pp=0;
			m++;
			if(m==2)
			{
				m=0;	   //若到了2s，则归零
			}
		}
		c0=m%10;		  //取出当前描述的个位与十位
		c1=m/10;
		display(c1,c0);	   //显示
	}
}
