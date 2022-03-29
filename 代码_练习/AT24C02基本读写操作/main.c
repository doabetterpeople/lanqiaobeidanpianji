#include <STC15F2K60S2.H>
#include "iic.h"
#include "absacc.h"
unsigned char code NixieTable[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
														0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,
														0xbf,0x7f};
/*主机首先发送起始信号，
接着发送设备地址和它想要读取的数据内存字节地址，执行一个伪写操作。
在24C02应答主机之后，主机重新发送起始信号和从设备地址，进行读操作。
24C02响应并发送应答信号，然后输出所要求的一个8位字节数据。
主机接收完这个8位数据后，产生一个“非应答”信号，最后发送停止条件，字节读操作结束。*/


/*在IIC总线协议中，设备地址是起始信号后第一个发送的字节。
如果硬件地址引脚A0、A1、A2均接地，
那么，24C02的设备的读操作地址为：0xA1；而写操作地址则为：0xA0。*/
void Delay(unsigned int t)
{
	while(t--);
}
unsigned char IIC_Read(unsigned char addr)
{
	unsigned char temp;
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck(); 
	IIC_SendByte(addr);
	IIC_WaitAck();
	IIC_Start();
	IIC_SendByte(0xa1);
	IIC_WaitAck();
	temp = IIC_RecByte(); 
	IIC_SendAck(0);
	IIC_Stop();
	return temp;
}

/*4C02接收完设备地址后，产生应答信号；
然后接收8位内存字节地址，产生应答信号，接着接收一个8位数据，产生应答信号；
最后主机发送停止信号，字节写操作结束。*/

void IIC_Write(unsigned char addr,unsigned char dat)
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
unsigned char temp1,temp3,temp5;
void IIC_Working()
{
	temp1=IIC_Read(0x01)+1;	
	temp3=IIC_Read(0x03)+2;
	temp5=IIC_Read(0x05)+3;
	
	if(temp1>10)
	{temp1=0;}
	if(temp3>20)
	{temp3=0;}
	if(temp5>30)
	{temp5=0;}
	
	IIC_Write(0x01, temp1);
	Delay(1000);
	IIC_Write(0x03, temp3);
	Delay(1000);
	IIC_Write(0x05, temp5);
	Delay(1000);
}

void Nixie_Display(unsigned char pose,unsigned char dat)
{
	XBYTE[0XC000] = 0x01 <<pose;
	XBYTE[0XE000] = dat ;
}

void Nixie_Run()
{
	Nixie_Display(0,NixieTable[temp1/10]);
	Delay(500);
	Nixie_Display(1,NixieTable[temp1%10]);
	Delay(500);
	Nixie_Display(2,0xbf);
	Delay(500);
	Nixie_Display(3,NixieTable[temp3/10]);
	Delay(500);
	Nixie_Display(4,NixieTable[temp3%10]);
	Delay(500);
	Nixie_Display(5,0xbf);
	Delay(500);
	Nixie_Display(6,NixieTable[temp5/10]);
	Delay(500);
	Nixie_Display(7,NixieTable[temp5&10]);
	Delay(500);
	
	XBYTE[0XA000] = 0xff;
	XBYTE[0XC000] = 0x00;
}
void main()
{
	IIC_Working();
	while(1)
	{	
		Nixie_Run();
	}
}
	


