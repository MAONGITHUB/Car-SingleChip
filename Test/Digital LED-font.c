#include <reg51.h>

sbit duanx=P2^6;    //段选信号的锁存器控制
sbit weix=P2^7;   //位选信号的锁存器控制
sbit led0=P0^0;sbit led1=P0^1;sbit led2=P0^2;sbit led3=P0^3;sbit led4=P0^4;sbit led5=P0^5;
unsigned char number[16]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71}; //数字0~F,小数点为P0.7(单独显示0x80)

void ResetP0()
{
  P0=0xff;
}

void main()
{   //P0.0~0.7控制573锁存器
  duanx=0;
  weix=0;

  P0=number[7];     //选择显示数字,共阴
  duanx=1;
  duanx=0;

  ResetP0();
  led0=0;
  led5=0;    //数码管位置选择,P0.0~0.5，由左至右，操作共阳，实际共阴
  weix=1;
  weix=0;

  while(1);
}

