#include <STC15F2K60S2.H>
#include "init.h"
#include "ds1302.h"

sbit S7 = P3^0;
sbit S6 = P3^1;
sbit S5 = P3^2;
sbit S4 = P3^3;

sbit L1 = P0^0;

extern unsigned char Nixie[];
extern unsigned char duanma[];
extern unsigned char time[];
extern unsigned char alarm[];
extern unsigned char Write_DS1302_addr[];
extern unsigned char Read_DS1302_addr[];
unsigned int dat_tem;
unsigned char time_mode=0; //时钟模式
unsigned int count_ms; 
char f_open=0;
unsigned char alarm_mode;   //闹钟模式
unsigned char flag_temp;
unsigned int led_time,led_open,led_flag;
unsigned int count_x;
void Display_max();
void LED_run();
unsigned char anjian = 0; //闹钟提示界面下是否有按键被按下
void Timer0_Service() interrupt 1
{
	if(time_mode==0)
	{
		LED_run();
		if(flag_temp==0)
		{
			DS1302_Read();
		}
	}
	else 
	{
		unsigned char i;
		Write_Ds1302(0x8e,0x00);
		for(i=0;i<7;i++)
		{
			Write_Ds1302(Write_DS1302_addr[i],time[i]);
		}
		Write_Ds1302(0x8e,0x80);
	}

	count_ms++;
	Nixie_Display();
	if(count_ms>1000)
	{
		count_ms=0;
		if(f_open ==1)
		{f_open=0;}
		else if(f_open==0)
		{f_open =1;}
	}
	
	if((time[0]==alarm[0])&&(time[1]==alarm[1])&&(time[2]==alarm[2]))
	{
		led_flag =1;
	}
	if(count_x<25)
	{
		if(anjian==0)
		{
			led_time++;
			if(led_time==200)
			{
				led_time = 0;
				count_x++;
				if(led_open==1)
				{
					led_open = 0;
				}
				else if(led_open == 0)
				{
				led_open = 1;
				}
			}
		}
		else if(anjian==1)
		{
			led_flag =0;
		}
	}
	else if(count_x==25)
	{
		led_flag = 0;
		count_x =  0;
	}

}
void LED_run()
{
	if(led_flag==1)
	{
		if(led_open==1)
		{
			HC138_init(4,0xfe);
		}
		else if(led_open ==0)
		{
			HC138_init(4,0xff);
		}
	}
	else {HC138_init(4,0xff);}
			
}
void Change()
{
	dat_tem = (int)DS18B20_Read();
}
void Temp_Display()
{
	if(dat_tem!=0&&dat_tem!=85)
	{
		Nixie[0] = 16;
		Nixie[1] = 16;
		Nixie[2] = 16;
		Nixie[3] = 16;
		Nixie[4] = 16;
		Nixie[5] = dat_tem/10;
		Nixie[6] = dat_tem%10;
		Nixie[7] = 12;
	}
}

void time_Display()
{
	Nixie[0] = time[2]/16;
	Nixie[1] = time[2]%16;
	Nixie[2] = 17;
	Nixie[3] = time[1]/16;
	Nixie[4] = time[1]%16;
	Nixie[5] = 17;
	Nixie[6] = time[0]/16;
	Nixie[7] = time[0]%16;
}

