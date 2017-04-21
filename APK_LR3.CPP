#define ScreenWidth 80
#include <dos.h>
#include <iostream.h>

struct VIDEO
{
	unsigned char symbol;
	unsigned char attributes;
};

char RegAttr = 0x01;

void getRegData();
void printRegister(int value, int offset);
void printMessage(char* str, int numbSymb, int offset);
void init();
void cleanScreen(int offset);
void changeRegAttr();

//OlD IRQ0-15
void interrupt (*int08) (...);
void interrupt (*int09) (...);
void interrupt (*int0A) (...);
void interrupt (*int0B) (...);
void interrupt (*int0C) (...);
void interrupt (*int0D) (...);
void interrupt (*int0E) (...);
void interrupt (*int0F) (...);

void interrupt (*int70) (...);
void interrupt (*int71) (...);
void interrupt (*int72) (...);
void interrupt (*int73) (...);
void interrupt (*int74) (...);
void interrupt (*int75) (...);
void interrupt (*int76) (...);
void interrupt (*int77) (...);

//Определяем новые прерывания IRQ0-15
void interrupt newInt08 (...) { getRegData(); int08(); }
void interrupt newInt09 (...) { changeRegAttr(); getRegData(); int09(); }
void interrupt newInt0A (...) { getRegData(); int0A(); }
void interrupt newInt0B (...) { getRegData(); int0B(); }
void interrupt newInt0C (...) { getRegData(); int0C(); }
void interrupt newInt0D (...) { getRegData(); int0D(); }
void interrupt newInt0E (...) { getRegData(); int0E(); }
void interrupt newInt0F (...) { getRegData(); int0F(); }

void interrupt newInt70 (...) { getRegData(); int70(); }
void interrupt newInt71 (...) { getRegData(); int71(); }
void interrupt newInt72 (...) { getRegData(); int72(); }
void interrupt newInt73 (...) { getRegData(); int73(); }
void interrupt newInt74 (...) { getRegData(); int74(); }
void interrupt newInt75 (...) { getRegData(); int75(); }
void interrupt newInt76 (...) { getRegData(); int76(); }
void interrupt newInt77 (...) { getRegData(); int77(); }

int main()
{
	unsigned far *fp;
	init();
	
	FP_SEG (fp) = _psp;
	FP_OFF (fp) = 0x2c;

	_dos_freemem(*fp);

	_dos_keep(0, (_DS -_CS) + (_SP/16) + 1);
	return 0;
}

void getRegData()
{
	int numberOfString = 1;
	cleanScreen(ScreenWidth * 13 - 1);
	printMessage("Master registers", 16, ScreenWidth * numberOfString++ + 1);
	printMessage("Masks", 5, ScreenWidth * numberOfString + 6);
	printRegister(inp(0x21), ScreenWidth * numberOfString++ + 28);

	printMessage("Requests", 8, ScreenWidth * numberOfString + 6);
	outp(0x20, 0x0A);
	printRegister(inp(0x20), ScreenWidth * numberOfString++ + 28);

	printMessage("Services", 8, ScreenWidth * numberOfString + 6);
	outp(0x20, 0x0B);
	printRegister(inp(0x20), ScreenWidth * numberOfString++ + 28);
	numberOfString++;
	printMessage("Slave registers", 15, ScreenWidth *  numberOfString++ + 1);
	printMessage("Masks", 5, ScreenWidth * numberOfString + 6);
	printRegister(inp(0xA1), ScreenWidth * numberOfString++ + 28);

	printMessage("Requests", 8, ScreenWidth * numberOfString + 6);
	outp(0xA0, 0x0A);
	printRegister(inp(0xA0), ScreenWidth * numberOfString++ + 28);

	printMessage("Services", 8, ScreenWidth * numberOfString + 6);
	outp(0xA0, 0x0B);
	printRegister(inp(0xA0), ScreenWidth * numberOfString + 28);
}

void printRegister(int value, int offset)
{
	VIDEO far* videobuffer = (VIDEO far*)MK_FP(0xB800, 0);
	videobuffer += offset;
	for( int i = 0; i < 8; i++)
	{
		videobuffer->symbol = value % 2 + '0';
		value /= 2;
		videobuffer->attributes = RegAttr;
		videobuffer++;
	}
}

void printMessage(char* str, int numbSymb, int offset)
{
	VIDEO far* videobuffer = (VIDEO far*)MK_FP(0xB800, 0);
	videobuffer += offset;

	for( int i = 0; i < numbSymb ; i++)
	{
		videobuffer->symbol = *(str + i);
		videobuffer->attributes = 0x03;
		videobuffer++;
	}
}

void init()
{
	int08 = getvect(0x08);
	int09 = getvect(0x09);
	int0A = getvect(0x0A);
	int0B = getvect(0x0B);
	int0C = getvect(0x0C);
	int0D = getvect(0x0D);
	int0E = getvect(0x0E);
	int0F = getvect(0x0F);
	
	int70 = getvect(0x70);
	int71 = getvect(0x71);
	int72 = getvect(0x72);
	int73 = getvect(0x73);
	int74 = getvect(0x74);
	int75 = getvect(0x75);
	int76 = getvect(0x76);
	int77 = getvect(0x77);
	
	setvect(0x08, newInt08);
	setvect(0x09, newInt09);
	setvect(0x0A, newInt0A);
	setvect(0x0B, newInt0B);
	setvect(0x0C, newInt0C);
	setvect(0x0D, newInt0D);
	setvect(0x0E, newInt0E);
	setvect(0x0F, newInt0F);

	setvect(0x78, newInt70);
	setvect(0x79, newInt71);
	setvect(0x7A, newInt72);
	setvect(0x7B, newInt73);
	setvect(0x7C, newInt74);
	setvect(0x7D, newInt75);
	setvect(0x7E, newInt76);
	setvect(0x7F, newInt77);	

	_disable();

	/* MASTER */
	outp(0x20, 0x11);	//ICW1
	outp(0x21, 0x08);   //ICW2
	outp(0x21, 0x04);	//ICW3
	outp(0x21, 0x01);	//ICW4
	
	/*SLAVE*/
	outp(0xA0, 0x11);	
	outp(0xA1, 0x78);	
	outp(0xA1, 0x02);	
	outp(0xA1, 0x01);       

	_enable(); 
}

void cleanScreen(int offset)
{
	for( int i = 0; i <= offset; i++)
		printMessage(" ", 1, i);
}

void changeRegAttr()
{
	if( RegAttr < 0x07) RegAttr++;
	else RegAttr = 0x01; 
}

