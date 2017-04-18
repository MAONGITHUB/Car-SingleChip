#include <reg51.h>
#include <string.h>		//strlen()Ҫ�õ�
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
unsigned char Buff[12]={'1','2','3','4','q','9','w','6','5'};     //���ڻ���
unsigned int num[6];


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
  	while(1)
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
	n=strlen(Buff);		//buff��һ���ַ�����
	while(i<6 && j<n )		//ȡ�����ǰ6��Ϊ���ֵ������
	{						//atoi�ǽ��ַ�������ת��Ϊ���ְ�
		if (Buff[j]>='0' && Buff[j]<='9')
		{
			num[i]=Buff[j]-'0';		//num[i]=atoi(c);	
			i++;
		}
		j++;
	}
	Display(i);		//strlen()���ܼ�����������ĳ��ȣ�����С��
}

void main(void)
{
	Init();
	char_to_int();
	while(1){}
}