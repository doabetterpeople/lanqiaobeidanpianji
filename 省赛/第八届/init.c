#include <STC15F2K60S2.H>
#include "onewire.h"
#include "ds1302.h"

unsigned char Nixie[]={0,0,0,0,0,0,0,0};
unsigned char duanma[] = { 0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0x00,0x40};
unsigned char Write_DS1302_addr[] = {0x80,0x82,0x84,0x86,0x88,0x8A,0x8C,0x8E};
unsigned char Read_DS1302_addr[] =  {0x81,0x83,0x85,0x87,0x89,0x8B,0x8D,0x8F};

                       //秒 分 时 日 月 星期 年
unsigned char time[] = {0x50,0x59,0x23,0x05,0x04,0x02,0x22,};
unsigned char alarm[]={0,0,0,0,0,0,0,0};
void HC138_init(unsigned char channel,unsigned char dat)
{
	P0 = dat;
	switch(channel)

	{
		case 4: P2 = (P2&0x1f) | 0x80; break;
		case 5: P2 = (P2&0x1f) | 0xa0; break;
		case 6: P2 = (P2&0x1f) | 0xc0; break;
		case 7: P2 = (P2&0x1f) | 0xe0; break;
		case 0: P2 = (P2&0x1f) | 0x00; break;
	}
	P2 = P2&0x1f;
}

void Nixie_Display()
{
	static unsigned char i;
	HC138_init(6,0x00);

	HC138_init(6,0x01<<i);
	HC138_init(7,~duanma[Nixie[i]]);

	if(++i==8)
	{
		i=0;
	}

}

void Timer0_Init()		
{
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;	
	EA  = 1;
}



void Delay(unsigned int t)
{
	while(t--);
}

float DS18B20_Read()
{
	unsigned char MSB,LSB;
  float temp;
	init_ds18b20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0x44);

	init_ds18b20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0xbe);

	LSB = Read_DS18B20();
	MSB = Read_DS18B20();
	
	temp = MSB<<8|LSB;
	return temp*0.0625;
}
void DS1302_Write()
{
	unsigned char i;
	Write_Ds1302(0x8e,0x00);
	for(i=0;i<7;i++)
	{
		Write_Ds1302(Write_DS1302_addr[i],time[i]);
	}
	Write_Ds1302(0x8e,0x80);
}

void DS1302_Read()
{
	char i;
	for(i=0;i<7;i++)
	{
		time[i] = Read_Ds1302 (Read_DS1302_addr[i]);
	}
}