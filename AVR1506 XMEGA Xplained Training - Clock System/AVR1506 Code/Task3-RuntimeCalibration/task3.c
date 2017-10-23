/*************************************************************************
 *
 *
 *              Task3: Runtime calibration with DFLL
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
	LEDPORT.OUT = 0xa0;  // Set initial value

	unsigned int counter = 0;

	// Enable the 32KHz internal oscillator which is used as reference for the DFLL
	CLKSYS_Enable( OSC_RC32KEN_bm );

	while (1)
	{

		/*  Enable automatic calibration of the internal 2MHz oscillator.
		 *  We do not use the external crystal connected to the TOSC pins,
		 *  but the internal 32kHz oscillator as reference.
		 */
		if ( (SWITCHPORT.IN & 0x01) == 0 )
		{
			CLKSYS_AutoCalibration_Enable( OSC_RC2MCREF_bm, false );

		/*  Due to an errata on ATxmega128A1 we need to enable the 32MHz internal RC oscillator
		 *  and the Autocalibration for the 32MHz internal RC oscillator in order for the calibration
		 *  to work correctly on the 2 MHz internal RC oscillator*/

		 	CLKSYS_Enable( OSC_RC32MEN_bm );
			do {} while ( CLKSYS_IsReady( OSC_RC32MRDY_bm ) == 0 );
			CLKSYS_AutoCalibration_Enable( OSC_RC32MCREF_bm, false );

			LEDPORT.OUTCLR = 0x0f;
			LEDPORT.OUTSET = 0x01;
		}

		/*  Disable the automatic calibration of the internal 2MHz oscillator.
		 */
		if ( (SWITCHPORT.IN & 0x02) == 0 )
		{
			CLKSYS_AutoCalibration_Disable( DFLLRC2M );

			
			//Disable the auto calibration of the 32MHz internal RC oscillator and the 32MHz internal RC oscillator
			CLKSYS_AutoCalibration_Disable( DFLLRC32M );
			CLKSYS_Disable( OSC_RC32MEN_bm );

			LEDPORT.OUTCLR = 0x0f;
			LEDPORT.OUTSET = 0x02;
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
