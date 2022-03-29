#include <STC15F2K60S2.H>
#include "absacc.h"
#include "iic.h"
unsigned char dat_rd3;
unsigned char code NixieTable[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
														0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,
														0xbf,0x7f};



void Delay(unsigned int t)
{
	while(t--);
}

void Nixie_Display(unsigned char addr,unsigned char dat)
{
	XBYTE[0XC000] = 0X01<<addr;
	XBYTE[0XE000] = dat; 
}



void Read_Rd3()
{
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(0x03);
	IIC_WaitAck();
	IIC_Stop();
	
	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();
	dat_rd3 = IIC_RecByte();
	IIC_WaitAck();
	IIC_SendAck(0);
	IIC_Stop();
}

void Write_DAC(unsigned char dat)
{
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(0x40);
	IIC_WaitAck();
	IIC_SendByte(dat);
	IIC_WaitAck();
	IIC_Stop();
}

void Nixie_Run()
{
	XBYTE[0XC000] = 0X1f;
	XBYTE[0XE000] = 0xff; 
	Delay(500);
	
	Nixie_Display(5,NixieTable[dat_rd3/100]);
	Delay(500);
	Nixie_Display(6,NixieTable[(dat_rd3%100)/10]);
	Delay(500);
	Nixie_Display(7,NixieTable[dat_rd3%10]);
	Delay(500);
	
	XBYTE[0XC000] = 0Xff;
	XBYTE[0XE000] = 0xff;
}
														
void main()
{
	while(1)
	{
		Read_Rd3();
		Nixie_Run();
		Write_DAC(dat_rd3);
	}
}	