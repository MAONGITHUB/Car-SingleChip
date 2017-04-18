#include<reg52.h>

sbit key1=P3^4;
sbit key2=P3^5;
sbit key3=P3^6;
sbit key4=P3^7;

sbit led=P1^0;

void Delayms( unsigned int z )
{
	unsigned int x;
	unsigned int y;
	for( x = z; x > 0; x-- )
	{
		for( y = 125; y > 0; y-- );
	}
}
void main()
{
	while(1)		   
	{
		if(key3==0)
		{
			P1=0x31;
			Delayms(500);
			P1=0x00;
		}
		if(key4==0)
		{
			P1=0x32;
			Delayms(500);
			P1=0x00;
		}
	}
}