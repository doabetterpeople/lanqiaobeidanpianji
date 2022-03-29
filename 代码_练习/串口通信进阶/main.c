#include <STC15F2K60S2.H>

void HC138_Init(unsigned char channel)
{
	switch(channel)
	{
		case 4: P2 = (P2&0x1f) | 0x80;break;
		case 5: P2 = (P2&0x1f) | 0xa0;break;
		case 6: P2 = (P2&0x1f) | 0xc0;break;
		case 7: P2 = (P2&0x1f) | 0xe0;break;
	}
}

void System_Init()
{
	HC138_Init(5);
	P0 = 0x00;
	HC138_Init(4);
	P0 = 0xff;
}
//=============´®¿Ú³õÊ¼»¯===========
void UART_Init()
{
	TMOD  = 0X20;
	TH1 = 0Xfd;
	TL1 = 0Xfd;	
	SCON = 0X50;
	AUXR &= 0X40;
	TR1 = 1;
	EA = 1;
	ES = 1;

}
unsigned char command = 0x00;
void UART_Service() interrupt 4
{
	while(RI==1)
	{
		command = SBUF;
		RI=0;
	}
}

void Send_Byte(unsigned char dat)
{
	SBUF = dat;
	while(TI==0);
	TI=0;
}

void Send_str(unsigned char *str)
{
	while(*str!='\0')
	{
		Send_Byte(*str++);
	}
}

void LED_Run()
{
	if(command!=0x00)
	switch(command&0Xf0)
	{
		case 0xa0: P0 = (P0|0x0f)&(~command|0xf0);command = 0x00;break;
		case 0xb0: P0 = (P0|0xf0)&((~command<<4)|0x0f);command = 0x00;break;
		case 0xc0: Send_str("The System is Runing...\r\n");command = 0x00;break;
	}
}
void main()
{
	System_Init();
	UART_Init();
	Send_str("Welcome to LQB System\r\n");
	while(1)
	{
		LED_Run();
	}
}