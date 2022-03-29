#include <STC15F2K60S2.H>

sbit S4 = P3^3;
sbit S5 = P3^2; 



unsigned char code NixieTable[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
														0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,
														0xbf,0x7f};

void HC138_Init(unsigned int x)
{
	switch(x)
	{
		case 4: 	P2 =(P2 & 0x1f) | 0x80;    break;
		case 5:   P2 =(P2 & 0x1f) | 0xA0;    break;
		case 6: 	P2 =(P2 & 0x1f) | 0xC0;    break;
		case 7:   P2 =(P2 & 0x1f) | 0xE0;    break;
	}
}	

void Nixie_Display(unsigned char pose,unsigned char dat)
{
	HC138_Init(6);
	P0 = 0x01 <<pose;
	HC138_Init(7);
	P0 = dat;
}

//=======================================
void Timer0_Init()
{
	TMOD = 0x01;
	TH0 = (65565-50000)/256;
	TL0 = (65535-50000)%256;
	
	ET0 = 1;
	EA = 1;
}
unsigned char count,count_s,count_m;
void Timer_Service() interrupt 1
{	
	count++;
	TH0 = (65565-50000)/256;
	TL0 = (65535-50000)%256;

	if(count == 20)
	{
		count_s++;
		count=0;
		if(count_s ==60)
		{
			count_m++;
			count_s=0;
			if(count_m==60)
			{
				count_m=0;
			}
		}
	}
}
//=======================================

void Delay(unsigned int t)
{
	while(t--);
}

void Timer0_Display()
{
	Nixie_Display(7,NixieTable[count%10]);
	Delay(200);
	Nixie_Display(6,NixieTable[count/10]);
	Delay(200);
	Nixie_Display(5,NixieTable[16]);
	Delay(200);
	Nixie_Display(4,NixieTable[count_s%10]);
	Delay(200);
	Nixie_Display(3,NixieTable[count_s/10]);
	Delay(200);
	Nixie_Display(2,NixieTable[16]);
	Delay(200);
	Nixie_Display(1,NixieTable[count_m%10]);
	Delay(200);
	Nixie_Display(0,NixieTable[count_m/10]);
	Delay(200);
	HC138_Init(6);
	P0 = 0xff;
	HC138_Init(7);
	P0 = 0xff;
}
void Key_Scan()
{
	if(S4 == 0)
	{
		Delay(100);
		if(S4==0)
		{
			TR0 = ~TR0;
			while(S4==0)
			{
				Timer0_Display();
			}
		}
	}
	if(S5 == 0)
	{
		Delay(100);
		if(S5==0)
		{
			count=0;count_s=0;count_m=0;
			while(S5==0)
			{
				Timer0_Display();
			}
		}
	}
}

void main()
{
	Timer0_Init();
	while(1)
	{
		Timer0_Display();
		Key_Scan();
	}
}