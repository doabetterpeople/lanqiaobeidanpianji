#include <STC15F2K60S2.H>
#include "iic.h"
sbit R1 = P3^2;
sbit R2 = P3^3;
sbit C1 = P3^4;
sbit C2 = P3^5;
sbit L1 = P0^0;
sbit L2 = P0^1;
sbit L3 = P0^2;

int dat_adc;
unsigned char Display_num = 1;
float setting_num=0;
unsigned char num=0;
unsigned led_temp;
unsigned int count;
unsigned char wrong=0;
unsigned char code NixieTable[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
														0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,
														0xbf,0x7f};
unsigned char code NixieTableDot[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};



void Delay(unsigned int t)
{
	while(t--);
}

void HC138_init(unsigned char channel)
{
	switch(channel)
	{
		case 4: P2 = (P2&0x1f) | 0x80; break;
		case 5: P2 = (P2&0x1f) | 0xa0; break;
		case 6: P2 = (P2&0x1f) | 0xc0; break;
		case 7: P2 = (P2&0x1f) | 0xe0; break;
	}
}

void Nixie_Display(unsigned char pose,unsigned char dat)
{
	HC138_init(6);
	P0 = 0x01 <<pose;
	HC138_init(7);
	P0 = dat;
}

float ADC_read()
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
	temp = IIC_RecByte();
	IIC_WaitAck();
	IIC_SendAck(0);
	IIC_Stop();
	return temp;
}	

void ADC_change()
{
	float x;
	x = ADC_read();
	dat_adc = x*1.961;

}
void ADC_Display()
{
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
	Nixie_Display(6,NixieTable[dat_adc/10%10]);
	Delay(200);
	Nixie_Display(7,NixieTable[dat_adc%10]);
	Delay(200);
	
	HC138_init(6);
	P0 = 0xff;
	HC138_init(7);
	P0 = 0xff;
}

void Setting_Display()
{
	unsigned int gg;
	gg = setting_num*100;
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
	Nixie_Display(5,NixieTableDot[gg/100]);
	Delay(200);
	Nixie_Display(6,NixieTable[gg/10%10]);
	Delay(200);
	Nixie_Display(7,NixieTable[gg%10]);
	Delay(200);
	
	HC138_init(6);
	P0 = 0xff;
	HC138_init(7);
	P0 = 0xff;
}
void Num_Display()
{
	Nixie_Display(0,0xc8);
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
	Nixie_Display(6,NixieTable[num/10]);
	Delay(200);
	Nixie_Display(7,NixieTable[num%10]);
	Delay(200);
	
	HC138_init(6);
	P0 = 0xff;
	HC138_init(7);
	P0 = 0xff;
}
void AT24C02_Write(unsigned char date)
{
	float cc;
	cc = setting_num;
	IIC_Start();
	IIC_SendByte(0xA0);
	IIC_WaitAck();
	IIC_SendByte(date);
	IIC_WaitAck();
	IIC_SendByte(cc*10);
	IIC_WaitAck();
	IIC_Stop();
}
float AT24C02_Read(unsigned char date)
{
	unsigned int temp;
	IIC_Start();
	IIC_SendByte(0xA0);
	IIC_WaitAck();
	IIC_SendByte(date);
	IIC_WaitAck();
	
	IIC_Start();
	IIC_SendByte(0xA1);
	IIC_WaitAck();
	temp = IIC_RecByte();
	IIC_SendAck(0);
	IIC_Stop();
	return temp;
}
void Display_max()
{
	ADC_change();
	switch(Display_num)
	{
		case 1: 
			ADC_Display();
		break;
		case 2:
			Setting_Display();
		break;
		case 3:
			AT24C02_Write(0x00);
			Num_Display();
		break;
	}
}

unsigned char flag;

void Num_Run()
{
	unsigned int gg;
	gg = setting_num*100;
	 if(dat_adc>gg)
	 {
		flag=1;
		TR1=0;
		count=0;
		led_temp=2;
	 }
	 else if(dat_adc<gg)
	 {
		flag=2;
		TR0=1;
	 }
	 if(dat_adc==gg)
	 {
		if(flag==1)
		{
			num++;
			flag=2;
		}
	 }
}

void key_Scan()         //°´¼üÉ¨Ãè
{
	R1 = 0;
	C1 = C2 = R2 = 1; 
	if((C1==0)&&(Display_num==2))      //S17 
	{
		Delay(50);
		if(C1==0)
		{
			wrong=0;
			setting_num=setting_num-0.5;
			if(setting_num<0)
			{
				setting_num=5;
			}
			while(C1==0)
			{
				Display_max();
			}
		}
	}
	if(((C1==0)&&(Display_num==1))||(C1==0)&&(Display_num==3))
	{
		while(C1==0)
		{
			Display_max();
		}
		wrong++;
	}
	if((C2==0)&&(Display_num==3))  //S13
	{
		Delay(50);
		if(C2==0)
		{
			wrong=0;
			while(C2==0)
			{
				Display_max();
			}
			num=0;
		}
	}
	if(((C2==0)&&(Display_num==1))||((C2==0)&&(Display_num==2)))
	{
		while(C2==0)
		{
			Display_max();
		}
		wrong++;
	}
	R2 = 0;
	C1 = C2 = R1 = 1; 
	if((C1==0)&&(Display_num==2))   //S16
	{
		Delay(50);
		if(C1==0)
		{
			wrong=0;
			setting_num=setting_num+0.5;
			if(setting_num>5)
			{
				setting_num=0;
			}
			while(C1==0)
			{
				Display_max();
			}
		}
	}
	if(((C1==0)&&(Display_num==1))||(C1==0)&&(Display_num==3))
	{
		while(C1==0)
		{
			Display_max();
		}
		wrong++;
	}
	if(C2==0)   //S12
	{
		Delay(50);
		if(C2==0)
		{
			wrong=0;
			while(C2==0)
			{
				Display_max();
			}
			Display_num++;
			if(Display_num==4)
			{
				Display_num=1;
			}
		}
	}
}
void Timer0_init()
{
	TMOD = 0x01;
	TL0 = (65536-50000)%256;
	TH0 = (65536-50000)/256;
	
	ET0 = 1;
	EA = 1;	
}

void Timer0_Service() interrupt 1
{
	
	TL0 = (65536-50000)%256;
	TH0 = (65536-50000)/256;
	count++;
	if(count==100)
	{
		count=0;
		led_temp=1;
	}
}
void LED_scan()
{
	if(led_temp==1)
	{
	HC138_init(4);
	L1 = 0;
	}
	else if(led_temp==2)
	{
	HC138_init(4);
	L1 = 1;
	}
	if(num%2==1)
	{
	HC138_init(4);
	L2 = 0;
	}
	else if(num%2==0)
	{
	HC138_init(4);
	L2 = 1;
	}
	if(wrong>=3)
	{
		L3 = 0;
	}
	else {L3 = 1;}
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
	Timer0_init();
	setting_num = AT24C02_Read(0x00);
	setting_num /= 10;
	while(1)
	{
		key_Scan();
		Display_max();
		Num_Run();
		LED_scan();
	}
}