#include<reg52.h>
#include<string.h>

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

//--------------------�ַ���ö��-----------------
//enum symbolstruct{w,s,a,d} sign;
int sign=0;

unsigned char Buff[2];     //���ڻ���
unsigned char order[5]={'w','s','a','d','q'};		//q�Ƕ�������


//--------------------��������-----------------
void Action()		//˲ʱ����
{
	switch(sign)
	{
		case 1:
		{
			P1=0xfe;
			Delayms(2000);
			P1=0xff;
			break;
		}
		case 2:
		{
			P1=0xfd;
			Delayms(2000);
			P1=0xff;
			break;
		}
		case 3:
		{
			P1=0xfb;
			Delayms(2000);
			P1=0xff;
			break;
		}
		case 4:
		{
			P1=0xf7;
			Delayms(2000);
			P1=0xff;
			break;
		}
		default:
			break;
	}
}

void StandingAction()		//�˶�����
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
	}
} 

//---------------------�������ж����ź�ʶ��-----------

void Verify()
{		
		if (Buff[0] == order[0])		//��һ��ָ����
		{
			sign=1;
			if (Buff[1] == order[4])		//�ڶ���ָ����ͬ��ʵ���˶�����
				StandingAction();
			else Action();		//����Ϊ˲ʱ����
		}

		else if (Buff[0] == order[1]) 
		{
			sign=2;
			if (Buff[1] == order[4])
					StandingAction();
			else Action();
		}

			else if (Buff[0] == order[2]) 
			{
				sign=3;
				if (Buff[1] == order[4])
					StandingAction();
				else Action();
			}

				else if (Buff[0] == order[3]) 
				{
					sign=4;
					if (Buff[1] == order[4])
						StandingAction();
					else Action();
				}
		memset(Buff,0,2);		//Buff������0���������
}

//------------���ڽ���--------------------------------
unsigned int WaitTime = 0; //���ڵȴ����յ�ʱ�����
void Receive() interrupt 4 using 1 		
//�ж�0 �ⲿ�ж�0���ж�1 ��ʱ��1�жϣ��ж�2 �ⲿ�ж�1���ж�3 ��ʱ��2�жϣ��ж�4 �����ж�
//using 0 �ǵ�0��Ĵ�����using 1 �ǵ�1��Ĵ�����using 2 �ǵ�2��Ĵ�����using 3 �ǵ�3��Ĵ�����
{
	unsigned char i;
	WaitTime = 0;
	P1 = 0xff;
	if(RI)
	{
		for(i = 0; i < 2; i++)
		{
			while(!RI)		//ѭ�����
			{		// ��ֹ��Ϊ���Ͷ˳������ɿ���
				WaitTime++;
				if(WaitTime > 2000)		//ԭʼֵ65500������ط��ͷ�Ӧ�ٶ��й�
				{
					//i=2;		//�񼼰�����ͨ����ʱ���ֱ������forѭ��	 �����а���
					break;		//��ʱ����whileѭ�����������жϼ��Ĵ���sbuf���
				}
			}
			WaitTime = 0;

			Buff[i] = SBUF;		//SBUF���ڼĴ���,����,99H,�������໥����
			RI = 0;		 //�жϻ���0
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

