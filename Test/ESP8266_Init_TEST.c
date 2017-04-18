#include <reg51.h>
#include <stdio.h>


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

//------------Chip��ʼ��-----------------
void Init()		//��������
{			  
	SCON = 0x50;         //������ʽ1
	PCON = 0;        //�����ʲ��ӱ�
	TMOD = 0x20;
	TH1 = 0xfd;     //11.0592����9600������
	TL1 = TH1;
	TR1 = 1;
	REN = 1;
	ES = 0;         //�ȹرմ����ж�,WIFIģ���ʼ�����ٴ�
	TI = 0;         //�������ڶ���ж���һ��Ҫд����Ȼ���ܽ��봮���ж�
	EA = 1;
}


void Init_ESP8266()
{
	unsigned int wait=1;
	ES=0;		//�رմ����ж�
	Delayms(10);		//�ȴ�3�����г�ʼ��
	
	TI=1;		//����printf�Դ������
	printf("AT+RST\n");		//ģ������
	Delayms(10);
	do		//�ȴ��������ȷ��
	{
		if(RI)
		{
			if(SBUF == 'W')	//��⵽ready WIFI CONNECTED WIFI GOT IP
				wait=0;
		}
	}while(wait);
	
	Delayms(20);
	wait=1;
	printf("AT+CIPMODE=1\n");		//ģ�鿪��͸��
	do		//�ȴ��������ȷ��
	{
		if(RI)
		{
			if(SBUF == 'O')	//��⵽OK
				wait=0;
		}
	}while(wait);

	Delayms(20);
	wait=1;
	printf("AT+CIPMUX=0\n");		//ģ�鵥����ģʽ
	do		//�ȴ��������ȷ��
	{
		if(RI)
		{
			if(SBUF == 'K')	//��⵽OK
				wait=0;
		}
	}while(wait);

	Delayms(20);
	wait=1;
	printf("AT+CIPSTART=\"UDP\"\,\"255.255.255.225\"\,8080\,1111\,0\n");		//ģ��UDP������,������,��б�ܺ���ԭ�����
	do		//�ȴ��������ȷ��
	{
		if(RI)
		{
			if(SBUF == 'K')	//��⵽OK
				wait=0;
		}
	}while(wait);

	Delayms(20);
	wait=1;
	printf("AT+CIPSEND\n");		//ģ�鿪������ģʽ
	TI=0;		//�ر�printf�Դ������
	do		//�ȴ��������ȷ��
	{
		if(RI)
		{
			if(SBUF == 'K')	//��⵽OK
				wait=0;
		}
	}while(wait);

	Delayms(20);
	ES=1;		//���������ж�
}


void main()
{
	Init();
	Delayms(10);
	Init_ESP8266();
	while(1){}		//����������ѭ��ִ��
}