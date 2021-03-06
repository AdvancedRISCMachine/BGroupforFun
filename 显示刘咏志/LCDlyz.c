#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/ssi.h"
#include "math.h"
#include "uc1701.h"

//#define round(x) (x - floorf(x) >= 0.5 ? floorf(x) + 1 : floorf(x))
//
//float Round(float value, int n)
//{
//	return (float)round(value * pow(10.0,(float)n)) / pow(10.0,(float)n);
//}

char XDY[96];
char XDL[96];
char BPHD[128];
char DYPC[128];
char PL[64];

float Uphase=249.9,Iphase=0.599;
float fre=49.57;

//	UC1701Init(60000);
//	UC1701Clear();


void prifloat(unsigned char ucLine, unsigned char ucRow,float value, int n)
{
	int i=0,temp,intfloValue,intValue;
	float floValue;
	intValue=(int)floorf(value);
	temp=intValue;
	for(;temp!=0;i++)
		temp=temp/10;

	floValue=value-intValue;
	intfloValue=(int)roundf(floValue*pow(10,n));//自然实现四舍五入
	UC1701DisplayN(ucLine,ucRow,intValue);
	UC1701CharDispaly(ucLine,ucRow+i,".");
	UC1701DisplayN(ucLine,ucRow+i+1,intfloValue);
}
void screen11(void)
{
	UC1701Clear();
	UC1701CharDispaly(0, 1, "A");
	UC1701CharDispaly(1, 1, "B");
	UC1701CharDispaly(2, 1, "C");
	UC1701CharDispaly(3, 1, "U");


	int i=0;
	for(;i<=2;i++)
		UC1701ChineseDispaly(i, 2, 3,XDY);
	for(i=0;i<=2;i++)
		UC1701CharDispaly(i, 8, ":102.5V");
	UC1701ChineseDispaly(3, 2, 4,BPHD);
	UC1701CharDispaly(3, 10, ":35");//
}
void screen12(void)
{
	UC1701Clear();
	UC1701CharDispaly(0, 1, "A");
	UC1701CharDispaly(1, 1, "B");
	UC1701CharDispaly(2, 1, "C");
	UC1701CharDispaly(3, 1, "I");

	int i=0;
	for(;i<=2;i++)
		UC1701ChineseDispaly(i, 2, 3,XDL);
	for(i=0;i<=2;i++)
		UC1701CharDispaly(i, 8, ":102.5A");
	UC1701ChineseDispaly(3, 2, 4,BPHD);
	UC1701CharDispaly(3, 10, ":35");
}
void screen21(void)
{
	UC1701Clear();
	UC1701CharDispaly(0, 0, "A");
	UC1701CharDispaly(1, 0, "B");
	UC1701CharDispaly(2, 0, "C");


	int i=0;
	for(;i<=2;i++)
	{
		UC1701ChineseDispaly(i, 1, 3,XDY);
		UC1701CharDispaly(i, 7, "THD");
	}
	for(i=0;i<=2;i++)
		UC1701CharDispaly(i, 10, ":99.9%");
	UC1701ChineseDispaly(3, 0, 4,DYPC);
	UC1701CharDispaly(3, 8, ":99.9%");//
}
void screen22(void)
{
	UC1701Clear();
	UC1701CharDispaly(0, 0, "A");
	UC1701CharDispaly(1, 0, "B");
	UC1701CharDispaly(2, 0, "C");


	int i=0;
	for(;i<=2;i++)
	{
		UC1701ChineseDispaly(i, 1, 3,XDL);
		UC1701CharDispaly(i, 7, "THD");
	}

	for(i=0;i<=2;i++)
		UC1701CharDispaly(i, 10, ":99.9%");
	UC1701ChineseDispaly(3, 0, 2,PL);
	UC1701CharDispaly(3, 4, ":");
	prifloat(3,5,fre,2);//50.01Hz
	UC1701CharDispaly(3, 10, "Hz");
}
char XDY[]={
		//		"相",
		0x10,0x10,0xD0,0xFF,0x90,0x10,0x00,0xFE,
		0x22,0x22,0x22,0x22,0x22,0xFF,0x02,0x00,
		0x04,0x03,0x00,0xFF,0x00,0x01,0x00,0xFF,
		0x42,0x42,0x42,0x42,0x42,0xFF,0x00,0x00,

		//		"电",
		0x00,0xF8,0x48,0x48,0x48,0x48,0xFF,0x48,
		0x48,0x48,0x48,0xFC,0x08,0x00,0x00,0x00,
		0x00,0x07,0x02,0x02,0x02,0x02,0x3F,0x42,
		0x42,0x42,0x42,0x47,0x40,0x70,0x00,0x00,

		//		"压",
		0x00,0x00,0xFE,0x02,0x82,0x82,0x82,0x82,
		0xFE,0x82,0x82,0x82,0xC3,0x82,0x00,0x00,
		0x40,0x30,0x0F,0x40,0x40,0x40,0x40,0x40,
		0x7F,0x40,0x42,0x44,0x4C,0x60,0x40,0x00
};
char BPHD[]= {
		//		"不",
		0x02,0x02,0x02,0x02,0x02,0x82,0x42,0xF2,
		0x0E,0x42,0x82,0x02,0x02,0x03,0x02,0x00,
		0x00,0x08,0x04,0x02,0x01,0x00,0x00,0xFF,
		0x00,0x00,0x00,0x01,0x03,0x06,0x00,0x00,

		//		"平",
		0x00,0x02,0x02,0x1A,0x62,0x02,0x02,0xFE,
		0x02,0x02,0x62,0x1A,0x03,0x82,0x00,0x00,
		0x01,0x01,0x01,0x01,0x01,0x01,0x01,0xFF,
		0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,

		//		"衡",
		0x10,0x88,0xC4,0x33,0xF8,0x54,0x53,0xFA,
		0x56,0x52,0xF0,0x44,0x44,0xC6,0x44,0x00,
		0x01,0x00,0xFF,0x08,0x49,0x29,0x19,0x0F,
		0x19,0x69,0x0D,0x48,0x80,0x7F,0x00,0x00,

		//		"度",
		0x00,0x00,0xFC,0x24,0x24,0x24,0xFC,0xA5,
		0xA6,0xA4,0xFC,0x24,0x34,0x26,0x04,0x00,
		0x40,0x20,0x9F,0x80,0x42,0x42,0x26,0x2A,
		0x12,0x2A,0x26,0x42,0x40,0xC0,0x40,0x00
};
char XDL[]={
		//		"相",
		0x10,0x10,0xD0,0xFF,0x90,0x10,0x00,0xFE,
		0x22,0x22,0x22,0x22,0x22,0xFF,0x02,0x00,
		0x04,0x03,0x00,0xFF,0x00,0x01,0x00,0xFF,
		0x42,0x42,0x42,0x42,0x42,0xFF,0x00,0x00,

		//		"电",
		0x00,0xF8,0x48,0x48,0x48,0x48,0xFF,0x48,
		0x48,0x48,0x48,0xFC,0x08,0x00,0x00,0x00,
		0x00,0x07,0x02,0x02,0x02,0x02,0x3F,0x42,
		0x42,0x42,0x42,0x47,0x40,0x70,0x00,0x00,

		//		"流",
		0x10,0x22,0x64,0x0C,0x80,0x44,0x44,0x64,
		0x55,0x4E,0x44,0x54,0x66,0xC4,0x00,0x00,
		0x04,0x04,0xFE,0x01,0x00,0x80,0x40,0x3F,
		0x00,0xFF,0x00,0x3F,0x40,0x40,0x70,0x00
};

