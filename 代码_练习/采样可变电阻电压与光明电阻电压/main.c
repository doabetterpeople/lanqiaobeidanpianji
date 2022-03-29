#include <STC15F2K60S2.H>
#include "absacc.h"
#include "iic.h"

void Nixie_Run(unsigned char channel,unsigned char dat);
sbit S4 = P3^3;
unsigned char code NixieTable[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
														0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,
														0xbf,0x7f};
void Delay(unsigned int t)
{
	while(t--);
}	
unsigned char temp1=0;
unsigned char temp2=0;
void AIN3_Read()
{

	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(0x03);
	IIC_WaitAck();
	IIC_Stop();
	
	//Nixie_Run(3,temp2);
	
	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();
	temp2 =IIC_RecByte();
	IIC_WaitAck();
	IIC_SendAck(0);
	IIC_Stop();
	Nixie_Run(3,temp2);
}
void AIN1_Read()
{
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(0x01);
	IIC_WaitAck();
	IIC_Stop();
	
	//Nixie_Run(1,temp1);
	
	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();
	temp1 =IIC_RecByte();
	IIC_WaitAck();
	IIC_SendAck(0);
	IIC_Stop();
	Nixie_Run(1,temp1);
}
unsigned char flag=1;
void Key_Scan()
{
	if(S4==0)
	{
		Delay(50);
		if(S4==0)
		{
			if(flag==1)
			{
				flag=2;
				while(S4==0)
				{
					Nixie_Run(1,temp1);
				}
			}
			else if(flag==2) 
			{
				flag=1;
				while(S4==0)
				{
					Nixie_Run(3,temp2);
				}
			}
		}
	}
}
void Nixie_Display(unsigned char pose,unsigned char dat)
{
	XBYTE[0XC000] = 0x01 <<pose;
	XBYTE[0XE000] = dat ;
}

void Nixie_Run(unsigned char channel,unsigned char dat)
{
	Nixie_Display(0,0xbf);
	Delay(500);
	Nixie_Display(1,NixieTable[channel]);
	Delay(500);
	Nixie_Display(2,0xbf);
	Delay(500);
	Nixie_Display(3,0xff);
	Delay(500);
	Nixie_Display(4,0xff);
	Delay(500);
	Nixie_Display(5,NixieTable[dat/100]);
	Delay(500);
	Nixie_Display(6,NixieTable[(dat/10)%10]);
	Delay(500);
	Nixie_Display(7,NixieTable[dat%10]);
	Delay(500);
	
	XBYTE[0XA000] = 0xff;
	XBYTE[0XC000] = 0x00;
}
void main()
{
	XBYTE[0X8000] = 0xff;
	XBYTE[0Xa000] = 0x00;
	while(1)
	{
		Key_Scan();
		if(flag==1)
		{
			AIN1_Read();
		}
		else if(flag==2)
		{
			AIN3_Read();
		}
	}
}

