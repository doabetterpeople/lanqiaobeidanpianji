#include <STC15F2K60S2.H>
#include "iic.h"

sbit S4 = P3^3; 
sbit S5 = P3^2; 
sbit S6 = P3^1; 
sbit S7 = P3^0; 

sbit L1 = P0^0; 
sbit L2 = P0^1; 
sbit L3 = P0^2; 
sbit L4 = P0^3; 
sbit L5 = P0^4; 

unsigned int count,count_s;
unsigned int dat_ne555;
unsigned char code NixieTableNoDot[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
unsigned char code NixieTableDot[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};
void Delay(unsigned int t)
{
	while(t--);
}
void HC138_init(unsigned char channel)
{
	switch(channel)
	{
		case 4: P2 = (P2&0x1f)|0x80;break;
		case 5: P2 = (P2&0x1f)|0xa0;break;
		case 6: P2 = (P2&0x1f)|0xc0;break;
		case 7: P2 = (P2&0x1f)|0xe0;break;
	}
}

void Nixie_Display(unsigned char pose,unsigned char dat)
{
	HC138_init(6);
	P0 = 0x01 << pose;
	HC138_init(7);
	P0 = dat;
}
//=================频率测量======================
void Timer_init()
{
	TMOD = 0x16;
	
	TH0 = 0xff;
	TL0 = 0xff;

	TH1 = (65535-50000)/256;
	TL1 = (65535-50000)%256;

	
	ET1 = 1;
	ET0 = 1;
	
	EA = 1;
	
	TR1 = 1;
	TR0 = 1; 
}

void Timer0_Service() interrupt 1
{
	count++;
}

void Timer1_Service() interrupt 3
{	
	TH1 = (65535-50000)/256;
	TL1 = (65535-50000)%256;

	count_s++;
	if(count_s==20)
	{
		dat_ne555=count;
		count_s=0;
		count=0;
	}
}
void NE555_Display()
{
	Nixie_Display(0,0x8e);
	Delay(200);
	Nixie_Display(1,0xff);
	Delay(200);
	if(dat_ne555>99999)
	{
		Nixie_Display(2,NixieTableNoDot[dat_ne555/100000]);
		Delay(200);
	}
	if(dat_ne555>9999)
	{
		Nixie_Display(3,NixieTableNoDot[(dat_ne555/10000)%10]);
		Delay(200);
	}
	if(dat_ne555>999)
	{
		Nixie_Display(4,NixieTableNoDot[(dat_ne555/1000)%10]);
		Delay(200);
	}
	if(dat_ne555>99)
	{
		Nixie_Display(5,NixieTableNoDot[(dat_ne555/100)%10]);
		Delay(200);
	}
	if(dat_ne555>9)
	{
		Nixie_Display(6,NixieTableNoDot[(dat_ne555/10)%10]);
		Delay(200);
	}
	Nixie_Display(7,NixieTableNoDot[dat_ne555%10]);
	Delay(200);
	
	HC138_init(6);
	P0 = 0xff;
	HC138_init(7);
	P0 = 0xff;
}

//================电压测量界面=================================
int dat_adc;
float AIN3_Read()
{
	float temp;
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(0x43);
	IIC_WaitAck();
	IIC_Stop();
	
	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();
	temp=IIC_RecByte();
	IIC_WaitAck();
	IIC_Stop();
	return temp;
}
void DAC(unsigned int date)
{
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(0x40);
	IIC_WaitAck();
	
	IIC_SendByte(date);
	IIC_WaitAck();
	IIC_SendAck(0);
	IIC_Stop();
}
void Change()
{
	float x;
	x = AIN3_Read();
	dat_adc = (int)(x*1.961);
}
void ADC_Display()
{
	Change();
	Nixie_Display(0,0xc1);
	Delay(200);
	Nixie_Display(1,0xff);
	Delay(200);
	Nixie_Display(2,0xff);
	Delay(200);
	Nixie_Display(3,0xff);
	Delay(200);
	Nixie_Display(4,0xff);
	Delay(200);
	Nixie_Display(5,NixieTableDot[dat_adc/100]);
	Delay(200);
	Nixie_Display(6,NixieTableNoDot[dat_adc/10%10]);
	Delay(200);
	Nixie_Display(7,NixieTableNoDot[dat_adc%10]);
	Delay(200);
	
	HC138_init(6);
	P0 = 0xff;
	HC138_init(7);
	P0 = 0xff;
}
//=======================按键功能======================================
unsigned char stat_main=0;
unsigned char flag=1;
unsigned char stat_led=0;
unsigned char stat_nixie=0;
void Display_max()
{
	Change();
	if(flag==1)
	{
		DAC(102.4);
	}
	if(flag==2) 
	{
		DAC(dat_adc/1.961);
	}
	if(stat_main==0)
	{
		if(stat_nixie==0)
		{
			ADC_Display();
		}
		else 
		{
			HC138_init(6);
			P0 = 0xff;
			HC138_init(7);
			P0 = 0xff;
		}
	}
	else
	{
		if(stat_nixie==0)
		{
			NE555_Display();
		}
		else 
		{
			HC138_init(6);
			P0 = 0xff;
			HC138_init(7);
			P0 = 0xff;
		}
	}
	
	if(stat_led==0)
	{
		if(stat_main==0)
		{
			HC138_init(4);
			L1 = 0;
			L2 = 1;
		}
		else 
		{
			HC138_init(4);
			L1 = 1;
			L2 = 0;
		}
		if((dat_adc<150)||((dat_adc>=250)&&(dat_adc<350)))
		{
			HC138_init(4);
			L3 = 1;
		}
		else {HC138_init(4);L3 = 0;}
		if((dat_ne555<1000)||((dat_ne555>=5000)&&(dat_ne555<10000)))
		{
			HC138_init(4);
			L4 = 1;
		}
		else {HC138_init(4);L4 = 0;}
		if(flag==1)
		{
			HC138_init(4);
			L5 = 1;
		}
	  if(flag==2) 
		{
			HC138_init(4);
			L5 = 0;
		}
	}
	else {HC138_init(4);P0 = 0xff;}
	
}
void Key_Scan()
{
	if(S4==0)
	{
		Delay(20);
		if(S4==0)
		{
				stat_main=~stat_main;
				while(S4==0)
				{
					Display_max();
				}
		}
	}
	if(S5==0)
	{
		Delay(20);
		if(S5==0)
		{
			if(flag==1)
			{
				flag=2;
			}
			else if(flag==2)
			{
				flag=1;
			}
			//	stat_dac=~stat_dac;
				while(S5==0)
				{
					Display_max();
				}
		}
	}
	
	if(S6==0)
	{
		Delay(20);
		if(S6==0)
		{
			stat_led=~stat_led;
			while(S6==0)
			{
				Display_max();
			}
		}
	}
	if(S7==0)
	{
		Delay(20);
		if(S7==0)
		{
				stat_nixie=~stat_nixie;
				while(S7==0)
				{
					Display_max();
				}
		}
	}
}




//=====================================================================
void System_init()
{
	HC138_init(5);
	P0 = 0x00;
	HC138_init(4);
	P0 = 0xff;
}

void main()
{
	System_init();
	Timer_init();
	while(1)
	{
		Key_Scan();
		Display_max();
	}
}
