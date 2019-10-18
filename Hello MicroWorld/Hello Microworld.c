#define MY_CONFIG

#include <xlcd.h>
#include "pragmas.h"

void set_osc_p18f45k22_4MHz(void)
{

	OSCCON = 0x52;  //0 101 r r 10		// Sleep on slp cmd, HFINT 4MHz, INT OSC Blk
	OSCCON2 = 0x04; //0 0 x 0 0 1 r r	// PLL No, CLK from OSC, MF off, Sec OSC off, Pri OSC
	OSCTUNE = 0x00; // 1 0 000000		// PLL disabled, Default factory freq tuning

	while (OSCCONbits.HFIOFS != 1)
		; // wait for osc to become stable
}

void configPort(void)
{
	// Port A to LED. set to output
	ANSELA = 0x00;
	LATA = 0x00;
	TRISA = 0x00;

	// portB config digital output
	ANSELB = 0x00;
	LATB = 0x00;
	TRISB = 0xF8;

	// don't need portC so set it to input
	ANSELC = 0x00;
	LATC = 0x00;
	TRISC = 0xFF;

	// port D configuration Digital input
	ANSELD = 0x00;
	LATD = 0x00;
	TRISD = 0xFF;

	// don't need portE so set it to input
	ANSELE = 0x00;
	LATE = 0x00;
	TRISE = 0xFF;
}

void DelayFor18TCY(void)
{
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
}

void DelayPORXLCD(void)
{
	Delay1KTCYx(15); // Delay of 15ms
	// Cycles = (TimeDelay * Fosc) / 4
	// Cycles = (15ms * 16MHz) / 4
	// Cycles = 60,000
	return;
}

void DelayXLCD(void)
{
	Delay1KTCYx(5); // Delay of 5ms
	// Cycles = (TimeDelay * Fosc) / 4
	// Cycles = (5ms * 16MHz) / 4
	// Cycles = 20,000
	return;
}

void LCD_Clear(void)
{
	while (BusyXLCD())
		;
	WriteCmdXLCD(0x01);
}

void LCD_Move(unsigned char row, unsigned char column)
{
	char ddaddr = 40 * (row - 1) + column;
	while (BusyXLCD())
		;
	SetDDRamAddr(ddaddr);
}

void Delay_Seconds(unsigned char s)
{
	unsigned char i, j;
	for (j = 0; j < s; j++)
	{
		for (i = 0; i < 100; i++)
			Delay1KTCYx(40);
	}
}

void initializeLCD(void)
{
	OpenXLCD(FOUR_BIT & LINES_5X7);
	while (BusyXLCD())
		;
	WriteCmdXLCD(DON); // Turn display on
	while (BusyXLCD())
		;
	WriteCmdXLCD(0x06); // move cursor right
	while (BusyXLCD())
		;
	WriteCmdXLCD(BLINK_OFF); // turn display on without cursor
	while (BusyXLCD())
		;
}

void initializeSystem(void)
{
	set_osc_p18f45k22_4MHz(); // Set the processor speed
	configPort();
	initializeLCD();
}

void main(void)
{
	initializeSystem();

	putrsXLCD("Hello World!");

	while (1)
		;
}