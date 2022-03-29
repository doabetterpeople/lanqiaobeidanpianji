#include <STC15F2K60S2.H>
#include "iic.h"
#include "absacc.h"
unsigned char code NixieTable[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
														0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,
														0xbf,0x7f};
/*�������ȷ�����ʼ�źţ�
���ŷ����豸��ַ������Ҫ��ȡ�������ڴ��ֽڵ�ַ��ִ��һ��αд������
��24C02Ӧ������֮���������·�����ʼ�źźʹ��豸��ַ�����ж�������
24C02��Ӧ������Ӧ���źţ�Ȼ�������Ҫ���һ��8λ�ֽ����ݡ�
�������������8λ���ݺ󣬲���һ������Ӧ���źţ������ֹͣ�������ֽڶ�����������*/


/*��IIC����Э���У��豸��ַ����ʼ�źź��һ�����͵��ֽڡ�
���Ӳ����ַ����A0��A1��A2���ӵأ�
��ô��24C02���豸�Ķ�������ַΪ��0xA1����д������ַ��Ϊ��0xA0��*/
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

/*4C02�������豸��ַ�󣬲���Ӧ���źţ�
Ȼ�����8λ�ڴ��ֽڵ�ַ������Ӧ���źţ����Ž���һ��8λ���ݣ�����Ӧ���źţ�
�����������ֹͣ�źţ��ֽ�д����������*/

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
	


