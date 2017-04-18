#include <reg52.h>
#include <string.h>		//strlen()Ҫ�õ�
#include <intrins.h>
#include <stdio.h>
//#include <math.h> 		//���η� pow(��ֵ,���η�)
//#include <stdlib.h>		//atoi()Ҫ�õ���

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
unsigned char wei[6]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf};		//�����λ��ѡ��

sbit duanx=P2^6;    //��ѡ�źŵ�����������
sbit weix=P2^7;   //λѡ�źŵ�����������
//unsigned char RBuff[8];		//={'1','2','3','4','q','9','w','6','5'};     //���ڻ���
unsigned int num[7];
unsigned int display_ctr=0;
unsigned int sending=0,new_signal=0;		//sending���ڻ��⴮�����ݷ��ͽ��������,new_signal�����ж��Ƿ��������ݽ���
int p=0;

//------------��̬ѭ�����У����飩����ͷָ�룩����-----------------
#define maxsize 20 	//��Ч�ַ����������ڵ���maxsize�����ĳ��λ��������޷���ʾ���������������̬����ı׶˰�
typedef struct Queue		//�洢�ṹ
{
	char elem[maxsize];
	int rear;
};

void InitQueue(struct Queue *Q)		//��ʼ��,����������ֱ��д��������Queue,��ûд����ı���,ֻ����ָ��*Qû�취ȡ��ַ��&Q
{
	memset(RBuff.elem,'z',8);		//���RBuff���ַ�,����������0,�Ա�ͳ����������ַ�
							//������ú���Ҫ��ָ����õ���ʼλ
	Q->rear=-1;
}

void EnQueue(struct Queue *Q,char e)	//������
{
	Q->rear = (Q->rear+1) % maxsize;
	Q->elem[Q->rear] = e;
}

unsigned char SerQueue(struct Queue *Q,int r)		// rΪ��Ҫ���ҵĵ�ǰλ��
{		//�鿴����Ԫ��ʱ��λ��Ϊ������ȡ���б����������Բ��죬��������Ը�д
	if (r<0)
		return Q->elem[maxsize+1+r];
	else
		return Q->elem[r%maxsize];
}

struct Queue RBuff;		//Rbuff��һ���ַ�����

//------------��ʱ����-----------------
void Delayms( unsigned int z )
{
	unsigned int x;
	unsigned int y;
	for( x = z; x > 0; x-- )
	{
		for( y = 110; y > 0; y-- );
	}
}

//----------��ʼ��-------------
void Init()
{
					//��������
	SCON = 0x50;  		 //������ʽ1
	PCON = 0;	  	 //�����ʲ��ӱ�
	TMOD = 0x20;
	TH1 = 0xfd;	   	//11.0592����9600������
	TL1 = TH1;
	TR1 = 1;
	REN = 1;
	ES = 1;			//�������ж�
	TI = 0;			//�������ڶ���ж���һ��Ҫд����Ȼ���ܽ��봮���ж�
	EA = 1;
}

//------------P0������--------------------------------
void ResetP0()
{
  	P0=0xff;
}

//------------����ܶ�̬��ʾ--------------------------------
void Display(int x )	//x���ڿ����������ʾ����
{								//P0.0~0.7����573������
	int y,k;
  	while(display_ctr && !RI)
 	{
 		for(k=0;k<x;k++)
  		{
  			duanx=0;
  			weix=0;
  			y=num[k];
  			P0=number[y];    	 //ѡ����ʾ����,����
  			duanx=1;
  			duanx=0;

	  		ResetP0();		
  			P0=wei[k];			//�����λ��ѡ��,P0.0~0.5���������ң�����������ʵ�ʹ���
  			weix=1;
 	 		weix=0;

 	 		Delayms(2);
 	 	}		
  	}
}

//------------���ڷ���--���ͺ���-----------------
void Send(int message)
{
	sending=1;
	SBUF = message;
	Delayms(10);
	while(1)
	{
		if (sending==0) break;
	}
}

//------------���ڷ�����Ϣѡ��-----------------
void Sort_send(int length)
{
	unsigned n=0;
	while(n<length)
	{
		Send(num[n]);
		n++;
	}
	display_ctr=1;		//�򴮿ڷ���������������ʾ
	Delayms(100);
	Display(length);		//strlen()���ܼ�����������ĳ��ȣ�����С��
}

//------------�ַ�ת�������ֲ����--------------------------------
void char_to_int()
{
	int i=0,n=0,k=0;
	int j=RBuff.rear-1;
	n=strlen(RBuff.elem);		//�������ַ�������0��ֹͣͳ��
	while(i<6 && k<n)		//ȡ�����ǰ6��Ϊ���ֵ������(�����ֻ��6��),������������
	{						//atoi�ǽ��ַ�������ת��Ϊ���ְ�
		if (SerQueue(&RBuff,j-k)>='0' && SerQueue(&RBuff,j-k)<='9')
		{
			num[i]=SerQueue(&RBuff,j-i)-'0';		//num[i]=atoi(c);	
			i++;		//����num����
		}
		k++;		//����ѭ��RBuff����
	}
	InitQueue(&RBuff);		//���RBuff���ַ�,����������0,�Ա�ͳ����������ַ�
	Sort_send(i);
}

//------------���ڽ���--------------------------------
void Receive() 
{
	//P1 = 0xff;
	//display_ctr=0;
	EnQueue(&RBuff,SBUF);		//SBUF���ڼĴ���,������ַһ��99H,���������໥�������Ḳ��
	new_signal=1;		//��ʶ�������ݽ���
	display_ctr=0;		//�ر��������ʾѭ��,�������,��֤������������@���ļ��
}

//------------�жϻ��ü����ʹ�������-----------
void Send_interrupt(void) interrupt 4 using 1
//�ж�0 �ⲿ�ж�0���ж�1 ��ʱ��1�жϣ��ж�2 �ⲿ�ж�1���ж�3 ��ʱ��2�жϣ��ж�4 �����ж�
//using 0 �ǵ�0��Ĵ�����using 1 �ǵ�1��Ĵ�����using 2 �ǵ�2��Ĵ�����using 3 �ǵ�3��Ĵ�����			 
{
	if(TI)      //TI���߼�ʱ��������RI����
	{
		ES=0;
		sending=0;	//������ڷ��ͱ�־
		TI=0;	
		ES=1;		//�����жϻ���
	}
	if(RI)    //�յ�����
	{
		ES=0;	//�رմ����ж�
		Receive();
		RI=0;   //���ж�����
		ES=1;	//�򿪴����ж�
	}
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