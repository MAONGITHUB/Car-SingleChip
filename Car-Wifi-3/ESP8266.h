#ifndef _ESP8266_h_
#define _ESP8266_h_

//#include <reg52.h>
#include <stdio.h>
#include <stc89c52.h>
#include <string.h>		//strcmp()比较字符串且严格大小写,stricmp()不区分大小写

unsigned char xdata respond_temp[20];
unsigned char xdata error_code[6]={0x79,0x50,0x50,0x5C,0x50,0x3F};		//esp8266初始化错误代码Error0


void Respond_input(int t)
{
	unsigned int i=0,wait_time=0;
	while(wait_time< (t*20))		//20次为1s,即60次3s
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
	unsigned int w=1,i=0,k=0;
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
					//printf("haole\n");
					memset(respond_temp,'z',10);
					break;
				}
			if(i == 9)
			{
				while(1)	//显示错误代码Error0
				{
					for(k=0;k<6;k++)
					{
						duanx=0;
						weix=0;
						P0=error_code[k];
						duanx=1;
						duanx=0;

						ResetP0();
						P0=wei[k];          //数码管位置选择,P0.0~0.5，由左至右，操作共阳，实际共阴
						weix=1;
						weix=0;
						Delayms(2);
					}
					P0=0x00;		//数码管关闭
					duanx=1,
					duanx=0;
				}
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

	Status_display(0);
	printf("AT+RST\n");
	Respond_input(3);
	Respond_check("IP");
	Delayms(100);

	Status_display(1);
	printf("AT+CIPMODE=1\n");
	Respond_input(1);
	Respond_check("OK");
	Delayms(100);	

	Status_display(2);
	printf("AT+CIPMUX=0\n");
	Respond_input(1);
	Respond_check("OK");
	Delayms(100);	

	Status_display(3);
	printf("AT+CIPSTART=\"UDP\"\,\"192.168.199.225\"\,8080\,1111\,0\n");
	Respond_input(1);
	Respond_check("OK");
	Delayms(100);	

	Status_display(4);
	printf("AT+CIPSEND\n");
	Respond_input(1);
	//Respond_check("OK");
	Delayms(100);

	Status_display(5);	
	//while(1);
	
	TI=0;
	ES=1;		//打开串口中断
}


#endif