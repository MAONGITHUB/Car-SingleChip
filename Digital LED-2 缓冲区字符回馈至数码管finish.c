#include <reg51.h>
#include <string.h>		//strlen()要用到
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
unsigned char Buff[12]={'1','2','3','4','q','9','w','6','5'};     //串口缓存
unsigned int num[6];


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
  	while(1)
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
	n=strlen(Buff);		//buff是一个字符数组
	while(i<6 && j<n )		//取输入的前6而为数字的数输出
	{						//atoi是将字符【串】转换为数字啊
		if (Buff[j]>='0' && Buff[j]<='9')
		{
			num[i]=Buff[j]-'0';		//num[i]=atoi(c);	
			i++;
		}
		j++;
	}
	Display(i);		//strlen()不能计算满额数组的长度，必须小于
}

void main(void)
{
	Init();
	char_to_int();
	while(1){}
}