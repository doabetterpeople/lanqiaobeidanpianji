#include <STC15F2K60S2.H>

void Delay(unsigned int t)
{
	while(t--);
	while(t--);
}

void HC138_Init(unsigned char x)   //HC138 INIT
{
	switch(x)
	{
		case 4: P2 =(P2&0x1f)|0x80;break; 
		case 5: P2 =(P2&0x1f)|0xa0;break; 
		case 6: P2 =(P2&0x1f)|0xc0;break; 
		case 7: P2 =(P2&0x1f)|0xe0;break; 
	}
}

void System_Init()   //系统初始化
{
	HC138_Init(5);
	P0 = 0x00;
	HC138_Init(4);
	P0 = 0xff;
}

void LED_Run()    //
{
	unsigned char i;
	for(i=0;i<3;i++)
	{
		HC138_Init(4);
		P0 = 0x00;
		Delay(60000);
		P0 = 0xff;
		Delay(60000);
	}
	for(i=0;i<8;i++)
	{
		HC138_Init(4);
		P0 = 0xfe << i; 
		Delay(60000);
	}
	
	HC138_Init(5);
	P0 = 0x10;
	Delay(60000);
	
	for(i=0;i<8;i++)
	{
		HC138_Init(4);
		P0 = ~(0xfe << i); 
		Delay(60000);
	}
	
	HC138_Init(5);
	P0 = 0x40;
	Delay(60000);
	P0 = 0x00;
}

void main()
{
	System_Init();
	while(1)
	{
		LED_Run();
	}
}