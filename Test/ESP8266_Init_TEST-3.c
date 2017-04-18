//#include <reg52.h>
#include <stdio.h>
#include <stc89c52.h>
#include <string.h>		//strcmp()比较字符串且严格大小写,stricmp()不区分大小写

unsigned char respond_temp[10];

//------------1ms单位延时函数-----------------
void Delayms( unsigned int z )
{
	unsigned int x;
	unsigned int y;
	for( x = z; x > 0; x-- )
	{
		for( y = 125; y > 0; y-- );
	}
}



//------------Chip初始化-----------------
void Init()
{
	//AUXR &= 0xFB;		//定时器2时钟为Fosc/12,即12T,T2在预分频禁止时默认为12分频
	//AUXR |= 0x01;		//串口1选择定时器2为波特率发生器,已经在T2CON设置R/TCLK

	RCAP2H = 0xff;	//T2高8位重载值
	RCAP2L = 0xdc;	//T2低8位重载值
	TH2 = RCAP2H;	//T2初值
	TL2 = RCAP2L;	//T2初值     
		
	PCON = 0;       //波特率不加倍	
	T2CON = 0x34;	    //T2控制设置,在R/TCLK确定T2作为串口波特率产生器 TF2 EXF2 (RCLK) (TCLK) EXEN2 (TR2) C/T2 CP/RL2
	T2MOD = 0x01;	    //T2模式设置, D7 D6 D5 D4 D3 D2 T2OE(输出到P1.0) DCEN(向下计数允许)
	SCON = 0x50;	    //串口工作方式1,8位数据,可变波特率
	REN = 1;	//启用串口接收

	/*--------------------Timer1--------------------------------*/
	TMOD = 0x10;	//定时器1,工作模式1(16位)
	TH1=(65536-46080)/256;		//所记次数应为46080,计时器每隔50000us即50ms发起一次中断
	TL1=(65536-46080)%256;		//46080=50000*11.0592/12
	TR1 = 1;	//打开定时器1

	/*-------------------interrupt_control----------------------*/
	ES = 0;//先关闭串口中断,WIFI模块初始化后再打开
	TI = 0;//串口输出中断需先置0避免引起冲突
	EA = 1;//开启总中断
}

void Respond_input()
{
	unsigned int i=0,wait_time=0;
	while(wait_time<100)		//20次为1s,即100次5s
	{
		if(RI)
		{
			respond_temp[i%10]=SBUF;	//回显字符载入循环数组,只保留后10个字符
			i++;
			RI=0;
		}
		if(TF1 == 1)		//定时器1溢出标志位
		{
			wait_time++;
			TH1=(65536-46080)/256;
			TL1=(65536-46080)%256;	//定时器1重置
			TF1= 0;
		}
	}	
}

void Respond_check(unsigned char* symbol)	//标志字符串必须为两位
{
	unsigned int w=1,i=0;
	//unsigned int len=0;
	//len=strlen(symbol);		//检测标志字符串长度
	unsigned char symbol_temp[3];		//最后一位存/0
	strcpy(symbol_temp,symbol);	//将字符串转为字符数组

	while(w)	//w为循环等待标志
	{	
		for(i=0;i<10;i++)	//循环 转换后的回显缓存数组
		{
			if(respond_temp[i] == symbol_temp[0])	//第一个标志位检测
				if(respond_temp[(i+1)%10] == symbol_temp[1])	//第二个标志位检测
				{
					i=10;
					w=0;
					printf("haole\n");
					break;
				}
		}
	}
}

void Init_ESP8266()
{
	unsigned int wait=1;
	unsigned int wait_count=0;
	
	ES=0;		//关闭串口中断
	Delayms(10);		
	
	TI=1;		//打开串口输出,使printf起作用
	printf("AT+RST\n");
	TI=0;
	Respond_input();
	Respond_check("OK");

	Delayms(50);
	TI=1;
	printf("AT+CWMODE=1\n");
	TI=0;
	
	//ES=1;		//打开串口中断
}



void main()
{
	//respond_temp=(char*) malloc (50);
	Init();
	Delayms(10);
	Init_ESP8266();
	while(1){}
}