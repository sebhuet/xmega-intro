/*************************************************************************
 *
 *
 *              Task2: Phase Locked Loop (PLL)
 *
 *
 *************************************************************************/

// Include header files for GCC/IAR
#include "driver/avr_compiler.h"

// Include Clock system driver from application note AVR1003
#include "driver/clksys_driver.h"

#define SWITCHPORT PORTC
#define LEDPORT PORTD




int main(void)
{
	// Configure switches
	PORTCFG.MPCMASK = 0xff; // Configure several PINxCTRL registers at the same time
	SWITCHPORT.PIN0CTRL = PORT_INVEN_bm; // Invert input to make STK600 switches go high when pressed

	SWITCHPORT.DIRCLR = 0xff; // Set port as input

	// Configure LEDs
	PORTCFG.MPCMASK = 0xff; // Configure several PINxCTRL registers at the same time
	LEDPORT.PIN0CTRL = PORT_INVEN_bm; // Invert input to turn STK600 leds on when port value is 1

	LEDPORT.DIRSET = 0xff; 	// Set port as output
	LEDPORT.OUT = 0xa0;  // Set initial value

	unsigned int counter = 0;

	while (1)
	{

		/*  Change the clock source to the internal 2MHz. Disable PLL.
		 *  Configure PLL with the 2 MHz RC oscillator as source and
		 *  multiply by 15 to get 30 MHz PLL clock and enable it. Wait
		 *  for it to be stable and set prescaler C to divide by two
		 *  to set the CPU clock to 15 MHz
		 */
		if ( SWITCHPORT.IN & 0x01 )
		{
			CLKSYS_Main_ClockSource_Select( CLK_SCLKSEL_RC2M_gc );

			CLKSYS_Disable( OSC_PLLEN_bm );

			CLKSYS_PLL_Config( OSC_PLLSRC_RC2M_gc, 15 );
			CLKSYS_Enable( OSC_PLLEN_bm );
			CLKSYS_Prescalers_Config( CLK_PSADIV_1_gc, CLK_PSBCDIV_1_2_gc );
			do {} while ( CLKSYS_IsReady( OSC_PLLRDY_bm ) == 0 );
			CLKSYS_Main_ClockSource_Select( CLK_SCLKSEL_PLL_gc );

			LEDPORT.OUTCLR = 0x0f;
			LEDPORT.OUTSET = 0x01;
		}

		/*  Configure PLL with the 2 MHz RC oscillator as source and
		 *  multiply by 15 to get 30 MHz PLL clock and enable it. Wait
		 *  for it to be stable and set prescaler B and C to divide by two
		 *  to set the CPU clock to 7.5 MHz
		 */
		if ( SWITCHPORT.IN & 0x02 )
		{
			CLKSYS_Main_ClockSource_Select( CLK_SCLKSEL_RC2M_gc );

			CLKSYS_Disable( OSC_PLLEN_bm );

			CLKSYS_PLL_Config( OSC_PLLSRC_RC2M_gc, 15 );
			CLKSYS_Enable( OSC_PLLEN_bm );
			CLKSYS_Prescalers_Config( CLK_PSADIV_1_gc, CLK_PSBCDIV_2_2_gc );
			do {} while ( CLKSYS_IsReady( OSC_PLLRDY_bm ) == 0 );
			CLKSYS_Main_ClockSource_Select( CLK_SCLKSEL_PLL_gc );

			LEDPORT.OUTCLR = 0x0f;
			LEDPORT.OUTSET = 0x02;
		}


		/*  Configure PLL with the 2 MHz RC oscillator as source and
		 *  multiply by 3 to get 6 MHz PLL clock and enable it. Wait
		 *  for it to be stable and set prescaler C to divide by two
		 *  to set the CPU clock to 3 MHz
		 */
		if ( SWITCHPORT.IN & 0x04 )
		{
			CLKSYS_Main_ClockSource_Select( CLK_SCLKSEL_RC2M_gc );

			CLKSYS_Disable( OSC_PLLEN_bm );

			CLKSYS_PLL_Config( OSC_PLLSRC_RC2M_gc, 3 );
			CLKSYS_Enable( OSC_PLLEN_bm );
			CLKSYS_Prescalers_Config( CLK_PSADIV_1_gc, CLK_PSBCDIV_1_2_gc );
			do {} while ( CLKSYS_IsReady( OSC_PLLRDY_bm ) == 0 );
			CLKSYS_Main_ClockSource_Select( CLK_SCLKSEL_PLL_gc );

			LEDPORT.OUTCLR = 0x0f;
			LEDPORT.OUTSET = 0x04;
		}


		/*  Dynamically change prescaler:
		 *  Set prescaler A to divide by 2, prescaler B with no division,
		 *  and prescaler C to divide by 2. This will give the CPU a freqency
		 *  of 1/4th of the input freqency. Input freqency will depend on the
		 *  previous switch that has been pressed.
		 */
		if ( SWITCHPORT.IN & 0x08 )
		{
			CLKSYS_Prescalers_Config( CLK_PSADIV_2_gc, CLK_PSBCDIV_1_2_gc );
			LEDPORT.OUTSET = 0x08;
		}


		/*  
		 *  Add code here for generating 62 MHz to clkper4 and 15.5 MHz to the CPU
		 */
		if ( SWITCHPORT.IN & 0x10 )
		{
			CLKSYS_Main_ClockSource_Select( CLK_SCLKSEL_RC2M_gc );

			CLKSYS_Disable( OSC_PLLEN_bm );

			CLKSYS_PLL_Config( OSC_PLLSRC_RC2M_gc, 31 );
			CLKSYS_Enable( OSC_PLLEN_bm );
			CLKSYS_Prescalers_Config( CLK_PSADIV_1_gc, CLK_PSBCDIV_2_2_gc );
			do {} while ( CLKSYS_IsReady( OSC_PLLRDY_bm ) == 0 );
			CLKSYS_Main_ClockSource_Select( CLK_SCLKSEL_PLL_gc );
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

