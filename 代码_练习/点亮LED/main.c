#include <STC15F2K60S2.H>

void HC138_Init(unsigned char x)
{
	switch(x)
	{
		case 4: P2 = (P2 &0x1f)|0x80;break;
		case 5: P2 = (P2 &0x1f)|0xa0;break;
		case 6: P2 = (P2 &0x1f)|0xc0;break;
		case 7: P2 = (P2 &0x1f)|0xe0;break;
	}
}

void Delay(unsigned int t)
{
	while(t--);
	while(t--);
}

void Led_Run()
{
	unsigned char i,j;
	for(i=0;i<3;i++)
	{
		HC138_Init(4);
		P0 = 0x00;
		Delay(60000);
		P0 = 0xff;
		Delay(60000);
	}
	for(j=0;j<8;j++)
	{
		HC138_Init(4);
		P0 = 0xfe << j;
		Delay(60000);
	}
	for(j=0;j<8;j++)
	{
		HC138_Init(4);
		P0 = ~(0xfe << j);
		Delay(60000);
	}
}

void Led_Init()
{
	HC138_Init(4);
	P0 = 0xff;
}
void main()
{
	Led_Init();
	while(1)
	{
		Led_Run();
	}
}
