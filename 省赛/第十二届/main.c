#include <STC15F2K60S2.H>
#include "onewire.h"
#include "iic.h"


sbit R1 = P3^2;
sbit R2 = P3^3;
sbit C1 = P4^2;
sbit C2 = P4^4;


unsigned char setting_num=25;
unsigned int dat_dac;
unsigned char display_num=1;
unsigned char mode=1;


unsigned char code NixieTable[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
														0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,
														0xbf,0x7f};
unsigned char code NixieTableDot[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};
void HC138_init(unsigned char channel)
{
	switch(channel)
	{
		case 4: P2 = (P2&0x1f) | 0x80;break;
		case 5: P2 = (P2&0x1f) | 0xa0;break;
		case 6: P2 = (P2&0x1f) | 0xc0;break;
		case 7: P2 = (P2&0x1f) | 0xe0;break;
	}
}
 
void Nixie_Display(unsigned char pose,unsigned char dat)
{
	HC138_init(6);
	P0 = 0x01 << pose;
	HC138_init(7);
	P0 = dat;
}
void Delay(unsigned int t)
{
	while(t--);
}	

float DS18B20_init()
{
	unsigned int temp;
	unsigned char MSB,LSB;
	init_ds18b20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0x44);
	Delay(1000);
	init_ds18b20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0xbe);
	
	LSB = Read_DS18B20();
	MSB = Read_DS18B20();
	
	temp = MSB << 8;
	temp = temp|LSB;
	return temp*0.0625;
}
unsigned int x;
void Temp_change()
{
	float wendu;
	wendu = DS18B20_init();
	x = (int)(wendu*100);
}
void Temp_Display()
{
	Nixie_Display(0,0xc6);
	Delay(200);
	Nixie_Display(1,0xff);
	Delay(200);
	Nixie_Display(2,0xff);
	Delay(200);
	Nixie_Display(3,0xff);
	Delay(200);
	Nixie_Display(4,NixieTable[x/1000]);
	Delay(200);
	Nixie_Display(5,NixieTableDot[x/100%10]);
	Delay(200);
	Nixie_Display(6,NixieTable[x/10%10]);
	Delay(200);
	Nixie_Display(7,NixieTable[x%10]);
	Delay(200);
	
	HC138_init(6);
	P0 = 0xff;
	HC138_init(7);
	P0 = 0xff;
}


void Setting_Display()
{
	Nixie_Display(0,0x8c);
	Delay(200);
	Nixie_Display(1,0xff);
	Delay(200);
	Nixie_Display(2,0xff);
	Delay(200);
	Nixie_Display(3,0xff);
	Delay(200);
	Nixie_Display(4,0xff);
	Delay(200);
	Nixie_Display(5,0xff);
	Delay(200);
	Nixie_Display(6,NixieTable[setting_num/10]);
	Delay(200);
	Nixie_Display(7,NixieTable[setting_num%10]);
	Delay(200);
	
	HC138_init(6);
	P0 = 0xff;
	HC138_init(7);
	P0 = 0xff;
}

void DAC(unsigned int dac)
{

	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(0x40);
	IIC_WaitAck();
	
	IIC_SendByte(dac);
	IIC_WaitAck();
	IIC_SendAck(0);
	IIC_Stop(); 
}

void DAC_Display(unsigned int dat)
{
	Nixie_Display(0,0x88);
	Delay(200);
	Nixie_Display(1,0xff);
	Delay(200);
	Nixie_Display(2,0xff);
	Delay(200);
	Nixie_Display(3,0xff);
	Delay(200);
	Nixie_Display(4,0xff);
	Delay(200);
	Nixie_Display(5,NixieTableDot[dat/100]);
	Delay(200);
	Nixie_Display(6,NixieTable[dat/10%10]);
	Delay(200);
	Nixie_Display(7,NixieTable[dat%10]);
	Delay(200);
	
	HC138_init(6);
	P0 = 0xff;
	HC138_init(7);
	P0 = 0xff;
}


void Display_max()
{
  Temp_change();
	if(display_num==1)
	{
		HC138_init(4);
		P0 = ~0x02;
		Temp_Display();
	}
	if(display_num==2)
	{
		HC138_init(4);
		P0 = ~0x04;
		Setting_Display();
	}
	if((display_num==3)&&(mode==1))
	{
		HC138_init(4);
		P0 = ~0x09;
		if(x<(setting_num*100))
		{
			DAC_Display(0);
			DAC(0);
		}
		if(x>=(setting_num*100))
		{
			DAC_Display(500);
			DAC(255);
		}			
	}
		if((display_num==3)&&(mode==2))
	{
		HC138_init(4);
		P0 = ~0x08;
		if(x<2000)
		{
			DAC_Display(100);
			DAC(51);
		}
		else if(x>4000)
		{
			DAC_Display(400);
			DAC(204);
		}			
		else if((x>=2000)&&(x<=4000))
		{
			DAC_Display(0.15*x-200);
			DAC((0.15*x-200)/1.961);
		}
	}
}

void Key_Scan()
{
	R1 = 0;
	R2=C1=C2=1;
	if((C1==0)&&(display_num==2))
	{
		Delay(50);
		if(C1==0)
		{
			setting_num++;
			while(C1==0)
			{
				Setting_Display();
			}
		} 
	}
	if(C2==0)
	{
		Delay(50);
		if(C2==0)
		{
			mode++;
			if(mode==3)
			{
				mode=1;
			}
			while(C2==0)
			{
				Display_max();
			}
		} 
	}
	R2 = 0;
	R1=C1=C2=1;
	if((C1==0)&&(display_num==2))
	{
		Delay(50);
		if(C1==0)
		{		
			setting_num--;	
			while(C1==0)
			{
				Setting_Display();
			}
		} 
	}
	if(C2==0)
	{
		Delay(50);
		if(C2==0)
		{	
			while(C2==0)
			{
				Display_max();
			}
			display_num++;
			if(display_num==4)
			{
				display_num=1;
			}

		} 
	}
}

void System_Init()
{	
	HC138_init(4);
	P0 = 0xff;
	HC138_init(5);
	P0 = 0x00;
}
void main()
{
	System_Init();
	while(1)
	{
		Key_Scan();
		Display_max();
	}
}