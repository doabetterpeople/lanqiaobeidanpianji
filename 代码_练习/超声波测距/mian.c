#include <STC15F2K60S2.H>
#include "absacc.h"
#include "intrins.h"

sbit TX = P1^0;
sbit RX = P1^1;
unsigned int distance=0;
unsigned char code NixieTable[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
														0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,
														0xbf,0x7f};
													
void Delay12us()		//@12.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 33;
	while (--i);
}

void Send()
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		TX = 1;
		Delay12us();
		TX = 0;
		Delay12us();
	}
}

void Distance_test()
{
	unsigned char time;
	TMOD &= 0x0f;
	TL1 = 0x00;
	TH1 = 0x00;
	Send();
	TR1 = 1;
	while((RX==1)&&(TF1==0));
	TR1 = 0;
		
	if(TF1==0)
	{
		time = TH1;
		time = (time<<8)|TL1;
		distance = ((time/10)*17)/100+3;
	}
	else
	{
		TF1 = 0;
		distance = 999;
	}
}
void Nixie_Display(unsigned char addr,unsigned char dat)
{
	XBYTE[0XC000] = 0X01 << addr;
	XBYTE[0XE000] = dat;
}

void Delay(unsigned int t)
{
	while(t--);
}
void Nixie_Run()
{
	if(distance == 999)
	{
		Nixie_Display(0,0x8e);
		Delay(500);
	}
	else 
	{
		Nixie_Display(0,0xff);
		Delay(500);
		Nixie_Display(1,0xff);
		Delay(500);
		Nixie_Display(2,0xff);
		Delay(500);
		Nixie_Display(3,0xff);
		Delay(500);
		Nixie_Display(4,0xff);
		Delay(500);
		Nixie_Display(5,NixieTable[distance/100]);
		Delay(500);
		Nixie_Display(6,NixieTable[(distance%10)/10]);
		Delay(500);
		Nixie_Display(7,NixieTable[distance%10]);
		Delay(500);
		XBYTE[0XC000] = 0XFF;
		XBYTE[0XE000] = 0XFF;
	}
}

void Delay_S(unsigned char t)
{
	while(t--)
	{
		Nixie_Run();
	}
}

void main()
{
	XBYTE[0X8000] = 0xff;
	XBYTE[0XA000] = 0x00;
	while(1)
	{
		Distance_test();
		Delay_S(10);
	}
}