void Time_h()
{
	if(f_open==1)
	{
		Nixie[0] = time[2]/16;
		Nixie[1] = time[2]%16;
	}
	else if (f_open==0)
	{
		Nixie[0] = 16;
		Nixie[1] = 16;
	}
	
	Nixie[2] = 17;
	Nixie[3] = time[1]/16;
	Nixie[4] = time[1]%16;
	Nixie[5] = 17;
	Nixie[6] = time[0]/16;
	Nixie[7] = time[0]%16;
}
void alarm_h()
{
	if(f_open==1)
	{
		Nixie[0] = alarm[2]/16;
		Nixie[1] = alarm[2]%16;
	}
	else if (f_open==0)
	{
		Nixie[0] = 16;
		Nixie[1] = 16;
	}
	
	Nixie[2] = 17;
	Nixie[3] = alarm[1]/16;
	Nixie[4] = alarm[1]%16;
	Nixie[5] = 17;
	Nixie[6] = alarm[0]/16;
	Nixie[7] = alarm[0]%16;
}
void Time_m()
{		
	Nixie[0] = time[2]/16;
	Nixie[1] = time[2]%16;
	if(f_open==1)
	{
	Nixie[3] = time[1]/16;
	Nixie[4] = time[1]%16;
	}
	else if (f_open==0)
	{
		Nixie[3] = 16;
		Nixie[4] = 16;
	}
	Nixie[2] = 17;
	Nixie[5] = 17;
	Nixie[6] = time[0]/16;
	Nixie[7] = time[0]%16;
}
void alarm_m()
{		
	Nixie[0] = alarm[2]/16;
	Nixie[1] = alarm[2]%16;
	if(f_open==1)
	{
	Nixie[3] = alarm[1]/16;
	Nixie[4] = alarm[1]%16;
	}
	else if (f_open==0)
	{
		Nixie[3] = 16;
		Nixie[4] = 16;
	}
	Nixie[2] = 17;
	Nixie[5] = 17;
	Nixie[6] = alarm[0]/16;
	Nixie[7] = alarm[0]%16;
}
void Time_s()
{		
	Nixie[0] = time[2]/16;
	Nixie[1] = time[2]%16;
	Nixie[2] = 17;
	Nixie[3] = time[1]/16;
	Nixie[4] = time[1]%16;
	Nixie[5] = 17;
	if(f_open==1)
	{
		Nixie[6] = time[0]/16;
		Nixie[7] = time[0]%16;
	}
	else if (f_open==0)
	{
		Nixie[6] = 16;
		Nixie[7] = 16;
	}
}
void alarm_s()
{		
	Nixie[0] = alarm[2]/16;
	Nixie[1] = alarm[2]%16;
	Nixie[2] = 17;
	Nixie[3] = alarm[1]/16;
	Nixie[4] = alarm[1]%16;
	Nixie[5] = 17;
	if(f_open==1)
	{
		Nixie[6] = alarm[0]/16;
		Nixie[7] = alarm[0]%16;
	}
	else if (f_open==0)
	{
		Nixie[6] = 16;
		Nixie[7] = 16;
	}
}

