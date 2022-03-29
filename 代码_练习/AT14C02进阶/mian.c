#include <STC15F2K60S2.H>
#include "absacc.h"
#include "iic.h"

sbit S4 = P3^3;
sbit S5 = P3^2;
sbit S6 = P3^1;

unsigned char code NixieTable[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
														0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,
														0xbf,0x7f};

void Nixie_Run();
unsigned char IIC_Read(unsigned char addr)
{
	unsigned char temp;
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck(); 
	IIC_SendByte(addr);
	IIC_WaitAck(); 
	IIC_Start();
	IIC_SendByte(0xa1);
	IIC_WaitAck(); 
	temp=IIC_RecByte();
	IIC_WaitAck();
	IIC_SendAck(0);
	IIC_Stop(); 
	return temp;
}

void IIC_Write(unsigned char addr,unsigned char dat)
{
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck(); 
	IIC_SendByte(addr);
	IIC_WaitAck();
	IIC_SendByte(dat);
	IIC_WaitAck();
	IIC_Stop(); 
}
unsigned char key1,key2,key3;
void Delay(unsigned int t)
{
	while(t--);
}
void Key_Scan()
{
	if(S4==0)
	{
		Delay(20);
		if(S4==0)
		{
			while(S4==0){Nixie_Run();}
			key1++;
			if(key1 > 13)
			{
				key1 = 0;
			}
		}
	}
	if(S5==0)
	{
		Delay(20);
		if(S5==0)
		{
			while(S5==0){Nixie_Run();}
			key2++;
			if(key2 > 13)
			{
				key2 = 0;
			}
		}
	}
	if(S6==0)
	{
		Delay(10);
		if(S6==0)
		{
			while(S6==0);
			key3++;
			if(key3>13)
			{
				key3 = 0;
			}
		}
	}
	IIC_Write(0x00,key1);
	Delay(1000);
	IIC_Write(0x01,key2);
	Delay(1000);
	IIC_Write(0x02,key3);
	Delay(1000);
}

void Nixie_Display(unsigned char pose,unsigned char dat)
{
	XBYTE[0XC000] = 0x01 <<pose;
	XBYTE[0XE000] = dat ;
}

void Nixie_Run()
{
	Nixie_Display(0,NixieTable[key1/10]);
	Delay(500);
	Nixie_Display(1,NixieTable[key1%10]);
	Delay(500);
	Nixie_Display(2,0xbf);
	Delay(500);
	Nixie_Display(3,NixieTable[key2/10]);
	Delay(500);
	Nixie_Display(4,NixieTable[key2%10]);
	Delay(500);
	Nixie_Display(5,0xbf);
	Delay(500);
	Nixie_Display(6,NixieTable[key3/10]);
	Delay(500);
	Nixie_Display(7,NixieTable[key3%10]);
	Delay(500);
	
	XBYTE[0XA000] = 0xff;
	XBYTE[0XC000] = 0x00;
}
void main()
{
	key1=IIC_Read(0x00);
	key2=IIC_Read(0x01);
	key3=IIC_Read(0x02);
	while(1)
	{
		Key_Scan();
		Nixie_Run();
	}
}
