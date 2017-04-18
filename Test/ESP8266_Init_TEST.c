#include <reg51.h>
#include <stdio.h>


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
	ES = 0;         //先关闭串口中断,WIFI模块初始化后再打开
	TI = 0;         //这两条在多个中断中一定要写，不然不能进入串口中断
	EA = 1;
}


void Init_ESP8266()
{
	unsigned int wait=1;
	ES=0;		//关闭串口中断
	Delayms(10);		//等待3秒后进行初始化
	
	TI=1;		//开启printf对串口输出
	printf("AT+RST\n");		//模块重启
	Delayms(10);
	do		//等待重启完成确认
	{
		if(RI)
		{
			if(SBUF == 'W')	//检测到ready WIFI CONNECTED WIFI GOT IP
				wait=0;
		}
	}while(wait);
	
	Delayms(20);
	wait=1;
	printf("AT+CIPMODE=1\n");		//模块开启透传
	do		//等待重启完成确认
	{
		if(RI)
		{
			if(SBUF == 'O')	//检测到OK
				wait=0;
		}
	}while(wait);

	Delayms(20);
	wait=1;
	printf("AT+CIPMUX=0\n");		//模块单链接模式
	do		//等待重启完成确认
	{
		if(RI)
		{
			if(SBUF == 'K')	//检测到OK
				wait=0;
		}
	}while(wait);

	Delayms(20);
	wait=1;
	printf("AT+CIPSTART=\"UDP\"\,\"255.255.255.225\"\,8080\,1111\,0\n");		//模块UDP服务开启,并监听,反斜杠后面原义输出
	do		//等待重启完成确认
	{
		if(RI)
		{
			if(SBUF == 'K')	//检测到OK
				wait=0;
		}
	}while(wait);

	Delayms(20);
	wait=1;
	printf("AT+CIPSEND\n");		//模块开启发送模式
	TI=0;		//关闭printf对串口输出
	do		//等待重启完成确认
	{
		if(RI)
		{
			if(SBUF == 'K')	//检测到OK
				wait=0;
		}
	}while(wait);

	Delayms(20);
	ES=1;		//开启串口中断
}


void main()
{
	Init();
	Delayms(10);
	Init_ESP8266();
	while(1){}		//不让主函数循环执行
}