void Key_Scan()
{
	if(S7==0)
	{
		Delay(20);
		if(S7==0)
		{
			if(led_flag ==1)
			{
				anjian = 1;
			}
			time_mode++;
			time_mode %= 4;
			while(S7==0)
			{
				Display_max();
			}
				
		}
	}
	if(S6==0)
	{
		Delay(50);
		if(S6==0)
		{
			if(led_flag ==1)
			{
				anjian = 1;
			}
			alarm_mode++;
			alarm_mode %= 4;
			while(S6==0)
			{
				Display_max();
			}
		}
	}
	if(S5==0)
	{
		Delay(20);
		if(S5==0)
		{
			if(led_flag ==1)
			{
				anjian = 1;
			}
			if(time_mode==1)
			{
				if((time[2]/16*10+time[2]%16)<23)
				{
					if(time[2]==0x09)
					{
						time[2] = 0x10;
					}
					else if(time[2]==0x19)
					{
						time[2] = 0x20;
					}
					else 
					{
						time[2] = time[2]+0x01;
					}
				}
				else if(time[2]==0x23)
				{
					time[2] = 0x00;
				}
			}
			if(time_mode==2)
			{
				if((time[1]/16*10+time[1]%16)<=58)
				{
					if(time[1]==0x09)
					{
						time[1] = 0x10;
					}
					else if(time[1]==0x19)
					{
						time[1] = 0x20;
					}
					else if(time[1]==0x29)
					{
						time[1] = 0x30;
					}
					else if(time[1]==0x39)
					{
						time[1] = 0x40;
					}
					else if(time[1]==0x49)
					{
						time[1] = 0x50;
					}
					else 
					{
						time[1] = time[1]+0x01;
					}
				}
				else if(time[1]==0x59)
				{
					time[1] = 0x00;
				}
			}
			if(time_mode==3)
			{
				if((time[0]/16*10+time[0]%16)<=58)
				{
					if(time[0]==0x09)
					{
						time[0] = 0x10;
					}
					else if(time[0]==0x19)
					{
						time[0] = 0x20;
					}
					else if(time[0]==0x29)
					{
						time[0] = 0x30;
					}
					else if(time[0]==0x39)
					{
						time[0] = 0x40;
					}
					else if(time[0]==0x49)
					{
						time[0] = 0x50;
					}
					else 
					{
						time[0] = time[0]+0x01;
					}
				}
				else if(time[0]==0x59)
				{
					time[0] = 0x00;
				}
			}
			if(alarm_mode==1)
			{
				if((alarm[2]/16*10+alarm[2]%16)<23)
				{
					if(alarm[2]==0x09)
					{
						alarm[2] = 0x10;
					}
					else if(alarm[2]==0x19)
					{
						alarm[2] = 0x20;
					}
					else 
					{
						alarm[2] = alarm[2]+0x01;
					}
				}
				else if(alarm[2]==0x23)
				{
					alarm[2] = 0x00;
				}
			}
			if(alarm_mode==2)
			{
				if((alarm[1]/16*10+alarm[1]%16)<=58)
				{
					if(alarm[1]==0x09)
					{
						alarm[1] = 0x10;
					}
					else if(alarm[1]==0x19)
					{
						alarm[1] = 0x20;
					}
					else if(alarm[1]==0x29)
					{
						alarm[1] = 0x30;
					}
					else if(alarm[1]==0x39)
					{
						alarm[1] = 0x40;
					}
					else if(alarm[1]==0x49)
					{
						alarm[1] = 0x50;
					}
					else 
					{
						alarm[1] = alarm[1]+0x01;
					}
				}
				else if(alarm[1]==0x59)
				{
					alarm[1] = 0x00;
				}
			}
			if(alarm_mode==3)
			{
				if((alarm[0]/16*10+alarm[0]%16)<=58)
				{
					if(alarm[0]==0x09)
					{
						alarm[0] = 0x10;
					}
					else if(alarm[0]==0x19)
					{
						alarm[0] = 0x20;
					}
					else if(alarm[0]==0x29)
					{
						alarm[0] = 0x30;
					}
					else if(alarm[0]==0x39)
					{
						alarm[0] = 0x40;
					}
					else if(alarm[0]==0x49)
					{
						alarm[0] = 0x50;
					}
					else 
					{
						alarm[0] = alarm[0]+0x01;
					}
				}
				else if(alarm[0]==0x59)
				{
					alarm[0] = 0x00;
				}
			}
			while(S5==0)
			{
				Display_max();
			}
		}
	}
	if(S4==0)
	{
		Delay(50);
		if(S4==0)
		{
			if(led_flag ==1)
			{
				anjian = 1;
			}
			if(time_mode==1)
			{
				if((time[2]/16*10+time[2]%16)>=1)
				{
					if(time[2]==0x10)
					{
						time[2] = 0x09;
					}
					else if(time[2]==0x20)
					{
						time[2] = 0x19;
					}
					else 
					{
						time[2] = time[2]-0x01;
					}
				}
				else if(time[2]==0x00)
				{
					time[2] = 0x23;
				}
			}
			if(time_mode==2)
			{
				if((time[1]/16*10+time[1]%16)>=1)
				{
					if(time[1]==0x10)
					{
						time[1] = 0x09;
					}
					else if(time[1]==0x20)
					{
						time[1] = 0x19;
					}
					else if(time[1]==0x30)
					{
						time[1] = 0x29;
					}
					else if(time[1]==0x40)
					{
						time[1] = 0x39;
					}
					else if(time[1]==0x50)
					{
						time[1] = 0x49;
					}
					else 
					{
						time[1] = time[1]-0x01;
					}
				}
				else if(time[1]==0x00)
				{
					time[1] = 0x59;
				}
			}
			if(time_mode==3)
			{
				if((time[0]/16*10+time[0]%16)>=1)
				{
					if(time[0]==0x10)
					{
						time[0] = 0x09;
					}
					else if(time[0]==0x20)
					{
						time[0] = 0x19;
					}
					else if(time[0]==0x30)
					{
						time[0] = 0x29;
					}
					else if(time[0]==0x40)
					{
						time[0] = 0x39;
					}
					else if(time[0]==0x50)
					{
						time[0] = 0x49;
					}
					else 
					{
						time[0] = time[0]-0x01;
					}
				}
				else if(time[0]==0x00)
				{
					time[0] = 0x59;
				}
			}
			if(alarm_mode==1)
			{
				if((alarm[2]/16*10+alarm[2]%16)>=1)
				{
					if(alarm[2]==0x10)
					{
						alarm[2] = 0x09;
					}
					else if(alarm[2]==0x20)
					{
						alarm[2] = 0x19;
					}
					else 
					{
						alarm[2] = alarm[2]-0x01;
					}
				}
				else if(alarm[2]==0x00)
				{
					alarm[2] = 0x23;
				}
			}
			if(alarm_mode==2)
			{
				if((alarm[1]/16*10+alarm[1]%16)>=1)
				{
					if(alarm[1]==0x10)
					{
						alarm[1] = 0x09;
					}
					else if(alarm[1]==0x20)
					{
						alarm[1] = 0x19;
					}
					else if(alarm[1]==0x30)
					{
						alarm[1] = 0x29;
					}
					else if(alarm[1]==0x40)
					{
						alarm[1] = 0x39;
					}
					else if(alarm[1]==0x50)
					{
						alarm[1] = 0x49;
					}
					else 
					{
						alarm[1] = alarm[1]-0x01;
					}
				}
				else if(alarm[1]==0x00)
				{
					alarm[1] = 0x59;
				}
			}
			if(alarm_mode==3)
			{
				if((alarm[0]/16*10+alarm[0]%16)>=1)
				{
					if(alarm[0]==0x10)
					{
						alarm[0] = 0x09;
					}
					else if(alarm[0]==0x20)
					{
						alarm[0] = 0x19;
					}
					else if(alarm[0]==0x30)
					{
						alarm[0] = 0x29;
					}
					else if(alarm[0]==0x40)
					{
						alarm[0] = 0x39;
					}
					else if(alarm[0]==0x50)
					{
						alarm[0] = 0x49;
					}
					else 
					{
						alarm[0] = alarm[0]-0x01;
					}
				}
				else if(alarm[0]==0x00)
				{
					alarm[0] = 0x59;
				}
			}
			if(time_mode==0)
			{
				flag_temp = 1;
			}
			while(S4==0)
			{
				Change();
				Display_max();
			}
			flag_temp = 0;
		}
	}
}

void Display_max()
{
	if(alarm_mode==0)
	{
		switch(time_mode)
		{
			case 0: 
				if(flag_temp==1)
				{
					Temp_Display();
				}
				else{time_Display();}
			break;
			case 1: Time_h();break;
			case 2: Time_m();break;
			case 3: Time_s();break;
		}
	}
	else if(alarm_mode!=0)
	{
		switch(alarm_mode)
		{
			case 1: alarm_h();break;
			case 2: alarm_m();break;
			case 3: alarm_s();break;
		}
	}
			
}

void main()
{	
	HC138_init(5,0x00);
	HC138_init(4,0xff);
	DS1302_Write();
	Timer0_Init();

	while(1)
	{	
		Key_Scan();
		Display_max();
	}
}