
/*
 * This demo shows to do some basic graphics including lines and bitmaps
 * We create a "slider" using some of the graphics primitives and the touchscreen function.
 *
 *
 */

#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_emc.h"
#include "pin_mux.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "eGFX.h"
#include "eGFX_Driver.h"
#include "FONT_5_7_1BPP.h"

#include "pin_mux.h"
#include "fsl_device_registers.h"
#include "fsl_i2c.h"
#include "fsl_i2s.h"
#include "fsl_wm8904.h"
#include "Audio.h"
#include "fsl_iocon.h"
#include "fsl_common.h"
#include "clock_config.h"
#include "fsl_power.h"

#include "fsl_i2c.h"
#include "fsl_ft5406.h"


//Sprites_16BPP_565.h & .c are autogenerated by the eGFX_Tools program in the eGFX folder.  There is a .bat that runs this program.  All you have to do is put .png files into the "Sprites" folder and run the GenSprites.bat file.
#include "Sprites_16BPP_565.h"

//Fonts are made with the eGFX_Fontgen.exe file that is eGFX Folder.

#include "OCR_A_Extended__20px__Bold__SingleBitPerPixelGridFit_1BPP.h"
#include "Consolas__26px__Regular__AntiAliasGridFit_1BPP.h"
#include "Magneto__26px__Regular__AntiAliasGridFit_16BPP_565.h" 

#include "math.h"

volatile uint32_t NextSampleOut = 0;

volatile uint32_t RequestBuffer = 0;
volatile uint32_t BufferIndex = 0;

#define BUFFER_SIZE		512

#define FFT_LEN 		BUFFER_SIZE

volatile float MicBuffer[BUFFER_SIZE];
float FFT_OutBuffer[BUFFER_SIZE];
float PowerSpectrum[BUFFER_SIZE/2];


/*
 * The DMIC is configured to interrupt when our FIFO
 * reaches a certain threshold.    In this case we
 * are processing every sample.
 *
 * We could also use DMA to record blocks of microphone data
 *
 *
 */void DMIC0_IRQHandler()
{
	int16_t NextSampleIn;

	/*
	 *	This is the IRQ handler for the DMIC.   When the DMIC FIFO gets to a
	 *	preprogrammed level,  and interrupt will be generate and we will end up here!
	 */

   /*
	   there are several possible interrupt source from the DMIC FIFO.
	     Check to see that it was the INT flag
   */
	if(DMIC0->CHANNEL[1].FIFO_STATUS & DMIC_CHANNEL_FIFO_STATUS_INT_MASK)
	{

		DMIC0->CHANNEL[1].FIFO_STATUS |= DMIC_CHANNEL_FIFO_STATUS_INT_MASK;


		NextSampleIn = (int16_t)(DMIC0->CHANNEL[1].FIFO_DATA);

		if(RequestBuffer)
		{
			MicBuffer[BufferIndex] = (float)((NextSampleIn))  /  ((float)(1<<15));
			BufferIndex++;

			if(BufferIndex == BUFFER_SIZE)
			{
				RequestBuffer = 0;
				BufferIndex = 0;
			}
		}
  }
}

