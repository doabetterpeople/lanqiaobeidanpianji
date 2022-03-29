#include <STC15F2K60S2.H>

sbit a=P2^5;
sbit b=P2^6;
sbit c=P2^7;
void M74hc138(unsigned char channal)
{
	switch (channal)
	{
		case 4:
			a=0;b=0;c=1;break;
		case 5:
			a=1;b=0;c=1;break;
	}
}

void begin()
{
	M74hc138(4);
	P0=0xff;
	M74hc138(5);
	P0=0x00;
}
//============ 
void inituart ()
{
	TMOD=0x20;
	TH1=0xfd;
	TL1=0xfd;
	TR1 = 1;
	
	SCON=0x50;
	AUXR=0x00;
	
	EA=1;
	ES=1;
}

void Severuart()   interrupt  4
{
	if(RI==1)
	{
		
		
	}
}


void Sendbit( unsigned  char thing)
{
	SBUF = thing;
	while(TI==0);
	TI=0;

}

void sendthing(unsigned  char *str)
{
	while(*str !='\0')
	{
		Sendbit(*str ++);
	}
}
//=============
void  main()
{
	begin();
	
	inituart ();
	sendthing("welcome to  world \n");
	while(1)
	{
		
	}
}
