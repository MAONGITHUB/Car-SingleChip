#ifndef _pwm_h_
#define _pwm_h_

#include <stc89c52.h>

int Lgear=9;		//左电机档位
int Rgear=9;		//右电机档位
int pwm_ratio=10;

void pwm_interrupt()interrupt 1 using 2	//timer0中断
{
	timer0_count++;
	pwm_left++;
	pwm_right++;
}

void motor(int L_oriention,int R_oriention)
{
	while(timer0_count<=(maintain_time/50))
	{
		if(pwm_left<=Lgear)
			P1=L_oriention;
			else
				P1=0x03;
		if(pwm_right<=Rgear)
			P1=R_oriention;
			else
				P1=0x03;
		if(pwm_left == pwm_ratio)
			pwm_left = 0;
		if(pwm_right == pwm_ratio)
			pwm_right = 0;
	}
}
