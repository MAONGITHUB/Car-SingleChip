#include <stdio.h>
#include <stc89c52.h>

void Delayms( unsigned int z )
{
	unsigned int x;
	unsigned int y;
	for( x = z; x > 0; x-- )
{
	for( y = 110; y > 0; y-- );
 }
}
  
void Init()
{
	//AUXR &= 0xFB;     //定时器2时钟为Fosc/12,即12T,T2在预分频禁止时默认为12分频
	//AUXR |= 0x01;		//串口1选择定时器2为波特率发生器,已经在T2CON设置R/TCLK

	RCAP2H = 0xff;  	//T2高8位重载值
	RCAP2L = 0xdc;	    //T2低8位重载值
	TH2 = RCAP2H;	    //T2初值
	TL2 = RCAP2L;	    //T2初值     
		
	PCON = 0;       //波特率不加倍	
	T2CON = 0x34;	    //T2控制设置,在R/TCLK确定T2作为串口波特率产生器 TF2 EXF2 (RCLK) (TCLK) EXEN2 (TR2) C/T2 CP/RL2
	T2MOD = 0x01;	    //T2模式设置, D7 D6 D5 D4 D3 D2 T2OE(输出到P1.0) DCEN(向下计数允许)
	SCON = 0x50;	    //串口工作方式1,8位数据,可变波特率
	REN = 1;	//启用串口接收

    /*--------------------Timer1--------------------------------*/
	TMOD = 0x10;	//定时器1,工作模式1(8位)
	TH1=(65536-46080)/256;		//所记次数应为46080,计时器每隔50000us即50ms发起一次中断
	TL1=(65536-46080)%256;		//46080=50000*11.0592/12
	TR1 = 1;	//打开定时器1

    /*-------------------interrupt_control----------------------*/
	ES = 0;//先关闭串口中断,WIFI模块初始化后再打开
	TI = 0;//串口输出中断需先置0避免引起冲突
	EA = 1;//开启总中断
}

void Send()
{ 
	printf("AT+CIPSTART=\"UDP\"\,\"192.168.199.225\"\,8080\,1111\,0\n");
}
 
void Send_interrupt(void) interrupt 4                    
{
	if(TI)          
	{
		TI=0;           
	}
}

void main()
{
	Init();
	Delayms(1000);
	ES=0;
	TI=1;
	Send();
	Delayms(10);
	ES=0;
	while(1){}              
}