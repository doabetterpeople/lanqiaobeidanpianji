#include <STC15F2K60S2.H>

sbit L1 = P0^0;
sbit L8 = P0^7;

void Delay(unsigned int t)
{
	while(t--);
}

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
//===============================================
void EXTI0_Init()
{	
	IT0 = 1;
	EX0 = 1;
	EA = 1;
}
unsigned char flag;
void EXTI_Service() interrupt 0
{
	flag = 1; 
}
//===============================================
void LED_Run()
{
	HC138_Init(4);
	L1 = 0;
	Delay(60000);
	L1 = 1;
	Delay(60000);
}
void L8_Run()
{
	if(flag==1)
	{
	L8 = 0;
	Delay(60000);
	Delay(60000);
	L8 = 1;
	Delay(60000);
	Delay(60000);
	flag = 0;
	}

}
void main()
{
	System_Init();
	EXTI0_Init();
	while(1)
	{
		LED_Run();
		L8_Run();
	}
}