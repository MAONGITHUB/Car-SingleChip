#include<reg52.h>
#include<string.h>

unsigned int num[7];
unsigned int display_ctr=0;     // �������ʾѭ������
unsigned int sending=0,new_signal=0;        //sending���ڻ��⴮�����ݷ��ͽ��������,new_signal�����ж��Ƿ��������ݽ���
unsigned int sign=0;
unsigned char Action_Buff='q';

#include"digital_led.h"     //��������ܲ���
extern struct Queue RBuff;     //digital_led.h
extern void InitQueue(struct Queue *Q);     //ѭ�����г�ʼ��
extern unsigned char SerQueue(struct Queue *Q,int r);       // rΪ��Ҫ���ҵĵ�ǰλ��
extern void char_to_int();     //�ַ�ת�������ֲ����


struct Queue RBuff;     //Rbuff��һ����̬ѭ������/�ַ�����

//----------��ʼ��-------------
void Init()
{
	//��������
	SCON = 0x50;	//������ʽ1
	PCON = 0;	//�����ʲ��ӱ�
	TMOD = 0x20;
	TH1 = 0xfd;	//11.0592����9600������
	TL1 = TH1;
	TR1 = 1;
	REN = 1;        //REN��ַ��ֵΪ���ƴ������ݽ��յ�������߽�ֹ
	ES = 1;	//�������ж�
	TI = 0;     //�������ڶ���ж���һ��Ҫд����Ȼ���ܽ��봮���ж�
	EA = 1;
}

void main(void)
{
	Init();
	InitQueue(&RBuff);
	while(1)  //����������ѭ��ִ�в���ѭ����⣨��ѯ��ʽ��
	{
		if(SerQueue(&RBuff,RBuff.rear) == '@' && new_signal)
		{
			char_to_int();
		}
	}       
}