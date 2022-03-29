#include <STC15F2K60S2.H>

sbit S4 = P3^3;
sbit S5 = P3^2;
unsigned char led_stat= 0xff;
unsigned char code NixieTable[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
														0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,
														0xbf,0x7f};

void Delay(unsigned int t)
{
	while(t--);
}

void HC138_Init(unsigned char channel)
{
	switch(channel)
	{
		case 4: P2 = (P2&0x1f) |0x80;break;
		case 5: P2 = (P2&0x1f) |0xa0;break;
		case 6: P2 = (P2&0x1f) |0xc0;break;
		case 7: P2 = (P2&0x1f) |0xe0;break;
	}
}

void System_Init()  //系统初始化
{
	HC138_Init(5);
	P0 = 0x00;
	HC138_Init(4);
	P0 = 0xff;
}

void Check()
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		HC138_Init(4);
		P0 = 0xfe <<i;
		Delay(60000);
		Delay(60000);
	}
	for(i=0;i<8;i++)
	{
		HC138_Init(4);
		P0 = ~(0xfe <<i);
		Delay(60000);
		Delay(60000);
	}
	for(i=0;i<8;i++)
	{
		HC138_Init(6);
		P0 = 0x01<<i;
		HC138_Init(7);
		P0 = 0x00;
		Delay(60000);
		Delay(60000);
		P0 = 0xff;
	}
}

//==========定时器初始化================
void TImer0_Init() //定时器0，8位
{
	TMOD = 0x01;
	TH0 = (65535-50000)/256;
	TL0 = (65535-50000)%256;
	
	ET0 = 1;
	EA = 1;
	TR0 = 1;  //开启定时器
}
unsigned char count,count_s,count_m,count_h;
void Timer0_Service() interrupt 1
{
	TH0 = (65535-50000)/256;
	TL0 = (65535-50000)%256;
	count++;
	if(count == 20)
	{
		count=0;
		count_s++;
		if(count_s==60)
		{
			count_s=0;
			count_m++;
			if(count_m==60)
			{
				count_h++;
				count_m=0;
				if(count_h==12)
				{
					count_h=0;
				}
			}
		}
	}
}
//==================================================
void Nixie_Display(unsigned char pose,unsigned char dat)
{
	HC138_Init(6);
	P0 = 0x01 << pose;
	HC138_Init(7);
	P0 = dat;
}
void Nixie_Run()
{
	Nixie_Display(7,NixieTable[count_s%10]);
	Delay(500);
	Nixie_Display(6,NixieTable[count_s/10]);
	Delay(500);
	Nixie_Display(5,NixieTable[16]);
	Delay(500);
	Nixie_Display(4,NixieTable[count_m%10]);
	Delay(500);
	Nixie_Display(3,NixieTable[count_m/10]);
	Delay(500);
	Nixie_Display(2,NixieTable[16]);
	Delay(500);
	Nixie_Display(1,NixieTable[count_h%10]);
	Delay(500);
	Nixie_Display(0,NixieTable[count_h/10]);
	Delay(500);
	HC138_Init(6);
	P0 = 0xff;
	HC138_Init(7);
	P0 = 0xff;
}

void Delay_key(unsigned int t)
{
	while(t--)
	{
		Nixie_Run();
	}
}
void Key_Scan()
{
	if(S4==0)
	{
		Delay(10);
		if(S4==0)
		{
			while(S4==0)
			{
				Nixie_Run();
			}
			HC138_Init(4);
			led_stat=(led_stat|0x40)& (~led_stat | 0xbf);
			P0 = led_stat;
			P2 = (P2 & 0x1f) | 0x00;
		}
	}
	if(S5==0)
	{
		Delay(10);
		if(S5==0)
		{
			while(S5==0)
			{
				Nixie_Run();
			}
			HC138_Init(4);
			led_stat=(led_stat|0x80)& (~led_stat | 0x7f);
			P0 = led_stat;
			P2 = (P2 & 0x1f) | 0x00;
		}
	}
}
//========串口初始化===========
unsigned char flag=0x00;
void UartInit(void)		//9600bps@11.0592MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR &= 0xBF;		//定时器1时钟为Fosc/12,即12T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0xE8;		//设定定时初值
	TH1 = 0xFF;		//设定定时初值
	TR1 = 1;		//启动定时器1
	EA = 1;
	ES = 1;
}

void UART_Service() interrupt 4
{
	while(RI==1)
	{
		flag = SBUF;
		RI=0;
	}
}

void Send_Byte(unsigned char dat)
{
	SBUF = dat;
	while(TI==0);
	TI=0;
}

void Working()
{
	if(flag!=0x00)
	{
		switch(flag&0xf0)
		{
			case 0xa0: 
				HC138_Init(4);
				led_stat = (led_stat|0x0f)&(~flag|0xf0);
				P0=led_stat;
				flag =0x00;
			break;
			case 0xb0:
				Send_Byte((count_h/10 << 4)|(count_h %10));
				Send_Byte((count_m/10 << 4)|(count_m %10));
				Send_Byte((count_s/10 << 4)|(count_s %10));
				flag=0x00;
			break;
		}
	}
}


void main()
{
	System_Init();
	Check();
	TImer0_Init();
	UartInit();
	while(1)
	{		
		Key_Scan();
		Nixie_Run();
		Working();
	}
}