#include <STC15F2K60S2.H>

sbit L1 = P0^0;
sbit L2 = P0^1;
sbit L3 = P0^2;
sbit L4 = P0^3;
sbit L5 = P0^4;
sbit L6 = P0^5;

sbit S4 = P3^3;
sbit S5 = P3^2;
sbit S6 = P3^1;
sbit S7 = P3^0;

void Delay(unsigned int t)
{
	while(t--);
}

void HC138_init(unsigned char channel)
{
	switch(channel)
	{
		case 4: P2 = (P2&0x1f)|0x80;break;
		case 5: P2 = (P2&0x1f)|0xa0;break;
		case 6: P2 = (P2&0x1f)|0xc0;break;
		case 7: P2 = (P2&0x1f)|0xe0;break;
	}
}
void System_Init()
{
	HC138_init(5);
	P0 = 0x00;
	HC138_init(4);
	P0 = 0xff;
}
unsigned char flag;
void Key_Scan()
{
	if(S7==0)
	{
		Delay(10);
		if(S7==0&&flag==0)
		{
			flag=1;
			while(S7==0);
			HC138_init(4);
			L1 = 0;
		}
		if(S7==0&&flag==1)
		{
			flag=0;
			while(S7==0);
			HC138_init(4);
			L1 = 1;
		}
	}
	if(S6==0)
	{
		Delay(10);
		if(S6==0&&flag==0)
		{
			flag=2;
			while(S6==0);
			HC138_init(4);
			L2 = 0;
		}
		if(S6==0&&flag==2)
		{
			flag=0;
			while(S6==0);
			HC138_init(4);
			L2 = 1;
		}
	}
	if(flag==1)
	{
		if(S5==0)
		{
			Delay(100);
			while(S5==0)
			{
				HC138_init(4);
				L3 = 0;
				while(S5==0);
				HC138_init(4);
				L3 = 1;
			}
		}
		if(S4==0)
		{
			Delay(100);
			while(S4==0)
			{
				HC138_init(4);
				L4 = 0;
				while(S4==0);
				HC138_init(4);
				L4 = 1;
			}
		}	
	}
	if(flag==2)
	{
		if(S5==0)
		{
			Delay(100);
			while(S5==0)
			{
				HC138_init(4);
				L5 = 0;
				while(S5==0);
				HC138_init(4);
				L5 = 1;
			}
		}
		if(S4==0)
		{
			Delay(100);
			while(S4==0)
			{
				HC138_init(4);
				L6 = 0;
				while(S4==0);
				HC138_init(4);
				L6 = 1;
			}
		}	
	}
}

void main()
{
	System_Init();
	while(1)
	{
		Key_Scan();
	}
}
