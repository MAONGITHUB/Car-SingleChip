#ifndef _car_action_h_
#define _car_action_h_

#include<reg52.h>
#include<string.h>

#include"delay_uart.h"        //�����ӳ�_����ģ��
extern void Delayms( unsigned int z );


//--------------------�ַ���ö��-----------------
//enum symbolstruct{w,s,a,d} sign;

unsigned char order[6]={'w','s','a','d','b'};		//q�Ƕ�������,��delay_uart���趨


//--------------------��������-----------------
void Action()		//˲ʱ����
{
	switch(sign)
	{
		case 1:
		{
			P1=0xfe;
			Delayms(800);
			P1=0xff;
			break;
		}
		case 2:
		{
			P1=0xfd;
			Delayms(800);
			P1=0xff;
			break;
		}
		case 3:
		{
			P1=0xfb;
			Delayms(800);
			P1=0xff;
			break;
		}
		case 4:
		{
			P1=0xf7;
			Delayms(800);
			P1=0xff;
			break;
		}
		default:
			break;
	}
}

void StandingAction()		//�˶�����  wq\sq\aq\dq
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

//---------------------------������---------------
sbit P2_3=P2^3;		//������������
void Beep()
{
	P2_3=0;		//P2.3Ϊ�������ܽţ�������
	Delayms(1000);
	P2_3=1;
}

//---------------------�������ж����ź�ʶ��-----------

void Verify()
{		
		if (Action_Buff == order[0])		//��һ��ָ����
		{
			sign=1;
			Action();		//����Ϊ˲ʱ����
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
		//memset(Buff,0,2);		//Buff������0���������
}

#endif