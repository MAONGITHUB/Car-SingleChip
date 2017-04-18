#ifndef _delay_uart_h_
#define _delay_uart_h_

#include<reg52.h>
#include<string.h>

#include"digital_led.h"     //��������ܲ���
extern struct Queue RBuff;    
extern void EnQueue(struct Queue *Q,char e);       

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

//------------���ڽ���--------------------------------
void Receive() 
{
    //P1 = 0xff;
    //display_ctr=0;
    EnQueue(&RBuff,SBUF);       //SBUF���ڼĴ���,������ַһ��99H,���������໥�������Ḳ��
    new_signal=1;       //��ʶ�������ݽ���
    display_ctr=0;      //�ر��������ʾѭ��,�������,��֤������������@���ļ��
}

//------------�жϻ��ü����ʹ�������-----------
void Send_interrupt(void) interrupt 4 using 1
//�ж�0 �ⲿ�ж�0���ж�1 ��ʱ��1�жϣ��ж�2 �ⲿ�ж�1���ж�3 ��ʱ��2�жϣ��ж�4 �����ж�
//using 0 �ǵ�0��Ĵ�����using 1 �ǵ�1��Ĵ�����using 2 �ǵ�2��Ĵ�����using 3 �ǵ�3��Ĵ�����           
{
    if(TI)      //TI���߼�ʱ��������RI����
    {
        ES=0;
        sending=0;  //������ڷ��ͱ�־
        TI=0;   
        ES=1;       //�����жϻ���
    }
    if(RI)    //�յ�����
    {
        ES=0;   //�رմ����ж�
        Receive();
        RI=0;   //���ж�����
        ES=1;   //�򿪴����ж�
    }
}

#endif