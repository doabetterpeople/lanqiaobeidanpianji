#include "reg52.h"

sbit DQ = P1^4;

//��������ʱ����
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

//ͨ����������DS18B20дһ���ֽ�
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

//��DS18B20��ȡһ���ֽ�
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

//DS18B20��ʼ��
bit init_ds18b20(void)
{
  	bit initflag = 0;
  	
  	DQ = 1;
  	Delay_OneWire(120);
  	DQ = 0;
  	Delay_OneWire(800); // ��ʱ����480us
  	DQ = 1;
  	Delay_OneWire(100);  // 14
  	initflag = DQ;     // initflag����1��ʼ��ʧ��
  	Delay_OneWire(50);
  
  	return initflag;
}

//void Delay(int num) //��ʱ����
//{
//    while(num--) ;
//}
//void Init_DS18B20(void)//��ʼ��ds18b20
//{
//    DQ = 1;    //DQ��λ
//    Delay(8);    //������ʱ
//    DQ = 0;    //��Ƭ����DQ����
//    Delay(80);   //��ȷ��ʱ ���� 480us
//    DQ = 1;    //��������
//    Delay(14);
//    Delay(20);
//}
//unsigned char ReadOneChar(void)//��һ���ֽ�
//{
//    unsigned char i=0;
//    unsigned char dat = 0;
//    for (i=8;i>0;i--)
//    {
//        DQ = 0; // �������ź�
//        dat>>=1;
//        DQ = 1; // �������ź�
//        if(DQ)
//        dat|=0x80;
//        Delay(4);
//    }
//    return(dat);
//}
//void WriteOneChar(unsigned char dat)//дһ���ֽ�
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
//unsigned int ReadTemperature(void)//��ȡ�¶�
//{
//    unsigned char a=0;
//    unsigned char b=0;
//    long unsigned int tvalue;//�¶�ֵ
//    Init_DS18B20();
//    WriteOneChar(0xCC); // ����������кŵĲ���
//    WriteOneChar(0x44); // �����¶�ת��     
//    Init_DS18B20();
//    WriteOneChar(0xCC); //����������кŵĲ���
//    WriteOneChar(0xBE); //��ȡ�¶ȼĴ���   
//    a=ReadOneChar();    //����8λ           
//    b=ReadOneChar();    //����8λ
//    tvalue=b;
//    tvalue<<=8;
//    tvalue=tvalue|a;
//    tvalue=tvalue*0.0625;  //��ȡ�����ݱ�������λ
//    return(tvalue);
//}
