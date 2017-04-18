#ifndef _car_action_h_
#define _car_action_h_

#include <stc89c52.h>
#include <string.h>

#include"delay_uart.h"        //调用延迟_串口模块
extern void Delayms( unsigned int z );


//--------------------字符串枚举-----------------
//enum symbolstruct{w,s,a,d} sign;

unsigned char order[6]={'w','s','a','d','b'};		//q是动作锁定,在delay_uart中设定


//--------------------动作函数-----------------
void Action()		//瞬时动作
{
	switch(sign)
	{
		case 1:
		{
			P1=0xfe;
			Delayms(100);
			P1=0xff;
			break;
		}
		case 2:
		{
			P1=0xfd;
			Delayms(100);
			P1=0xff;
			break;
		}
		case 3:
		{
			P1=0xfb;
			Delayms(100);
			P1=0xff;
			break;
		}
		case 4:
		{
			P1=0xf7;
			Delayms(100);
			P1=0xff;
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
			P1=0xfe;
			break;
		}
		case 2:
		{
			P1=0xfd;
			break;
		}
		case 3:
		{
			P1=0xfb;
			break;
		}
		case 4:
		{
			P1=0xf7;
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