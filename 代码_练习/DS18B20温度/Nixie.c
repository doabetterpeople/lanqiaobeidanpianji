#include <STC15F2K60S2.H>
#include "absacc.h"

void Delay_Nixie(unsigned int t)
{
	while(t--);
}

void Display_NixieBit(unsigned char i,unsigned char dat)
{
	XBYTE[0XC000] = 0x01<<i;
	XBYTE[0XE000] = dat;
}

void Display_All(unsigned char dat)
{
	XBYTE[0XC000] = 0xff;
	XBYTE[0XE000] = dat;	
}