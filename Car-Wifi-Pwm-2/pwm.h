#ifndef _pwm_h_
#define _pwm_h_

#include <stc89c52.h>

#include"digital_led.h"
extern void Status_display(int x);

sbit EN_L=P1^4;
sbit EN_R=P1^5;

int Lgear=10;		//左电机档位
int Rgear=10;		//右电机档位
static int pwm_ratio=10;
int pwm_left,pwm_right;

int timer_count=0;
static int maintain_time=6000;	//瞬时运动保持时间,以100us为倍


void pwm_interrupt(void) interrupt 1 using 2	//timer0中断
{
	timer_count++;
	pwm_left++;
	pwm_right++;
	TF0=0;
}

//--------------------PWM电机输出---------------------
void Motor(int L_oriention,int R_oriention)
{
	TF0=0;
	ET0=1;
	TR0=1;
	while(timer_count<=maintain_time)//(maintain_time/50))
	{
		if(pwm_left<=Lgear)
			P1=L_oriention;
			else
				EN_L=0;
		if(pwm_right<=Rgear)
			P1=R_oriention;
			else
				EN_R=0;
		if(pwm_left == pwm_ratio)
			pwm_left = 0;
		if(pwm_right == pwm_ratio)
			pwm_right = 0;
		if (timer_count==9)
			Status_display(timer_count);
	}
	TR0=0;
	ET0=0;
	Status_display(5);
	timer_count=0;

}

#endif