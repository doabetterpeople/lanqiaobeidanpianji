#include <STC15F2K60S2.H>
#include "iic.h"
#include "Nixie.h"
#include "onewire.h"
#include "absacc.h"

sbit S4 = P3^3;

unsigned char temp;
void DS18B20_Read()
{
	unsigned char LSB,MSB;
	init_ds18b20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0x44);
	
	init_ds18b20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0xbe);
	
	LSB = Read_DS18B20();
	MSB = Read_DS18B20();
	
	temp = (MSB<<8)|LSB;
	if(temp&0xf800==0x0000)
	{
		temp=temp>>4;
		temp = temp*10;
		temp = temp+LSB*0.0625;
	}
}
void Delay(unsigned int t)
{
	while(t--);
}
void Nixie_Run()
{
	Display_NixieBit(0,0xc6);
	Delay(200);
	Display_NixieBit(1,NixieTableNoDot[temp/100]);
	Delay(200);
	Display_NixieBit(2,NixieTableDot[temp/10%10]);
	Delay(200);
	Display_NixieBit(3,NixieTableNoDot[temp%10]);
	Delay(200);
	XBYTE[0XC000] = 0xf0;
	XBYTE[0XE000] = 0xff;
	Delay(200);
}


void Timer0Init(void)		//50毫秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0xB0;		//设置定时初值
	TH0 = 0x3C;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	EA = 1;
	ET0 = 1;
}

unsigned char count;
unsigned char flag1=1;
void Timer_Service() interrupt 1
{
	TL0 = 0xB0;		//设置定时初值
	TH0 = 0x3C;		//设置定时初值
	count++;
	if(count>20)
	{
		count=0;
		if(flag1==1)
		{
			flag1=2;
		}
		else if(flag1==2)
		{
			flag1=1;
		}
	}
}



void Check()
{
	unsigned int i;
	if(temp>250)
	{
		if(flag1==1)
		{
			XBYTE[0X8000] = 0X55;
		}
		if(flag1==2)
		{
			XBYTE[0X8000] = 0XFF;
		}
	}
}
unsigned int dat_rd;
void Read_Ain3()
{
	IIC_Start(); 
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(0x43);
	IIC_WaitAck();
	IIC_Stop(); 
	
	IIC_Start(); 
	IIC_SendByte(0x91);
	IIC_WaitAck();
	dat_rd=IIC_RecByte()*2;
	IIC_WaitAck();
	IIC_SendAck(0);
	IIC_Stop();
}
void RD3_Display()
{	
	XBYTE[0XC000] = 0x1f;
	XBYTE[0XE000] = 0xff;
	Delay(200);
	Display_NixieBit(5,0xc1);
	Delay(200);
	Display_NixieBit(6,NixieTableDot[dat_rd/100]);
	Delay(200);
	Display_NixieBit(7,NixieTableNoDot[dat_rd/10%10]);
	Delay(200);
	XBYTE[0XC000] = 0xff;
	XBYTE[0XE000] = 0xff;
}
unsigned char flag=1;
void Key_Scan()
{
	if(S4==0)
	{
		  Delay(20);
			if(S4==0)
			{
				if(flag==1)
				{
					flag=2;
					while(S4==0)
					{
						DS18B20_Read();
						Nixie_Run();
					}
				}
				else if(flag==2)
				{
					flag=1;
					while(S4==0)
					{
						Read_Ain3();
						RD3_Display();
					}
				}
			}
	}
}

void System_init()
{
	XBYTE[0X8000]=0XFF;
	XBYTE[0XA000]=0X00;
}

void main()
{
	System_init();
	Timer0Init();
	while(1)
	{
		Read_Ain3();
		DS18B20_Read();	
		Key_Scan();
		if(flag==1)
		{
			Nixie_Run();
		}
		if(flag==2)
		{
			RD3_Display();
		}
		Check();
	}
}