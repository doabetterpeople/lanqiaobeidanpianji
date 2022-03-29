#include <STC15F2K60S2.H>

//==============´®¿Ú³õÊ¼»¯========================
void UART_Init()
{
	TMOD = 0x20;
	TH1 = 0xfd;
	TL1 = 0xfd;
	AUXR &=0X40; 
	TR1 = 1;
	SCON = 0x50;
	EA = 1;
	ES = 1;
}
void Send_Byte(unsigned char dat)
{
	SBUF = dat;
	while(TI==0);
	TI=0;
}
unsigned char urdat;
void UART_Service() interrupt 4
{
	if(RI==1)
	{
		RI=0;
		urdat=SBUF;
		Send_Byte(urdat+1);
	}
}
void main()
{
	UART_Init();
	Send_Byte(0x5A);
	Send_Byte(0xa5);
	while(1)
	{
		
	}
}