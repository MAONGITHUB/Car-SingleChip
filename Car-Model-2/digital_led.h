#ifndef _digital_led_h_
#define _digital_led_h_


#include <reg52.h>
#include <string.h>     //strlen()Ҫ�õ�
#include <intrins.h>
#include <stdio.h>

#include"delay_uart.h"        //�����ӳ�_����ģ��


 /*
 atof()     ���ַ���ת��Ϊ˫���ȸ�����ֵ
 atoi()     ���ַ���ת��Ϊ����ֵ
 atol()     ���ַ���ת��Ϊ������ֵ
 strtod()   ���ַ���ת��Ϊ˫���ȸ�����ֵ�������治�ܱ�ת��������ʣ������
 strtol()   ���ַ���ת��Ϊ����ֵ�������治�ܱ�ת��������ʣ������
 strtoul()  ���ַ���ת��Ϊ�޷��ų�����ֵ�������治�ܱ�ת��������ʣ������
 */

//sbit led0=P0^0;sbit led1=P0^1;sbit led2=P0^2;sbit led3=P0^3;sbit led4=P0^4;sbit led5=P0^5;

unsigned char number[16]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71}; //����0~F,С����ΪP0.7(������ʾ0x80)
unsigned char wei[6]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf};       //�����λ��ѡ��

sbit duanx=P2^6;    //��ѡ�źŵ�����������
sbit weix=P2^7;   //λѡ�źŵ�����������


//------------��̬ѭ�����У����飩����ͷָ�룩����-----------------
#define maxsize 10  //��Ч�ַ����������ڵ���maxsize�����ĳ��λ��������޷���ʾ���������������̬����ı׶˰�
typedef struct Queue        //�洢�ṹ
{
	char elem[maxsize];
	int rear;
};

void InitQueue(struct Queue *Q)     //��ʼ��,����������ֱ��д��������Queue,��ûд����ı���,ֻ����ָ��*Qû�취ȡ��ַ��&Q
{
	memset(Q->elem,'z',8);       //���RBuff���ַ�,����������0,�Ա�ͳ����������ַ�
							//������ú���Ҫ��ָ����õ���ʼλ
	Q->rear=-1;
}

void EnQueue(struct Queue *Q,char e)    //������
{
	Q->rear = (Q->rear+1) % maxsize;
	Q->elem[Q->rear] = e;
}

unsigned char SerQueue(struct Queue *Q,int r)       // rΪ��Ҫ���ҵĵ�ǰλ��
{       //�鿴����Ԫ��ʱ��λ��Ϊ������ȡ���б����������Բ��죬��������Ը�д
	if (r<0)
		return Q->elem[maxsize+1+r];
	else
		return Q->elem[r%maxsize];
}

//------------P0������--------------------------------
void ResetP0()
{
	P0=0xff;
}

//------------����ܶ�̬��ʾ--------------------------------
void Display(int x)    //x���ڿ����������ʾ����(����������ﰡ,ÿ��Action��Ⱥܾòż���x,x�Ĵ���������)
{                               //P0.0~0.7����573������
	int y,k;
	while(display_ctr && !RI)
	{
		for(k=0;k<x;k++)
		{
			duanx=0;
			weix=0;
			y=num[k];
			P0=number[y];        //ѡ����ʾ����,����
			duanx=1;
			duanx=0;

			ResetP0();
			P0=wei[k];          //�����λ��ѡ��,P0.0~0.5���������ң�����������ʵ�ʹ���
			weix=1;
			weix=0;
			Delayms(2);
		}     
	}
	P0=0x00;		//����ܹر�
	duanx=1,
	duanx=0;
}

//------------���ڷ�����Ϣѡ��-----------------
void Sort_send(int len)
{
	int n=0;
	while(n<len)
	{
		Send(num[n]);
		n++;
	}
	display_ctr=1;      //�򴮿ڷ���������������ʾ
	Delayms(100);
	Display(len);        //strlen()���ܼ�����������ĳ��ȣ�����С��
}

//------------�ַ�ת�������ֲ����--------------------------------
void char_to_int()
{
	int i=0,n=0,k=0;
	int j=RBuff.rear-1;
	n=strlen(RBuff.elem);       //�������ַ�������0��ֹͣͳ��
	while(i<6 && k<n)       //ȡ�����ǰ6��Ϊ���ֵ������(�����ֻ��6��),������������
	{                       //atoi�ǽ��ַ�������ת��Ϊ���ְ�
		if (SerQueue(&RBuff,j-k)>='0' && SerQueue(&RBuff,j-k)<='9')
		{
			num[i]=SerQueue(&RBuff,j-i)-'0';        //num[i]=atoi(c);   
			i++;        //����num����
		}
		k++;        //����ѭ��RBuff����
	}
	InitQueue(&RBuff);      //���RBuff���ַ�,����������0,�Ա�ͳ����������ַ�
	Sort_send(i);
}



#endif
