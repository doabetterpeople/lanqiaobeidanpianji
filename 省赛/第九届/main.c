#include <STC15F2K60S2.H>
#include "iic.h"

sbit S7 = P3^0;
sbit S6 = P3^1;
sbit S5 = P3^2;
sbit S4 = P3^3;

int dat_adc;
unsigned int count;
unsigned int time[4];
unsigned char led_mode=1;
unsigned char led_stat=0xff;
unsigned int stat;
unsigned char dat_pwm;
unsigned char temp_pwm;
unsigned char flag;
unsigned char nixie;
unsigned int nixie_800;
unsigned char key_set;
unsigned char led_level2;
unsigned char code NixieTable[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
														0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,
														0xbf,0x7f};
unsigned char code NixieTableDot[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};

void Display_mode();
void Display_time();
void HC138_init(unsigned char channel,unsigned char dat)
{
	P0 = dat;
	switch (channel)
	{
		case 4: P2 = (P2&0x1f)|0x80;break;
		case 5: P2 = (P2&0x1f)|0xa0;break;
		case 6: P2 = (P2&0x1f)|0xc0;break;
		case 7: P2 = (P2&0x1f)|0xe0;break;
		case 0: P2 = (P2&0x1f)|0x00;break;
	}
	P2 = (P2&0x1f)|0x00;
}
void Delay(unsigned int t)
{
	while(t--);
}
void Nixie_Display(unsigned char pose,unsigned char dat)
{
	HC138_init(6,0x01 << pose);
	HC138_init(7,dat);
}
float ADC_Read()
{
	float temp;
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(0x43);
	IIC_WaitAck();
	IIC_Stop();

	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();
	temp=IIC_RecByte();
	IIC_WaitAck();
	IIC_SendAck(1);
	IIC_Stop();
	return temp;
}
void change()
{
	float x;
	x = ADC_Read();
	dat_adc = (int)(x*1.961);
}
unsigned int AT24C02_read(unsigned char addr)
{
	unsigned int dat;
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(addr);
	IIC_WaitAck();
	
	IIC_Start();
	IIC_SendByte(0xa1);
	IIC_WaitAck();
	dat=IIC_RecByte();
	IIC_WaitAck();
	IIC_SendAck(1);
	IIC_Stop();
	return dat;
}
void AT24C02_Write(unsigned char addr,unsigned char dat)
{
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(addr);
	IIC_WaitAck();
	IIC_SendByte(dat);
	IIC_WaitAck();
	IIC_Stop();
}
void Timer0_init()		//1毫秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = (65535-1000)%256;		//设置定时初值
	TH0 = (65535-1000)/256;		//设置定时初值
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;
	EA  = 1;
}

void LED_run()
{
	switch(stat)
	{
		case 0: led_stat=0xff;break;
		case 1: led_stat=0xfe;break;
		case 2: led_stat=0xfd;break;
		case 3: led_stat=0xfb;break;
		case 4: led_stat=0xf7;break;
		case 5: led_stat=0xef;break;
		case 6: led_stat=0xdf;break;
		case 7: led_stat=0xbf;break;
		case 8: led_stat=0x7f;break;
		
		case 9:  led_stat=0x7f;break;
		case 10: led_stat=0xbf;break;
		case 11: led_stat=0xdf;break;
		case 12: led_stat=0xef;break;
		case 13: led_stat=0xf7;break;
		case 14: led_stat=0xfb;break;
		case 15: led_stat=0xfd;break;
		case 16: led_stat=0xfe;break;
		
		case 17: led_stat=0x7e;break;
		case 18: led_stat=0xbd;break;
		case 19: led_stat=0xdb;break;
		case 20: led_stat=0xe7;break;
		
		case 21: led_stat=0xe7;break;
		case 22: led_stat=0xdb;break;
		case 23: led_stat=0xbd;break;
		case 24: led_stat=0x7e;break;
	}
	if(stat==1)
	{
		led_mode  = 1;
	}
	else if(stat == 9)
	{
		led_mode  = 2;
	}
	else if(stat == 17)
	{
		led_mode  = 3;
	}
	else if(stat == 21)
	{
		led_mode  = 4;
	}
}
void LED_level()
{
	if(dat_adc<125)
	{
		dat_pwm=5;
		led_level2=1;
	}
	else if(dat_adc<250)
	{
		dat_pwm=10;
		led_level2=2;
	}
	else if(dat_adc<375)
	{
		dat_pwm=15;
		led_level2=3;
	}
	else if(dat_adc<=500)
	{
		dat_pwm=20;
		led_level2=4;
	}
}
void Timer0_Service() interrupt 1
{
	TL0 = (65535-1000)%256;		//设置定时初值
	TH0 = (65535-1000)/256;		//设置定时初值
	count++;
	temp_pwm++;
	if(temp_pwm<=dat_pwm)
	{
		HC138_init(4,led_stat);
	}
	else if(temp_pwm<20)
	{
		HC138_init(4,0xff);
	}
	else {HC138_init(4,led_stat);temp_pwm=0;LED_run();}
	if(count==time[led_mode-1])
	{		
		count=0;
		if(flag == 1)
			stat++;
		if(stat == 25)
			stat=0;
	}
	
	nixie_800++;
	if(nixie_800==800)
	{
		nixie_800=0;
		nixie=~nixie;
	}
}
void Nixie_off()
{
	HC138_init(6,0xff);
	HC138_init(7,0xff);
}

