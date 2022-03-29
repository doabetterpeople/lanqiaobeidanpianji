#include <STC15F2K60S2.H>
#include "iic.h"
#include "absacc.h"

sbit S4 = P3^3;
sbit S5 = P3^2;
unsigned char dat_rd1;
unsigned char level;
unsigned char back_rd1;
unsigned char back_level;

void Nixie_Run_old();


unsigned char code NixieTable[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
														0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,
														0xbf,0x7f};
void Delay(unsigned int t)
{
	while(t--);
}	

void Read_RD1()
{
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(0x01);
	IIC_WaitAck();
	IIC_Stop();
	
	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();
	dat_rd1 = IIC_RecByte();
	IIC_WaitAck();
	IIC_SendAck(0);
	IIC_Stop();
}	

unsigned char AT24C02_Read(unsigned char addr)
{
	unsigned char temp;
	IIC_Start();
	IIC_SendByte(0xA0);
	IIC_WaitAck();
	IIC_SendByte(addr);
	IIC_WaitAck();
	
	IIC_Start();
	IIC_SendByte(0xA1);
	IIC_WaitAck();
	temp = IIC_RecByte();
	IIC_WaitAck();
	IIC_SendAck(0);
	IIC_Stop();
	return temp;
}

void AT24C02_Write(unsigned char addr,unsigned char dat)
{
	IIC_Start();
	IIC_SendByte(0xA0);
	IIC_WaitAck();
	IIC_SendByte(addr);
	IIC_WaitAck();
	IIC_SendByte(dat);
	IIC_WaitAck();
	IIC_Stop();
}
void Right_auto()
{
	Read_RD1();
	if(dat_rd1>200)
	{
		XBYTE[0X8000] = 0XFE;
		level = 1;
	}
	else if(dat_rd1>150)
	{
		XBYTE[0X8000] = 0XFC;
		level = 2;
	}
	else if(dat_rd1>100)
	{
		XBYTE[0X8000] = 0XF0;
		level = 3;
	}
	else if(dat_rd1>50)
	{
		XBYTE[0X8000] = 0XC0;
		level = 4;
	}
	else if(dat_rd1>0)
	{
		XBYTE[0X8000] = 0X00;
		level = 5;
	}
}

void Nixie_Display(unsigned char pose,unsigned char dat)
{
	XBYTE[0XC000] = 0x01 <<pose;
	XBYTE[0XE000] = dat ;
}
void Nixie_Run_now()
{
	Nixie_Display(0,0xbf);
	Delay(500);
	Nixie_Display(1,NixieTable[level]);
	Delay(500);
	Nixie_Display(2,0xbf);
	Delay(500);
	Nixie_Display(3,0xff);
	Delay(500);
	Nixie_Display(4,0xff);
	Delay(500);
	Nixie_Display(5,NixieTable[dat_rd1/100]);
	Delay(500);
	Nixie_Display(6,NixieTable[(dat_rd1/10)%10]);
	Delay(500);
	Nixie_Display(7,NixieTable[dat_rd1%10]);
	Delay(500);
	
	XBYTE[0XA000] = 0xff;
	XBYTE[0XC000] = 0x00;
}

void Key_Scan()
{
	if(S4==0)
	{
		Delay(20);
		if(S4==0)
		{
			AT24C02_Write(0x01,level);
			AT24C02_Write(0x02,dat_rd1);
			while(S4==0)
			{
				Nixie_Run_now();
			}
		}
	}
	if(S5==0)
	{
		Delay(20);
		if(S5==0)
		{
			back_level = AT24C02_Read(0x01);
			back_rd1 = AT24C02_Read(0x02);
			while(S5==0)
			{
				Nixie_Run_old();
			}
		}
	}
}

void Nixie_Run_old()
{
	Nixie_Display(0,0xbf);
	Delay(500);
	Nixie_Display(1,NixieTable[back_level]);
	Delay(500);
	Nixie_Display(2,0xbf);
	Delay(500);
	Nixie_Display(3,0xff);
	Delay(500);
	Nixie_Display(4,0xff);
	Delay(500);
	Nixie_Display(5,NixieTable[back_rd1/100]);
	Delay(500);
	Nixie_Display(6,NixieTable[(back_rd1/10)%10]);
	Delay(500);
	Nixie_Display(7,NixieTable[back_rd1%10]);
	Delay(500);
	
	XBYTE[0XA000] = 0xff;
	XBYTE[0XC000] = 0x00;
}

void main()
{
	XBYTE[0X8000] = 0xff;
	XBYTE[0XA000] = 0x00;
	while(1)
	{
		Right_auto();
		Key_Scan();
		Nixie_Run_now();
	}
}