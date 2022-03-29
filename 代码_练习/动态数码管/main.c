#include <STC15F2K60S2.H>

unsigned char code NixieTable[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
														0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,
														0xbf,0x7f};
unsigned char month =1;
void Delay(unsigned int t)
{
	while(t--);
}
														
void HC138_Init(unsigned char x)
{
	switch(x)
	{
		case 4: P2 = (P2&0x1f)|0x80; break;
		case 5: P2 = (P2&0x1f)|0xa0; break;
		case 6: P2 = (P2&0x1f)|0xc0; break;
		case 7: P2 = (P2&0x1f)|0xe0; break;
	}
}	

void System_init()
{
	HC138_Init(4);
	P0 =0xff;
	HC138_Init(5);
	P0 = 0x00;
}
void Nixie_Display(unsigned char i,unsigned char dat)
{
	HC138_Init(6);
	P0 = 0x01 << i;
	HC138_Init(7);
	P0 = dat;
}

void Nixie_Run()
{
	Nixie_Display(0,NixieTable[2]);
	Delay(100);
	Nixie_Display(1,NixieTable[0]);
	Delay(100);
	Nixie_Display(2,NixieTable[1]);
	Delay(100);
	Nixie_Display(3,NixieTable[8]);
	Delay(100);
	Nixie_Display(4,NixieTable[16]);
	Delay(100);
	Nixie_Display(5,NixieTable[16]);
	Delay(100);
	
	Nixie_Display(6,NixieTable[month/10]);
	Delay(100);
	Nixie_Display(7,NixieTable[month%10]);
	Delay(100);
}
void Delay_s(unsigned int t)
{
	while(t--)
	{
		Nixie_Run();
	}
}
void main()
{
	System_init();
	while(1)
	{
		month++;
		if(month>12)
		{
			month=1;
		}
		Nixie_Run();
		Delay_s(100);
	}
}