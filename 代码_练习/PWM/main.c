#include <STC15F2K60S2.H>

sbit L1 = P0^0;
sbit S7 = P3^0;

void HC138_Init(unsigned char channel)
{
	switch(channel)
	{
		case 4: P2 = (P2&0x1f) | 0x80;  break;
		case 5: P2 = (P2&0x1f) | 0xa0;  break;
		case 6: P2 = (P2&0x1f) | 0xc0;  break;
		case 7: P2 = (P2&0x1f) | 0xe0;  break;
	}
}
//============定时器相关======================
void Timer0_Init()
{
	TMOD = 0x01;
	TH0 = (65535-100)/256;
	TL0 = (65535-100)%256;
	ET0= 1;
	EA = 1;
	TR0 = 1;
}

unsigned char count,compare;

void Timer0_Service() interrupt 1
{
	TH0 = (65535-100)/256;
	TL0 = (65535-100)%256;
	count++;
	count= count%100;
	if(count<compare)
	{
		HC138_Init(4);
		L1=0;
	}
	else
	{
		HC138_Init(4);
		L1=1;
	}
}

//===========按键相关==============

void Delay(unsigned int t)
{
	while(t--);
}

unsigned char flag=0;
void LED_Run()
{
	if(flag==0)  compare=0;
	if(flag==1)  compare=10;
	if(flag==2)  compare=50;
	if(flag==3)  compare=90;
}
	
void Key_Scan()
{
	if(S7 == 0)
	{
		Delay(100);
		if(S7 ==0)
		{
			flag++;
			flag= flag%4;
			while(S7==0)
			{
				LED_Run();
			}
		}
	}
}

void main()
{
	Timer0_Init();
	while(1)
	{
		Key_Scan();
	}
}