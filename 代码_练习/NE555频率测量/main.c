#include <STC15F2K60S2.H>
#include "Nixie.h"

unsigned int count,count_s;
unsigned int dat;
void Timer_Init()   //定时器0用作计数模式，8位自动重装 
{										//定时器1用作定时模式，16位
	TMOD = 0x16;
	
	TH0 = 0xff;
	TL0 = 0xff;
	
	TH1 = (65535-50000)/256;
	TL1 = (65535-50000)%256;
	
	ET0 = 1;  
	ET1 = 1;

	EA = 1;
	
	TR0 = 1;
	TR1 = 1;
}

void Timer0_Service() interrupt 1
{
	count++;
}

void Timer1_Service() interrupt 3
{
	TH1 = (65535-50000)/256;
	TL1 = (65535-50000)%256;
	count_s++;
	if(count_s == 20)
	{
		dat = count;
		count = 0;
		count_s = 0;
	}
}

void Display()
{
	Display_NixieBit(0,0x8e);
	Delay_Nixie(100);
	Display_NixieBit(1,0xff);
	Delay_Nixie(100);
	Display_NixieBit(2,0xff);
	Delay_Nixie(100);
	
	if(dat>9999)
	{
		Display_NixieBit(3,NixieTableNoDot[dat/10000]);
		Delay_Nixie(100);
	}
	if(dat>999)
	{
		Display_NixieBit(4,NixieTableNoDot[(dat/1000)%10]);
		Delay_Nixie(100);
	}
	if(dat>99)
	{
		Display_NixieBit(5,NixieTableNoDot[(dat/100)%10]);
		Delay_Nixie(100);
	}
	if(dat>9)
	{
		Display_NixieBit(6,NixieTableNoDot[(dat/10)%10]);
		Delay_Nixie(100);
	}
	Display_NixieBit(7,NixieTableNoDot[dat%10]);
	Delay_Nixie(100);
	
	Display_All(0xff);
	Delay_Nixie(100);
}

void main()
{
	Timer_Init();
	while(1)
	{
		Display();
	}
}