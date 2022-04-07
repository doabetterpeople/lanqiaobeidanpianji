#ifndef __INIT_H_
#define __INIT_H_



void HC138_init(unsigned char channel,unsigned char dat);
void Nixie_Display();
void Timer0_Init();
void Delay(unsigned int t);
float DS18B20_Read();
void DS1302_Read();
void DS1302_Write();

#endif