char DYPC[]={
		//	"电",
		0x00,0xF8,0x48,0x48,0x48,0x48,0xFF,0x48,
		0x48,0x48,0x48,0xFC,0x08,0x00,0x00,0x00,
		0x00,0x07,0x02,0x02,0x02,0x02,0x3F,0x42,
		0x42,0x42,0x42,0x47,0x40,0x70,0x00,0x00,

		//	"压",
		0x00,0x00,0xFE,0x02,0x82,0x82,0x82,0x82,
		0xFE,0x82,0x82,0x82,0xC3,0x82,0x00,0x00,
		0x40,0x30,0x0F,0x40,0x40,0x40,0x40,0x40,
		0x7F,0x40,0x42,0x44,0x4C,0x60,0x40,0x00,

		//	"偏",
		0x40,0x20,0xF8,0x07,0x00,0xFC,0x24,0x24,
		0x25,0x26,0x24,0x24,0x24,0xBE,0x04,0x00,
		0x00,0x00,0xFF,0x20,0x1C,0x03,0xFF,0x09,
		0x7F,0x09,0x09,0x7F,0x89,0xFF,0x01,0x00,

		//	"差",
		0x00,0x04,0x24,0x24,0x25,0x26,0xA4,0x7C,
		0x24,0x26,0x25,0x34,0x26,0x84,0x00,0x00,
		0x41,0x21,0x11,0x89,0x85,0x8B,0x89,0x89,
		0xF9,0x89,0x8D,0x89,0xC1,0x81,0x01,0x00
};
char PL[]={
		//"频",
		0x40,0x7C,0x40,0xFF,0x48,0x6C,0x4A,0xF2,
		0x12,0x1A,0xD6,0x12,0x12,0xFB,0x12,0x00,
		0x08,0x87,0x40,0x2F,0x10,0x0F,0x80,0x8F,
		0x40,0x20,0x1F,0x20,0x40,0xCF,0x00,0x00,

		//"率",
		0x00,0x14,0xA4,0x44,0x04,0x24,0xB5,0x6E,
		0x24,0x94,0x04,0x44,0xA6,0x14,0x00,0x00,
		0x08,0x09,0x08,0x08,0x09,0x09,0x09,0xFF,
		0x09,0x09,0x0B,0x08,0x08,0x0D,0x08,0x00
};
