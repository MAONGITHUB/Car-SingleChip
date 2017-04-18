#ifndef _delay_uart_h_
#define _delay_uart_h_

#include<reg52.h>
#include<string.h>

#include"digital_led.h"     //调用数码管部分
extern struct Queue RBuff;    
extern void EnQueue(struct Queue *Q,char e);
extern void InitQueue(struct Queue *Q);

#include"car_action.h"	//调用车运动部分
extern void StandingAction();		//运动保持
extern void Verify();		//运动方向标志确定
      

//------------延时函数-----------------
void Delayms( unsigned int z )
{
	unsigned int x;
	unsigned int y;
	for( x = z; x > 0; x-- )
	{
		for( y = 125; y > 0; y-- );
	}
}

//------------串口发送--发送函数-----------------
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

//------------串口接收--------------------------------
void Receive() 
{
	//P1 = 0xff;
	display_ctr=0;		//关闭数码管显示循环,避免干扰,保证进入主函数‘@’的检测
	if(SBUF =='q' && Action_Buff!='q' && Action_Buff!='b')	//q置后
		{
			StandingAction();
			Action_Buff='q';
			InitQueue(&RBuff);
		}
	else if(SBUF=='w' || SBUF=='s' || SBUF=='a' || SBUF=='d' || SBUF=='b' )
		{
			Action_Buff=SBUF;
			Verify();
			InitQueue(&RBuff);
		}
		else if(SBUF>='0' && SBUF<='9')
		{
			EnQueue(&RBuff,SBUF);       //SBUF串口寄存器,两个地址一样99H,但物理上相互独立不会覆盖
			new_signal=1;       //标识有新数据进来
			//display_ctr=0;      
		}
}

//------------中断回置及缓和串口阻塞-----------
void Send_interrupt(void) interrupt 4 using 1
//中断0 外部中断0；中断1 定时器1中断；中断2 外部中断1；中断3 定时器2中断；中断4 串口中断
//using 0 是第0组寄存器。using 1 是第1组寄存器。using 2 是第2组寄存器。using 3 是第3组寄存器。           
{
	if(TI)      //TI在逻辑时序上先于RI触发
	{
		ES=0;
		sending=0;	//清除正在发送标志
		TI=0;		//发送中断回置
		ES=1;       
	}
	if(RI)    //收到数据
	{
		ES=0;   //关闭串口中断
		Receive();
		RI=0;   //清中断请求
		ES=1;   //打开串口中断
	}
}

#endif