#include <STC15F2K60S2.H>

sbit L1 = P0^0;
sbit L8 = P0^7;

void HC138_Init(unsigned char channel)
{
	switch(channel)
	{
		case 4: P2 = (P2 & 0x1f)|0x80;break;
		case 5: P2 = (P2 & 0x1f)|0xa0;break;
		case 6: P2 = (P2 & 0x1f)|0xc0;break;
		case 7: P2 = (P2 & 0x1f)|0xe0;break;
	}
}

void System_Init()
{
	HC138_Init(5);
	P0 = 0x00;
	HC138_Init(4);
	P0 = 0xff;
}
//=================================
void Timer0_Init()
{
	TMOD = 0X01;
	TH0 = (65535-50000)/256;
	TL0 = (65535-50000)%256;
	
	TR0 = 1;
	ET0 = 1;
	EA = 1;
}
unsigned char flag;
void Timer0_Service() interrupt 1
{
	TH0 = (65535-50000)/256;
	TL0 = (65535-50000)%256;
	flag++;
	if(!(flag%10))
	{
		L1 = ~L1;
	}
	if(flag == 100)
	{
		L8 = ~L8;
		flag=0;
	}
}
//=================================
void main()
{
	System_Init();
	Timer0_Init();
	while(1)
	{
		
	}
}