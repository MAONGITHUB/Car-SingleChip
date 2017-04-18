#include<reg52.h>

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
	SCON = 0x50;   //������ʽ1
	PCON = 0;	   //�����ʲ��ӱ�
	TMOD = 0x20;
	TH1 = 0xfd;	   //11.0592����9600������
	TL1 = TH1;
	TR1 = 1;
	REN = 1;
	ES = 1;		//�������ж�
	TI = 0;		//�������ڶ���ж���һ��Ҫд����Ȼ���ܽ��봮���ж�
	EA = 1;
}

//--------------------��������-----------------
void Action()		
{
	P1 = 0xf0;
	Delayms(1500);
	P1 = 0x0f;
	Delayms(1000);

}

//---------------------��ʶ���ж�----------------
unsigned char Buff[3];     //���ڻ���
unsigned char Order[3] = {'1','2','3'};

void Verify()		
{
	if(Buff[0] == Order[0])
	{
		 if(Buff[1] == Order[1])
		 {
			 if(Buff[2] == Order[2])
			 {
				Action();
			 }
		 }
	}
}

//------------���ڽ���--------------------------------
unsigned int WaitTime = 0; //���ڵȴ����յ�ʱ�����
void Receive() interrupt 4 using 1 		
//�ж�0 �ⲿ�ж�0���ж�1 ��ʱ��1�жϣ��ж�2 �ⲿ�ж�1���ж�3 ��ʱ��2�жϣ��ж�4 �����ж�
//using 0 �ǵ�0��Ĵ�����using 1 �ǵ�1��Ĵ�����using 2 �ǵ�2��Ĵ�����using 3 �ǵ�3��Ĵ�����
{
	unsigned char i;
	WaitTime = 0;
	P1 = 0;
	if(RI)
	{
		for(i = 0; i < 3; i++)
		{
			while(!RI)		//ѭ�����
			{		// ��ֹ��Ϊ���Ͷ˳������ɿ���
				WaitTime++;
				if(WaitTime > 65500)
				{
					break;
				}
			}
			WaitTime = 0;

			Buff[i] = SBUF;		//SBUF���ڼĴ���,����
			RI = 0;
		}
		Verify();
	}
}

void main(void)
{
	Init();

	while(1)
	{
		
	}
}

