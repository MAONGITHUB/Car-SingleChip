#include<reg52.h>

#define uchar unsigned char		//uchar����unsigned char

sbit dula=P2^6;
sbit wela=P2^7;
unsigned char j,k,c1,c0,m,n=255;
unsigned char pp;
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};

void delay(unsigned char i)
{
  for(j=i;j>0;j--)
    for(k=125;k>0;k--);
}

void display(uchar a,uchar b)
{
   P0=table[a];
   dula=1;
   dula=0;
   
   P0=0xef;
   wela=1;
   wela=0;
   delay(5);
   
   P0=table[b];
   dula=1;
   dula=0;
   
   P0=0xdf;
   wela=1;
   wela=0;
   delay(5);
}

void main()
{
	TMOD=0x01;		 //ģʽ���ã�00000001���ɼ����õ��Ƕ�ʱ��0��������ģʽ1��M1=0��M0=1����
	TR0=1;			 //�򿪶�ʱ��
	TH0=(65536-46080)/256;// ���ھ���Ϊ11.0592,�����Ǵ���ӦΪ46080����ʱ��ÿ��50000΢��=50ms����һ���жϡ�
	TL0=(65536-46080)%256;//46080��������Ϊ50000*11.0592/12
	//ET0=1;			 //����ʱ��0�ж�
//	EA=1;			 //�����ж�
	while(1)
	{
		if(TF0==1)			//��Ƭ��һֱ�ڲ�ѯ��ʱ��0�������־λTF0״̬
		{
			TF0=0;
			pp++;
			TH0=(65536-46080)/256;
			TL0=(65536-46080)%256;
		}
		if(pp==20)		//pp=20���ж�20�μ�20��20ms��1s
		{	pp=0;
			m++;
			if(m==2)
			{
				m=0;	   //������2s�������
			}
		}
		c0=m%10;		  //ȡ����ǰ�����ĸ�λ��ʮλ
		c1=m/10;
		display(c1,c0);	   //��ʾ
	}
}