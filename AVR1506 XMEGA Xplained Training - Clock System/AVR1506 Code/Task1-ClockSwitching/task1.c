/*************************************************************************
 *
 *
 *              Task1: Dynamic clock switching
 *
 *
 *************************************************************************/

// Include header files for GCC/IAR
#include "driver/avr_compiler.h"

// Include Clock system driver from application note AVR1003
#include "driver/clksys_driver.h"

#define SWITCHPORT PORTF
#define LEDPORT PORTE

int main(void)
{
	// Configure switches
	PORTCFG.MPCMASK = 0xff; // Configure several PINxCTRL registers at the same time
	SWITCHPORT.PIN0CTRL = (SWITCHPORT.PIN0CTRL & ~PORT_OPC_gm) | PORT_OPC_PULLUP_gc; //Enable pull-up to get a defined level on the switches

	SWITCHPORT.DIRCLR = 0xff; // Set port as input

	// Configure LEDs
	PORTCFG.MPCMASK = 0xff; // Configure several PINxCTRL registers at the same time
	LEDPORT.PIN0CTRL = PORT_INVEN_bm; // Invert input to turn the leds on when port output value is 1

	LEDPORT.DIRSET = 0xff; 	// Set port as output
	LEDPORT.OUT = 0xa1;  // Set initial value

	unsigned int counter = 0;

	while (1)
	{

		/*  Enable internal 2 MHz rc oscillator and wait until it's
		 *  stable. Set the 2 MHz rc oscillator as the main clock source.
		 *  Then disable other oscillators.
		 */
		if ( (SWITCHPORT.IN & 0x01) == 0 )
		{
			CLKSYS_Enable( OSC_RC2MEN_bm );
			do {} while ( CLKSYS_IsReady( OSC_RC2MRDY_bm ) == 0 );
			CLKSYS_Main_ClockSource_Select( CLK_SCLKSEL_RC2M_gc );
			CLKSYS_Disable( OSC_RC32MEN_bm | OSC_RC32KEN_bm );

			LEDPORT.OUTCLR = 0x0f;
			LEDPORT.OUTSET = 0x01;
		}

		/*  Enable internal 32 MHz ring oscillator and wait until it's
		 *  stable. Set the 32 MHz ring oscillator as the main clock source. 
		 *  Then disable the other oscillators.
		 */
		if ( (SWITCHPORT.IN & 0x02) == 0 )
		{
			CLKSYS_Enable( OSC_RC32MEN_bm );
			do {} while ( CLKSYS_IsReady( OSC_RC32MRDY_bm ) == 0 );
			CLKSYS_Main_ClockSource_Select( CLK_SCLKSEL_RC32M_gc );
			CLKSYS_Disable( OSC_RC2MEN_bm | OSC_RC32KEN_bm );


			LEDPORT.OUTCLR = 0x0f;
			LEDPORT.OUTSET = 0x02;
		}

		/*  Enable internal 32 kHz calibrated oscillator and check for
		 *  it to be stable. Set the 32 kHz oscillator as the main clock source. 
		 *  Then disable the other oscillators.
		 */
		if ( (SWITCHPORT.IN & 0x04) == 0 )
		{
			CLKSYS_Enable( OSC_RC32KEN_bm );
			do {} while ( CLKSYS_IsReady( OSC_RC32KRDY_bm ) == 0 );
			CLKSYS_Main_ClockSource_Select( CLK_SCLKSEL_RC32K_gc );
			CLKSYS_Disable( OSC_RC2MEN_bm | OSC_RC32MEN_bm );

			LEDPORT.OUTCLR = 0x0f;
			LEDPORT.OUTSET = 0x04;
		}

		// Blink LEDS after the loop has been run several times
		// This gives us visual feedback on the MCU speed
		counter++;
		if (counter > 20000 )
		{
			LEDPORT.OUTTGL = 0xe0;
			counter=0;
		}


	}
}
