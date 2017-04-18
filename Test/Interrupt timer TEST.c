#include <stc89c52.h>
#include <string.h>

int timer_count=0;
unsigned char xdata number[16]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71}; //数字0~F;小数点为P0.7(单独显示0x80)
unsigned char wei[6]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf};       //数码管位置选择

sbit duanx=P2^6;    //段选信号的锁存器控制
sbit weix=P2^7;   //位选信号的锁存器控制


void Init()
{
	TMOD = 0x02;	//定时器1/0,工作模式1(16位手动)/工作模式2(8位自动
	/*--------------------Timer0--------------------------------*/
	TH0=0xa4;		//100us预重装
	TL0=0x00;		//100us计时
	ET0=0;;		//定时器0中断允许
	TR0=0;	//定时器0开关
	TF0=0;	//中断标志位

	/*-------------------interrupt_control----------------------*/
	ES = 0;//先关闭串口中断,WIFI模块初始化后再打开
	EA = 1;//开启总中断
}

void pwm_interrupt(void) interrupt 1 using 0	//timer0中断
{
	timer_count++;
	TF0=0;
}

void main(void)
{
	Init();
	ET0=1;
	TR0=1;
	while(1)
	{
		if (timer_count==20000)
		{
			ET0=0;
			duanx=0;
			weix=0;
			P0=0x7D;	//选择显示数字,共阴
			duanx=1;
			duanx=0;
			P0=0xff;
			P0=0xfe;		//数码管位置0
			weix=1;
			weix=0;
		}
	}
}