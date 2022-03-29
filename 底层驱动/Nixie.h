#ifndef  __Nixie_H__
#define  __Nixie_H__

unsigned char code NixieTableNoDot[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
unsigned char code NixieTableDot[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};


void Delay_Nixie(unsigned int t);
void Display_NixieBit(unsigned char i,unsigned char dat);
void Display_All(unsigned char dat);


#endif