#ifndef _ESP8266_h_
#define _ESP8266_h_

#include <reg52.h>
#include <stdio.h>

#include"delay_uart.h"        //调用延迟_串口模块


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
			if(SBUF == 'P')	//检测到ready WIFI CONNECTED WIFI GOT IP
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
			if(SBUF == 'K')	//检测到OK
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

#endif