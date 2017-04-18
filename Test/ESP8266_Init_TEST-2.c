//#include <reg52.h>
#include <stdio.h>
#include <stc89c52.h>
#include <string.h>		//strcmp()比较字符串且严格大小写,stricmp()不区分大小写

unsigned char respond_temp[10];

//------------延时函数-----------------
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
	unsigned int i=0;
	while(RI)
	{
		respond_temp[i]=SBUF;
		i++;
		RI=0;
	}	
}

void Respond_check(unsigned char respond_symbol)
{
	unsigned int w=1;
	unsigned int i=0;
	unsigned int respond_temp_len=strlen(respond_temp);
	while(w)
	{	
		for(i=0;i<respond_temp_len;i++)
		{
			if(respond_temp[i] == respond_symbol)	//strcmp()比较字符串且严格大小写,stricmp()不区分大小写
			{
				i=respond_temp_len;
				w=0;
                printf("haole\n");
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
	while(1)
	{
		Respond_input();
		if(TF1 == 1)	//定时器1溢出标志位
		{
			wait_count++;
			TH1=(65536-46080)/256;
			TL1=(65536-46080)%256;	//定时器1重置
			TF1= 0;
		}
		if(wait_count == 100) 
		{	
			//printf("errpr\n");
			break;
		}
	}
	
	Respond_check('O');

	Delayms(50);
    TI=1;
	printf("AT+CWMODE=1\n");
    TI=0;
	
	ES=1;		//打开串口中断
}



void main()
{
	//respond_temp=(char*) malloc (50);
	Init();
	Delayms(10);
	Init_ESP8266();
	while(1){}
}