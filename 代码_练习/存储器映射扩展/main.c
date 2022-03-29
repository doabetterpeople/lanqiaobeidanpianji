#include <STC15F2K60S2.H>
#include "absacc.h"

void Delay(unsigned int t)
{
	while(t--);
}
void System_Init()
{
	XBYTE[0xa000] = 0x00;
	XBYTE[0x8000] = 0xff;
}

void LED_Run()
{
	XBYTE[0x8000] = 0xf0;
	Delay(60000);
	Delay(60000);
	Delay(60000);
	XBYTE[0x8000] = 0x0f;
	Delay(60000);
	Delay(60000);
	Delay(60000);
	XBYTE[0x8000] = 0xff;
	Delay(60000);
	Delay(60000);
	Delay(60000);
}

void Nixie_Run()
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		XBYTE[0xc000] = 0x01 << i;
		XBYTE[0xe000] = 0x00;
		Delay(60000);
		Delay(60000);
		Delay(60000);
	}
	XBYTE[0xc000] = 0xff;
	XBYTE[0xe000] = 0xff;
}

void main()
{
	System_Init();
	while(1)
	{
		LED_Run();
		Nixie_Run();
	}
}

