#include <STC15F2K60S2.H>

unsigned char code NixieTable[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
														0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,
														0xbf,0x7f};
sbit R1 = P3^0;
sbit R2 = P3^1;
sbit R3 = P3^2;
sbit R4 = P3^3;

sbit C4 = P3^4; 
sbit C3 = P3^5;
sbit C2 = P4^2;
sbit C1 = P4^4;

void HC138_Init(unsigned char channel)
{
	switch(channel)
	{
		case 4:	P2 = (P2&0x1f) | 0x80; break;
		case 5:	P2 = (P2&0x1f) | 0xa0; break;
		case 6:	P2 = (P2&0x1f) | 0xc0; break;
		case 7:	P2 = (P2&0x1f) | 0xe0; break;
	}
}
void DisplayKeyNum(unsigned char value)
{
	HC138_Init(6);
	P0 = 0x01;
	HC138_Init(7);
	P0 = value;
}
void System_init()
{
	HC138_Init(5);
	P0 = 0x00;
	HC138_Init(4);
	P0 = 0xff;	
}

unsigned char KeyNum;
void Key_Scan()
{
	R1 = 0;
	R2=R3=R4=1;
	C1=C2=C3=C4=1;
	if(C1==0) {while(C1==0);KeyNum=0;DisplayKeyNum(NixieTable[KeyNum]);}
	if(C2==0) {while(C2==0);KeyNum=1;DisplayKeyNum(NixieTable[KeyNum]);}
	if(C3==0) {while(C3==0);KeyNum=2;DisplayKeyNum(NixieTable[KeyNum]);}
	if(C4==0) {while(C4==0);KeyNum=3;DisplayKeyNum(NixieTable[KeyNum]);}
	
	R2 = 0;
	R1=R3=R4=1;
	C1=C2=C3=C4=1;
	if(C1==0) {while(C1==0);KeyNum=4;DisplayKeyNum(NixieTable[KeyNum]);}
	if(C2==0) {while(C2==0);KeyNum=5;DisplayKeyNum(NixieTable[KeyNum]);}
	if(C3==0) {while(C3==0);KeyNum=6;DisplayKeyNum(NixieTable[KeyNum]);}
	if(C4==0) {while(C4==0);KeyNum=7;DisplayKeyNum(NixieTable[KeyNum]);}
	
	R3 = 0;
	R2=R1=R4=1;
	C1=C2=C3=C4=1;
	if(C1==0) {while(C1==0);KeyNum=8;DisplayKeyNum(NixieTable[KeyNum]);}
	if(C2==0) {while(C2==0);KeyNum=9;DisplayKeyNum(NixieTable[KeyNum]);}
	if(C3==0) {while(C3==0);KeyNum=10;DisplayKeyNum(NixieTable[KeyNum]);}
	if(C4==0) {while(C4==0);KeyNum=11;DisplayKeyNum(NixieTable[KeyNum]);}
	
	R4 = 0;
	R2=R3=R1=1;
	C1=C2=C3=C4=1;
	if(C1==0) {while(C1==0);KeyNum=12;DisplayKeyNum(NixieTable[KeyNum]);}
	if(C2==0) {while(C2==0);KeyNum=13;DisplayKeyNum(NixieTable[KeyNum]);}
	if(C3==0) {while(C3==0);KeyNum=14;DisplayKeyNum(NixieTable[KeyNum]);}
	if(C4==0) {while(C4==0);KeyNum=15;DisplayKeyNum(NixieTable[KeyNum]);}
}



void main()
{
	System_init();
	while(1)
	{
		Key_Scan();
	}
}