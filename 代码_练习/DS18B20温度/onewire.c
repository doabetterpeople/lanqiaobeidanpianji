#include "reg52.h"

sbit DQ = P1^4;

//单总线延时函数
#ifndef STC12  
void Delay_OneWire(unsigned int t)  //STC89C52RC
{
	while(t--);
}
#else
void Delay_OneWire(unsigned int t)  //STC12C5260S2
{
	unsigned char i;
	while(t--){
		for(i=0;i<12;i++);
	}
}
#endif

//通过单总线向DS18B20写一个字节
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DQ = 0;
		DQ = dat&0x01;
		Delay_OneWire(50);
		DQ = 1;
		dat >>= 1;
	}
	Delay_OneWire(50);
}

//从DS18B20读取一个字节
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat;
  
	for(i=0;i<8;i++)
	{
		DQ = 0;
		dat >>= 1;
		DQ = 1;
		if(DQ)
		{
			dat |= 0x80;
		}	    
		Delay_OneWire(50);
	}
	return dat;
}

//DS18B20初始化
bit init_ds18b20(void)
{
  	bit initflag = 0;
  	
  	DQ = 1;
  	Delay_OneWire(120);
  	DQ = 0;
  	Delay_OneWire(800); // 延时大于480us
  	DQ = 1;
  	Delay_OneWire(100);  // 14
  	initflag = DQ;     // initflag等于1初始化失败
  	Delay_OneWire(50);
  
  	return initflag;
}

//void Delay(int num) //延时函数
//{
//    while(num--) ;
//}
//void Init_DS18B20(void)//初始化ds18b20
//{
//    DQ = 1;    //DQ复位
//    Delay(8);    //稍做延时
//    DQ = 0;    //单片机将DQ拉低
//    Delay(80);   //精确延时 大于 480us
//    DQ = 1;    //拉高总线
//    Delay(14);
//    Delay(20);
//}
//unsigned char ReadOneChar(void)//读一个字节
//{
//    unsigned char i=0;
//    unsigned char dat = 0;
//    for (i=8;i>0;i--)
//    {
//        DQ = 0; // 给脉冲信号
//        dat>>=1;
//        DQ = 1; // 给脉冲信号
//        if(DQ)
//        dat|=0x80;
//        Delay(4);
//    }
//    return(dat);
//}
//void WriteOneChar(unsigned char dat)//写一个字节
//{
//    unsigned char i=0;
//    for (i=8; i>0; i--)
//    {
//        DQ = 0;
//        DQ = dat&0x01;
//        Delay(5);
//        DQ = 1;
//        dat>>=1;
//    }
//}
//unsigned int ReadTemperature(void)//读取温度
//{
//    unsigned char a=0;
//    unsigned char b=0;
//    long unsigned int tvalue;//温度值
//    Init_DS18B20();
//    WriteOneChar(0xCC); // 跳过读序号列号的操作
//    WriteOneChar(0x44); // 启动温度转换     
//    Init_DS18B20();
//    WriteOneChar(0xCC); //跳过读序号列号的操作
//    WriteOneChar(0xBE); //读取温度寄存器   
//    a=ReadOneChar();    //读低8位           
//    b=ReadOneChar();    //读高8位
//    tvalue=b;
//    tvalue<<=8;
//    tvalue=tvalue|a;
//    tvalue=tvalue*0.0625;  //读取的数据保留到个位
//    return(tvalue);
//}