int main(void)
{
	
  	/* USART0 clock */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* Setup the I/O pins*/
    BOARD_InitPins();
	
	/*
		Get the CPU running at 180MHz.
	*/
	
    BOARD_BootClockPLL180M();
  
    BOARD_InitDebugConsole();

	/*
		There is a bunch of (16MB) of flash hooked up to the LPC54608 via a special interface called "SPIFI".
		we need to make sure it is initialized.   This is where all of the bitmap "sprites" are stored.
		SPIFI is a flash interface that is serial but appears like normal flash memory in the address space of LPC54608
	*/
	BOARD_InitSPIFI_ExternalFlash();


	 /*
				
		IMPORTANT  

		The guts of this function is a bit different than the previous labs as the
		system clock frequency is now 180MHz.  we have to kick in additional dividers for the SDRAM
		to operate correctly.   You will need to the version in this lab if you are going to run at 180MHz.
		Just a warning if you are copying this project elsewhere.

	*/

		BOARD_InitSDRAM();

		/*
				
		IMPORTANT  

		The guts of this function is a bit different than the previous labs (labs 1 - 4) as the
		system clock frequency is now 180MHz.  we have to kick in additional dividers for the LCD.
		to operate correctly.   You will need to the version in this lab if you are going to run at 180MHz.
		Just a warning if you are copying this project elsewhere.

		*/

	  /*
	   *
	   * This initializes the graphics subsystem
	   */
	   eGFX_InitDriver();
	 
	   /*
	    * Start the Digital MIC.
	    *
	    *
	    */
	   Init_DMIC();

	   /*
	    * We get a COM port to the PC to do printf's
	    *
	    */
	   PRINTF("Demo 2 : Visualize the DMIC\r\n");

		while(1)
		{

			RequestBuffer = 1;

			while(RequestBuffer == 1)
			{

			}

				 //Clear the backbuffer
			eGFX_ImagePlane_16BPP_ClearToColor(&eGFX_BackBuffer,eGFX_RGB888_TO_RGB565(255,255,255));
			
			for(int i=0;i<eGFX_PHYSICAL_SCREEN_SIZE_X;i++)
			{

				float Point1 = (MicBuffer[i-1]*(eGFX_PHYSICAL_SCREEN_SIZE_Y/2));
				float Point2 = (MicBuffer[i]*(eGFX_PHYSICAL_SCREEN_SIZE_Y/2));




				eGFX_DrawLine(&eGFX_BackBuffer,                       //We always have to tell the graphics functions what we are drawing to
				     			i-1 ,	Point1+eGFX_PHYSICAL_SCREEN_SIZE_Y/2.0 - 1,       //X and Y coordinate of starting point.   Our Y value is the scaled value of the microphone sample + 1/2 of the screen height
								i   ,   Point2+eGFX_PHYSICAL_SCREEN_SIZE_Y/2.0 - 1, 			//X and Y coordinate of end point
								eGFX_RGB888_TO_RGB565(0,64,64)              /*The color.   The LCD uses 16-bit color is 5-6-5 form */
								);

				eGFX_DrawLine(&eGFX_BackBuffer,                       //We always have to tell the graphics functions what we are drawing to
				     			i-1 ,	Point1+eGFX_PHYSICAL_SCREEN_SIZE_Y/2.0,       //X and Y coordinate of starting point.   Our Y value is the scaled value of the microphone sample + 1/2 of the screen height
								i   ,   Point2+eGFX_PHYSICAL_SCREEN_SIZE_Y/2.0, 			//X and Y coordinate of end point
								eGFX_RGB888_TO_RGB565(0,32,32)              /*The color.   The LCD uses 16-bit color is 5-6-5 form */
								);
				eGFX_DrawLine(&eGFX_BackBuffer,                       //We always have to tell the graphics functions what we are drawing to
							     			i-1 ,	Point1+eGFX_PHYSICAL_SCREEN_SIZE_Y/2.0 + 1,       //X and Y coordinate of starting point.   Our Y value is the scaled value of the microphone sample + 1/2 of the screen height
											i   ,   Point2+eGFX_PHYSICAL_SCREEN_SIZE_Y/2.0 + 1, 			//X and Y coordinate of end point
											eGFX_RGB888_TO_RGB565(0,64,64)              /*The color.   The LCD uses 16-bit color is 5-6-5 form */
											);


			}

		     eGFX_printf_Colored(&eGFX_BackBuffer,175,
		    		 	 	 	 	 	 	      10,
												  &OCR_A_Extended__20px__Bold__SingleBitPerPixelGridFit_1BPP,
												   eGFX_RGB888_TO_RGB565(16,16,0),
												  "FIT2017 - Digital MIC Visualizer");
								

		     //Render the Logo
		     eGFX_DMA_16BPP_Blit(&eGFX_BackBuffer, 0, 0, &Sprite_16BPP_565_galileo);

			  //The last step is to dump the backbuffer to the screen.
			  eGFX_Dump(&eGFX_BackBuffer);
		}
}



