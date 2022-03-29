#include <STC15F2K60S2.H>

unsigned char code NixieTable[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
														0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,
														0xbf,0x7f};

void Delay(unsigned int t)
{
	while(t--);
}	
														
void HC138_Init(unsigned char x)
{
	switch(x)
	{
		case 4: P2 = (P2&0X1F)|0X80; break;
		case 5: P2 = (P2&0X1F)|0Xa0; break;
		case 6: P2 = (P2&0X1F)|0Xc0; break;
		case 7: P2 = (P2&0X1F)|0Xe0; break;
	}
}	

void System_Init()
{
	HC138_Init(4);
	P0 = 0xff;
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
	unsigned char i,j;
	for(i=0;i<8;i++)
	{
		for(j=0;j<10;j++)
		{
			Nixie_Display(i,NixieTable[j]);
			Delay(60000);
		}
	}
	
	for(i=0;i<16;i++)
	{
		HC138_Init(6);
		P0 = 0xff;
		HC138_Init(7);
		P0 = NixieTable[i];
		Delay(60000);
	}

}
void main()
{
	System_Init();
	while(1)
	{
		Nixie_Run();
	}
}
