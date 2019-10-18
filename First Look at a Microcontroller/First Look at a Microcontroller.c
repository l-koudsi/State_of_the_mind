
/*
	File Name:	First Look at a Microcontroller.c
	Author:		Lynn Koudsi
	Date:		Oct 18, 2019	
	Modified:	None

	Description:  Cycle LED's based on pushbutton input
	
***********************************************************************************************/

// Preprocessor ===================================================================

#include "pragmas.h"
#include <delays.h>


// Constants ======================================================================

#define TRUE			1
#define	FALSE			0

#define LED_ON			1				
#define LED_OFF 		0				

#define PB1 PORTDbits.RD7
#define PB2 PORTDbits.RD6
#define PB3 PORTDbits.RD5
#define PB4 PORTDbits.RD4

#define PB PORTBbits.RB3

#define LEDRED LATBbits.LATB0
#define LEDBLUE LATBbits.LATB1
#define LEDGREEN LATBbits.LATB2
		
#define SAMPLERATE 10
								
// Global Variables ===============================================================
int resultArray[SAMPLERATE];
int avg;
char flag = 0;
char ledCounter = 0;

// Functions ======================================================================

/*** configPort: ******************************************************
Author:		LKoudsi
Date:		Oct 18, 2019		
Modified:	None
Desc:		Set the I/O pins to inputs and outputs.
Input: 		None
Returns:	None
**********************************************************************************/
void configPort(void)
{
	// potentiometer on port A0
	ANSELA = 0x01;
	LATA = 0x00;
	TRISA = 0xFF;
	
	// portB config digital output on ports B1, B2, B3
	ANSELB = 0x00;
	LATB = 0x00;
	//TRISB = 0xF1;
	TRISB = 0xF8;		// 1111 1000

	// Serial is on port C6(TX) and C7(RX)
	ANSELC = 0x00;
	LATC = 0x00;
	TRISC = 0xFF;
	
	// port D configuration Digital input
	ANSELD = 0x00;		// Port D digital
	LATD = 0x00;		// Port D initialize to 0
	TRISD = 0xFF;		// Port D Input
	
	// don't need portE so set it to input
	ANSELE = 0x00;
	LATE = 0x00;
	TRISE = 0xFF;
}

/*** set_osc_p18f45k22_4MHz: ******************************************************
Author:		LKoudsi
Date:		Oct 18, 2019		
Modified:	None
Desc:		Sets the internal Oscillator of the Pic 18F45K22 to 4MHz.
Input: 		None
Returns:	None
**********************************************************************************/
void set_osc_p18f45k22_4MHz(void)
{
	
	OSCCON = 0x52; 		//0 101 r r 10		// Sleep on slp cmd, HFINT 4MHz, INT OSC Blk
	OSCCON2 = 0x04; 	//0 0 x 0 0 1 r r	// PLL No, CLK from OSC, MF off, Sec OSC off, Pri OSC
	OSCTUNE = 0x00;		// 1 0 000000		// PLL disabled, Default factory freq tuning
	
	while (OSCCONbits.HFIOFS != 1); 	// wait for osc to become stable
}

/*** initializeSystem: ***********************************************************
Author:		LKoudsi
Date:		Oct 18, 2019	
Modified:	None
Desc:		Initalizes the Temperature Logger System.
Input: 		None
Returns:	None
**********************************************************************************/
void initializeSystem(void)
{
	set_osc_p18f45k22_4MHz();			// Set the processor speed
	configPort();
}	
// eo initializeSystem::**********************************************************

/*********************************************************************************
***** MAIN FUNCTION **************************************************************
**********************************************************************************/

void main(void)	
{
	initializeSystem();	
	
	Delay10KTCYx(5); // Wait 50 ms if OSC is set to 4MHz.
	
	while(1)
	{
		if(PB){		
			ledCounter++;

			if(ledCounter == 4) {
				ledCounter = 0;	
			}

			switch(ledCounter){
				case (0):
					LEDRED = 0;
					LEDBLUE = 0;
					LEDGREEN = 0;
					Delay10KTCYx( 15 ); // for debouncing
					break;
				case (1):
					LEDRED = 0;
					LEDBLUE = 0;
					LEDGREEN = 1;
					Delay10KTCYx( 15 ); // for debouncing
					break;
				case (2):
					LEDRED = 0;
					LEDBLUE = 1;
					LEDGREEN = 0;
					Delay10KTCYx( 15 ); // for debouncing
					break;
				case (3):
					LEDRED = 1;
					LEDBLUE = 0;
					LEDGREEN = 0;
					Delay10KTCYx( 15 ); // for debouncing
					break;
				default:
					break;
			}
		}
	}
}