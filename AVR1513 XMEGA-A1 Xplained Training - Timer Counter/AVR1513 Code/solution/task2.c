// Setup for different compilers
#include "../avr_compiler.h"

// Include timer counter driver
#include "../tc_driver.h"

// The board.h header file defines which IO ports peripherals like
// Switches and LEDs are connected to. The header file is configured
// for use with XMEGA-A1 Xplained.
#include "../board.h"

/*This code use Timer/Counter C0: TCC0 */


int main( void )
{
  
	/* To toggle a LED, set pin 0  as output and high so the LED is default low. */

	LEDPORT.OUTSET = PIN0_bm; 
	LEDPORT.DIRSET = PIN0_bm;

	//Add code here that sets the timer compare value for channel A
	//ie. CCA register for the TCC0

    TC_SetCompareA( &TCC0, 0x0300);


	/* We need to start the counter with correct prescaling, 
	* and make it count to a specific TOP value, as we did in task1.
	* To do this we use the driver functions supplied in TC_driver.c, study the source
	* file to see how the timer is configured */

	TC_SetPeriod(&TCC0, 0xFFFF);
	TC0_ConfigClockSource(&TCC0, TC_CLKSEL_DIV64_gc); 


	/* This code turn on the LED on compare match, and turn the LED off on timer overflow */

	while(1)
	{

		if((TCC0.INTFLAGS & TC0_OVFIF_bm) != 0)
		{
			TCC0.INTFLAGS = TC0_OVFIF_bm;      // Clear the TC_OVFIF by writing a logical 1 to the flag
			LEDPORT.OUTSET = PIN0_bm;            // Turn off LED
		}
  
		if((TCC0.INTFLAGS & TC0_CCAIF_bm) !=0)
		{
			TCC0.INTFLAGS = TC0_CCAIF_bm;      //Clear the TC_CCAIF by writing a logical 1 to the flag
			TCC0.CCABUF += 0x1000;            //Update the CCABUF register with a new value to change the LED on time.
			LEDPORT.OUTCLR = PIN0_bm;            //Turn on LED
		}
	}
}