void level_Display()
{
	Nixie_Display(0,0xff);
	Delay(200);
	Nixie_Display(1,0xff);
	Delay(200);
	Nixie_Display(2,0xff);
	Delay(200);
	Nixie_Display(3,0xff);
	Delay(200);
	Nixie_Display(4,0xff);
	Delay(200);
	Nixie_Display(5,0xff);
	Delay(200);
	Nixie_Display(6,NixieTable[16]);
	Delay(200);
	Nixie_Display(7,NixieTable[led_level2]);
	Delay(200);
	HC138_init(6,0xff);
	HC138_init(7,0xff);
}
void save()
{
	switch(led_mode)
	{
		case 1: AT24C02_Write(0x01,time[0]/10);break; 
		case 2: AT24C02_Write(0x02,time[1]/10);break; 
		case 3: AT24C02_Write(0x03,time[2]/10);break; 
		case 4: AT24C02_Write(0x04,time[3]/10);break; 
	}
	Delay(1000);
	Nixie_off();
	key_set = 0;
	led_mode = 1;

}
void Display_max()
{
	switch(key_set)
	{
		case 1: Display_mode();break;
		case 2: Display_time();break;
		case 3: save();break;
	}
}
void Key_scan()
{
	if(S7 == 0)
	{
		Delay(50);
		if(S7 == 0)
		{
			if(flag == 0)
			{
				flag = 1;
			}
			else 
			{
				flag = 0;
				stat = 0;
				key_set = 0;
				led_mode=1;
			}
			while(S7 == 0);
		}
	}
	if(S6==0)
	{
		Delay(10);
		if(S6==0)
		{
			key_set++;
			if(key_set==1)
			{
				Display_mode();
			}
      else if(key_set==2)
			{
				Display_time();
			}
			while(S6 == 0){
				Display_max();
			}
		}
	}
	if(S5==0)
	{
		Delay(10);
		if(S5==0)
		{
			if(key_set==1)
			{
				led_mode++;
				if(led_mode==5)
				{
					led_mode = 1;
				}
				while(S5==0)
				{Display_mode();}
			}
			else if(key_set==2)
			{
				time[led_mode-1] = time[led_mode-1]+100;
				if(time[led_mode-1]>1200)
				{
					time[led_mode-1]=1200;
				}
				while(S5==0)
				{Display_time();}
			}
		}
	}
	if(S4==0)
	{
		Delay(10);
		if(S4==0)
		{
			if(key_set==1)
			{
				led_mode--;
				if(led_mode==0)
				{
					led_mode = 4;
				}
				while(S4==0)
				{Display_mode();}
			}
			else if(key_set==2)
			{
				time[led_mode-1] = time[led_mode-1]-100;
				if(time[led_mode-1]<400)
				{
					time[led_mode-1]=400;
				}
				while(S4==0)
				{Display_time();}
			}
			else if(key_set==0)
			{
				while(S4==0) 
				{
					level_Display();
					LED_run();
				}
			}
		}
	}
}


void Display_mode()
{
	if(nixie==0)
	{
		Nixie_Display(0,0xbf);
		Delay(200);
		Nixie_Display(1,NixieTable[led_mode]);
		Delay(200);
		Nixie_Display(2,0xbf);
		Delay(200);
	}
	else 
	{
		Nixie_Display(0,0xff);
		Delay(200);
		Nixie_Display(1,0xff);
		Delay(200);
		Nixie_Display(2,0xff);
		Delay(200);
	}
	if(time[led_mode-1]>=1000)
	{
		Nixie_Display(4,NixieTable[time[led_mode-1]/1000]);
		Delay(200);
	}

	Nixie_Display(5,NixieTable[time[led_mode-1]/100%10]);
	Delay(200);
	Nixie_Display(6,NixieTable[time[led_mode-1]/10%10]);
	Delay(200);
	Nixie_Display(7,NixieTable[time[led_mode-1]%10]);
	Delay(200);
	
	HC138_init(6,0xff);
	HC138_init(7,0xff);
	
}

void Display_time()
{
		Nixie_Display(0,0xbf);
		Delay(200);
		Nixie_Display(1,NixieTable[led_mode]);
		Delay(200);
		Nixie_Display(2,0xbf);
		Delay(200);
	if(nixie==0)
	{
		if(time[led_mode-1]>=1000)
		{
			Nixie_Display(4,NixieTable[time[led_mode-1]/1000]);
			Delay(200);
		}
		Nixie_Display(5,NixieTable[time[led_mode-1]/100%10]);
		Delay(200);
		Nixie_Display(6,NixieTable[time[led_mode-1]/10%10]);
		Delay(200);
		Nixie_Display(7,NixieTable[time[led_mode-1]%10]);
		Delay(200);
	}
	else 
	{
		Nixie_Display(4,0xff);
		Delay(200);
		Nixie_Display(5,0xff);
		Delay(200);
		Nixie_Display(6,0xff);
		Delay(200);
		Nixie_Display(7,0xff);
		Delay(200);
	}
	HC138_init(6,0xff);
	HC138_init(7,0xff);
}
void System_init()
{	
	HC138_init(4,0xff);
	HC138_init(5,0x00);
}
void READ_TIME()
{
	unsigned char j;
	time[0]=AT24C02_read(0x01)*10;
	time[1]=AT24C02_read(0x02)*10;
	time[2]=AT24C02_read(0x03)*10;
	time[3]=AT24C02_read(0x04)*10;
	for(j=0;j<4;j++)
	{
		if((time[j]>1200)||(time[j]<400))
		{
			time[j]=400;
		}
	}
}
void main()
{
	System_init();
	READ_TIME();
	Timer0_init();
	while(1)
	{
		ADC_Read();
		change();
		LED_level();
		Key_scan();
		Display_max();
	}
}