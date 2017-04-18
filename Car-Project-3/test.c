#include <reg51.h>
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
char SBuff[4];
unsigned int num[6];
int display_ctr=0;
int sending=0;		//sending���ڻ��⴮�����ݷ��ͽ��������
int p=0;

//------------��̬ѭ�����У����飩����ͷָ�룩����-----------------
int maxsize=8;
typedef struct Queue	//�洢�ṹ
{
	char elem[maxsize];
	int rear=-1;
};

void EnQueue(struct Queue *Q,char e)
{
	Q.rear = (Q.rear+1) % maxsize;
	Q.elem[Q.rear] = e;
}

char SerQueue(struct Queue *Q,int r)		// rΪ��Ҫ���ҵĵ�ǰλ��
{		//�鿴����Ԫ��ʱ��λ��Ϊ������ȡ���б����������Բ��죬��������Ը�д
	if (r<0)
		return Q[maxsize+1+r];
	else
		return Q[r];
}

struct Queue RBuff;

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
void Display(int x )
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

//------------�ַ�ת��������--------------------------------
void char_to_int()
{
	int i=0,n=0,j=0;
	n=strlen(RBuff);		//Rbuff��һ���ַ�����
	while(i<6 && j<n )		//ȡ�����ǰ6��Ϊ���ֵ������
	{						//atoi�ǽ��ַ�������ת��Ϊ���ְ�
		if (RBuff[j]>='0' && RBuff[j]<='9')
		{
			num[i]=RBuff[j]-'0';		//num[i]=atoi(c);	
			i++;
		}
		j++;
	}
	memset(RBuff,0,12);
	display_ctr=1;
	Delayms(1000);
	Display(i);		//strlen()���ܼ�����������ĳ��ȣ�����С��
}

//------------���ڷ���--���ͺ���-----------------
void Send(char type,char cmd)
{ 
	char SBuff[4];	  //�������ݰ�����ǰ��ȥ��
	char *p;
	int Send_Count=0;
	*p = SBuff;
	SBuff[0]=0XFF;		//ǰ��У����
	SBuff[1]=type;
	SBuff[2]=cmd;
	SBuff[3]=0XFF;		//����У����
	 
	
		SBUF = SBuff[1];  //����
		sending=1;	//�������ڷ��ͱ�־
		while(1)
		{
			if (sending==0) break;
		}
		//_nop_();       //΢���ʱ����(ָ��),12MHz��ʱ1us
		SBUF = SBuff[2];
		sending=1;
		while(1)
		{
			if (sending==0) break;
		}
}

//------------���ڽ���--------------------------------
void Receive() 
{
	P1 = 0xff;
	display_ctr=0;
	EnQueue(RBuff,SBUF);		//SBUF���ڼĴ���,������ַһ��99H,���������໥�������Ḳ��
}

//------------�жϻ��ü����ʹ�������-----------
void Send_interrupt(void) interrupt 4 using 1
//�ж�0 �ⲿ�ж�0���ж�1 ��ʱ��1�жϣ��ж�2 �ⲿ�ж�1���ж�3 ��ʱ��2�жϣ��ж�4 �����ж�
//using 0 �ǵ�0��Ĵ�����using 1 �ǵ�1��Ĵ�����using 2 �ǵ�2��Ĵ�����using 3 �ǵ�3��Ĵ�����			 
{
	if(TI)      //TI���߼�ʱ��������RI����
	{
		sending=0;	//������ڷ��ͱ�־
		TI=0;			//�����жϻ���
	}
	if(RI)    //�յ�����
	{
		ES=0;	//�رմ����ж�
		Receive();
		RI=0;   //���ж�����
		ES=1;	//�򿪴����ж�
		if(SerQueue(RBuff,RBuff.rear) == '@')
		{
			Send(Serqueue(RBuff,(RBuff.rear-1)),Serqueue(RBuff,(RBuff.rear-2)));
		}
	}
}
	

void main(void)
{
	Init();
	while(1){}		//����������ѭ��ִ��
}