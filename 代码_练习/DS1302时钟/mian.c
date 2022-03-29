#include <STC15F2K60S2.H>
#include "ds1302.h"
#include "Nixie.h"

unsigned char DS1302_Write_adr[7]= {0x80,0x82,0x84,0x86,0x88,0x8a,0x8c};
unsigned char DS1302_Read_adr[7] = {0x81,0x83,0x85,0x87,0x89,0x8b,0x8d};
//2022年1月18日 星期二 23点59分45秒
//2022年2月13日 星期日 17点50分00秒
unsigned char Timer[7] ={0x00,0x50,0x17,0x13,0x02,0x07,0x22};

void DS1302_Config()
{
//	unsigned char i;
//	Write_Ds1302(0x8e,0x00);  //控制寄存器的第7位为写保护，必须为0才能进行写操作
//	for(i=0;i<7;i++)
//	{
//		Write_Ds1302(DS1302_Write_adr[i],Timer[i]);
//	}
//	Write_Ds1302(0x8e,0x80);
	unsigned char i;
	Write_Ds1302(0x8e,0x00);
	for(i=0;i<7;i++)
	{
		Write_Ds1302(DS1302_Write_adr[i],Timer[i]);
	}
	Write_Ds1302(0x8e,0x80);
}

void Read_Ds1302_Timer()
{
//	char i;
//	for(i=0;i<7;i++)
//	{
//		Timer[i]=Read_Ds1302(DS1302_Read_adr[i]);
//	}
	char i;
	for(i=0;i<7;i++)
	{
		Timer[i]=Read_Ds1302 ( DS1302_Read_adr[i] );
	}
}
void Display()
{
		Display_NixieBit(0,NixieTableNoDot[Timer[2]/16]);
		Delay_Nixie(200);
		Display_NixieBit(1,NixieTableNoDot[Timer[2]%16]);
		Delay_Nixie(200);	
		Display_NixieBit(2,0xbf);
		Delay_Nixie(200);		
	
		Display_NixieBit(3,NixieTableNoDot[Timer[1]/16]);
		Delay_Nixie(200);
		Display_NixieBit(4,NixieTableNoDot[Timer[1]%16]);
		Delay_Nixie(200);
		Display_NixieBit(5,0xbf);
		Delay_Nixie(200);	
	
		Display_NixieBit(6,NixieTableNoDot[Timer[0]/16]);
		Delay_Nixie(200);
		Display_NixieBit(7,NixieTableNoDot[Timer[0]%16]);
		Delay_Nixie(200);
	
		Display_All(0xff);
		Delay_Nixie(200);
}
void main()
{
	DS1302_Config();
	while(1)
	{
		Read_Ds1302_Timer();
		Display();
	}
}
