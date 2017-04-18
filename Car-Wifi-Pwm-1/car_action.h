#ifndef _car_action_h_
#define _car_action_h_

#include <stc89c52.h>
#include <string.h>

#include"delay_uart.h"        //调用延迟_串口模块
extern void Delayms( unsigned int z );

static int maintain_time=6;	//瞬时运动保持时间,以50us为单位

static int ratio=100;		//1khz拆分比例
int gear=9;		//档位
int Lgear=9;		//左电机档位
int Rgear=9;		//右电机档位

//--------------------字符串枚举-----------------
//enum symbolstruct{w,s,a,d} sign;

unsigned char order[6]={'w','s','a','d','b'};		//q是动作锁定,在delay_uart中设定

//--------------------PWM电机输出---------------------
void Pwm(int L_oriention,int R_oriention)
{
	int p=0;
	int timer0_count=0;

	while(timer0_count<=maintain_time)	//以100ms为单位
	{
		if(TF1 == 1)
		{
			p++;
			TH1=(65536-46080)/256;
			TL1=(65536-46080)%256;	//定时器1重置,1/(50us)频率
			TF1 = 0;
		}
		if(p==100)//ratio)	//占空比比例满额
			p=0;
		if(p<=90)//(ratio-Lgear))
			P0=0x01;//L_oriention;
		else
			P0=0x00;
		if(p<=80)//(ratio-Rgear))
			P0=0x02;//R_oriention;
		else
			P0=0x00;
		if(TF0 == 1)
		{
			timer0_count++;
			TH0=(65536-46080)/256;
			TL0=(65536-46080)%256;	//定时器0重置,1/(50us)频率
			TF0 = 0;
		}
	}
}

/*--------------------动作参数-----------------
左电机：前IN2  后IN1  使能EN1
右电机：前IN3  后IN4  使能EN2
左：IN2-P1.0    IN1-P1.2    EN-P1.4
右：IN3-P1.1    IN4-P1.3    EN-P1.5
-------------------------------------------------*/


/*--------------------动作函数-----------------*/
void Action()		//瞬时动作
{
	switch(sign)
	{
		case 1:
		{
			Pwm(0x31,0x32);	//0011  0001/0010
			//Delayms(main_time);
			P1=0x00;
			break;
		}
		case 2:
		{
			Pwm(0x38,0x34);	//0011  1000/0100
			//Delayms(main_time);
			P1=0x00;
			break;
		}
		case 3:
		{
			Pwm(0x00,0x32);	//0011  0000/0010
			//Delayms(main_time);
			P1=0x00;
			break;
		}
		case 4:
		{
			Pwm(0x31,0x00);	//0011  0001/0000
			//Delayms(main_time);
			P1=0x00;
			break;
		}
		default:
			break;
	}
}

void StandingAction()		//运动保持  wq\sq\aq\dq
{
	switch(sign)
	{
		case 1:
		{
			P1=0x13;
			break;
		}
		case 2:
		{
			P1=0x2c;
			break;
		}
		case 3:
		{
			P1=0x12;
			break;
		}
		case 4:
		{
			P1=0x11;
			break;
		}
		default:
			break;
	}
} 

//---------------------------蜂鸣器---------------
sbit P2_3=P2^3;		//必须这样定义
void Beep()
{
	P2_3=0;		//P2.3为蜂鸣器管脚，共阳！
	Delayms(1000);
	P2_3=1;
}

//---------------------动作组判定及信号识别-----------

void Verify()
{
		if (Action_Buff == order[0])		//第一个指令检测
		{
			sign=1;
			Action();		//否则为瞬时动作
		}
		else if (Action_Buff  == order[1]) 
		{
			sign=2;
			Action();
		}
			else if (Action_Buff  == order[2]) 
			{
				sign=3;
				Action();
			}
				else if (Action_Buff  == order[3]) 
				{
					sign=4;
					Action();
				}
					else if(Action_Buff == order[4])
					{
						Beep();
					}
		//memset(Buff,0,2);		//Buff数组置0，避免干扰
}

#endif