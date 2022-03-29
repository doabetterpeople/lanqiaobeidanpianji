#include <STC15F2K60S2.H>
#include "absacc.h"
#include "onewire.h"
#include "Nixie.h"

unsigned char LSB,MSB;
unsigned char temp;

//void Temp_Show();
void Delay(unsigned int t)
{
	while(t--);
}


void DS18B20_temp()
{
	init_ds18b20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0x44);
	Delay(10000);
	Write_DS18B20(0xcc);
	Write_DS18B20(0xbe);
	
	LSB = Read_DS18B20();
	MSB = Read_DS18B20();

	init_ds18b20();
	temp = (MSB<<8)|LSB;
	if((temp&0xf800) == 0x0000)
	{
		temp = temp >>4;
		temp *= 10;
		temp = temp + (LSB&0x0f)*0.0625;
	}
	
	
	
}

//void Temp_Show()
//{
//	Display_NixieBit(7,NixieTableNoDot[temp%10]);
//	Delay_Nixie(200);
//	Display_NixieBit(6,NixieTableDot[(temp%100)/10]);
//	Delay_Nixie(200);
//	Display_NixieBit(5,NixieTableNoDot[temp/100]);
//	Delay_Nixie(200);
//	
//	Display_NixieBit(4,0xff);
//	Delay_Nixie(100);
//	Display_NixieBit(3,0xff);
//	Delay_Nixie(100);
//	Display_NixieBit(2,0xff);
//	Delay_Nixie(100);
//	Display_NixieBit(1,0xff);
//	Delay_Nixie(100);
//	Display_NixieBit(0,0xff);
//	Delay_Nixie(100);
//	
//	Display_All(0xff);
//}

void Check()
{
	if(temp>250)
	{
		P0^0 = 1;
		Delay(60000);
		P0^0 = 0;
		Delay(60000);
	}
}

void main()
{
	while(1)
	{
		DS18B20_temp();
//		Temp_Show();
	}
}