#ifndef _ONEWIRE_H
#define _ONEWIRE_H

#define OW_SKIP_ROM 0xcc
#define DS18B20_CONVERT 0x44
#define DS18B20_READ 0xbe

//º¯ÊýÉùÃ÷
void Write_DS18B20(unsigned char dat);
unsigned char Read_DS18B20(void);
bit init_ds18b20(void);


#endif