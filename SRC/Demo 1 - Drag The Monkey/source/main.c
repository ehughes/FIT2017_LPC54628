/***
 *      ____                         _
 *     |  _ \  ___ _ __ ___   ___   / |
 *     | | | |/ _ \ '_ ` _ \ / _ \  | |
 *     | |_| |  __/ | | | | | (_) | | |
 *     |____/ \___|_| |_| |_|\___/  |_|          __  __             _
 *     |  _ \ _ __ __ _  __ _  | |_| |__   ___  |  \/  | ___  _ __ | | _____ _   _
 *     | | | | '__/ _` |/ _` | | __| '_ \ / _ \ | |\/| |/ _ \| '_ \| |/ / _ \ | | |
 *     | |_| | | | (_| | (_| | | |_| | | |  __/ | |  | | (_) | | | |   <  __/ |_| |
 *     |____/|_|  \__,_|\__, |  \__|_| |_|\___| |_|  |_|\___/|_| |_|_|\_\___|\__, |
 *                      |___/                                                |___/
 */

/*
This example adds a few items to lab 5 to show how to read the touch screen and render some graphics.

Some important highlights ( and changes since lab 5!!)

1.)    The CPU Runs at 180MHz.
2.)    The Scatter file has this added at the end:

	LR_EROM1 0x10000000 0x010000000  {    ; load region size_region
  ER_EROM1 0x10000000 0x010000000  {  ; load address = execution address
   *.o (ExtFlashSection)
  }
}
*/

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

/*
	This union will be used to split 32-bit fifo data into 2 int16_t samples.
	Unions are uses to overlay several variable across the same memory.
*/

typedef union
{
	uint32_t Data;
	int16_t Channel[2];
}I2S_FIFO_Data_t;


/*
 * Special Note!!  The routines below are the interrupt handlers
for Flexcomm 6 and 7.    We are directly processing the data in the interrupts.
I commented out the default handlers in fsl_flexcomm.c   The NXP I2s routines for
transmitting and recieving data are quite bulky.  We are going define our own interrupt handlers here.

*/

/***
 *      ___ ____  ____    _______  __  ___       _                             _
 *     |_ _|___ \/ ___|  |_   _\ \/ / |_ _|_ __ | |_ ___ _ __ _ __ _   _ _ __ | |_
 *      | |  __) \___ \    | |  \  /   | || '_ \| __/ _ \ '__| '__| | | | '_ \| __|
 *      | | / __/ ___) |   | |  /  \   | || | | | ||  __/ |  | |  | |_| | |_) | |_
 *     |___|_____|____/    |_| /_/\_\ |___|_| |_|\__\___|_|  |_|   \__,_| .__/ \__|
 *                                                                      |_|
 *
 *   We are using the I2S in a "per sample" mode.  Every time we need to generate a new sample, we get
 *   an interrupt
 *
 */

float	MonkeySliderValue = 0;
int     SampleIndex =0;

const int16_t SinTable[16] = { 0, 12246	, 22627	, 29564	, 32000	, 29564	, 22627	,  12246	, 0 ,
							   -12246, -22627, -29564, -32000	,  -29564	, -22627	, -12246 };

void FLEXCOMM6_DriverIRQHandler(void)
{
    if (I2S0->FIFOINTSTAT & I2S_FIFOINTSTAT_TXLVL_MASK)
    {
    	SampleIndex+=1;

    	if(SampleIndex>15)
    		SampleIndex =0;

		I2S_FIFO_Data_t Tmp;

		/*
		 * Generate a sample value in signed 16-bit format that is scaled by our virtual slider
		 *
		 */

		int16_t DAC_Value = (int16_t)(MonkeySliderValue  * (float)SinTable[SampleIndex]);

    	Tmp.Channel[0] = DAC_Value;  //16 points / wave from the table is 2Khz

    	Tmp.Channel[1] = DAC_Value;  //use the same sample for the left and right channel

        /* Tmp is a union that had our data in the "packed" (2 16 bit samples in a 32-bit word) format  */
		I2S0->FIFOWR = Tmp.Data;

	    /* Clear TX level interrupt flag */
        I2S0->FIFOSTAT = I2S_FIFOSTAT_TXLVL(1U);
		}
}

/***
 *      ___ ____  ____    ____  __  __  ___       _                             _
 *     |_ _|___ \/ ___|  |  _ \ \ \/ / |_ _|_ __ | |_ ___ _ __ _ __ _   _ _ __ | |_
 *      | |  __) \___ \  | |_) | \  /   | || '_ \| __/ _ \ '__| '__| | | | '_ \| __|
 *      | | / __/ ___) | |  _ <  /  \   | || | | | ||  __/ |  | |  | |_| | |_) | |_
 *     |___|_____|____/  |_| \_\/_/\_\ |___|_| |_|\__\___|_|  |_|   \__,_| .__/ \__|
 *
 *
 *   Everytime we get a new sample in, this routine gets called.
 *                                                                       |_|
 */

void FLEXCOMM7_DriverIRQHandler(void)
{
    register float LeftChannel;
	register float RightChannel;

	I2S_FIFO_Data_t FIFO_Data;

     /* Clear RX level interrupt flag */
     I2S1->FIFOSTAT = I2S_FIFOSTAT_RXLVL(1U);

	   /*
		  Read the Recieve FIFO.   Data is packed as two samples in one 32-bit word.
	    */

        FIFO_Data.Data = I2S1->FIFORD;


	  /*
			In the configuration for this lab,  2 channels of data are packed
			in one 32-bit word.  The Right Channel is in the upper 16-bits and the Left-Channel in the lower
		    Notice between we can use a "union" (I2S_FIFO_Data_t) to read the data in as 32-bit and access it as two 16-bit signed numbers.
	  */

	   LeftChannel = (float)(FIFO_Data.Channel[0])/32768.0f;
	   RightChannel = (float)(FIFO_Data.Channel[1])/32768.0f;

		/*
			Do something with the Left and Right channel here
		*/
}


/***
 *      __  __             _              
 *     |  \/  | ___  _ __ | | _____ _   _ 
 *     | |\/| |/ _ \| '_ \| |/ / _ \ | | |
 *     | |  | | (_) | | | |   <  __/ |_| |
 *     |_|__|_|\___/|_| |_|_|\_\___|\__, |
 *     / ___|| (_) __| | ___ _ __   |___/ 
 *     \___ \| | |/ _` |/ _ \ '__|        
 *      ___) | | | (_| |  __/ |           
 *     |____/|_|_|\__,_|\___|_|           
 *                                        
 */
 
//Here I am going to define some symbols control the "area" of the screen where the slider will be.
//Notice that the Sprite for the monkey is just a eGFX_ImagePlane struct and you get it the size of it
//Remember, sprites are autogenerated by the eGFX_Tools.exe program (which we run with the GenSprites.bat file)
 
#define SLIDER_REGION_START_X	 50
#define SLIDER_REGION_STOP_X	 (eGFX_PHYSICAL_SCREEN_SIZE_X - 50)
#define SLIDER_REGION_START_Y    150
#define SLIDER_REGION_STOP_Y	 SLIDER_REGION_START_Y+Sprite_16BPP_565_monkey.SizeY

int main(void)
{
	
		//Some temporary variables for reading the touch screen
  	int32_t x=10,y=10;
		
	 //We are going to make a variable that stores the coordinate of the slider. 
	 //I will initialize the value to somewhere in the middle
  
	int32_t MonkeySliderPosition = (SLIDER_REGION_STOP_X + SLIDER_REGION_START_X)/2;
	
	
	//some variables for the touch screen driver
    ft5406_handle_t touch_handle;
   
    touch_event_t touch_event;

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
	    * Start the Audio CODEC @ 32KHz Rate
	    * There are some macros in Audio.c that can be used to change the sample rate.
	    *
	    */
	   InitAudio_CODEC();

	   FT5406_Init(&touch_handle,I2C2);

	   /*
	    * We get a COM port to the PC to do printf's
	    *
	    */
	   PRINTF("Demo 1 : Drag the Monkey!\r\n");

		while(1)
		{

				 //Clear the backbuffer
					 eGFX_ImagePlane_16BPP_ClearToColor(&eGFX_BackBuffer,eGFX_RGB888_TO_RGB565(0,0,0));
			
				 eGFX_printf_HorizontalCentered_Colored(&eGFX_BackBuffer,
											10, 
											&OCR_A_Extended__20px__Bold__SingleBitPerPixelGridFit_1BPP,   
											eGFX_RGB888_TO_RGB565(255,255,0),
											"FIT2017 - Drag the Monkey %d , %d",x,y);
								
			
						 //read in the x,y coordinate of the the touch sensor if there is a good reading.
					if (kStatus_Success == FT5406_GetSingleTouch(&touch_handle, &touch_event, &y, &x))
					{
						if (touch_event == kTouch_Contact)
						{
											//Check to see if we are in the slider area
										  if(	 x>=SLIDER_REGION_START_X && x<=SLIDER_REGION_STOP_X
												&& y>SLIDER_REGION_START_Y && y<= SLIDER_REGION_STOP_Y)
											{
													//Store the Value
													MonkeySliderPosition = x;
											}
						}
					}
				
				//The variable MonkeySliderPosition is the X coordinate of the slider.  
				//Let's normalize this value to 0 to 1.0 so we can easily re-scale to something else later.
				
			  MonkeySliderValue =  (float)(MonkeySliderPosition - SLIDER_REGION_START_X)/
					  	  	  	    (float)(SLIDER_REGION_STOP_X-SLIDER_REGION_START_X);

			  eGFX_printf_HorizontalCentered_Colored(&eGFX_BackBuffer,
											50, 
											&Magneto__26px__Regular__AntiAliasGridFit_16BPP_565 ,   
											eGFX_RGB888_TO_RGB565(255,255,255),
											"Slider Value : %.2f",MonkeySliderValue);
				
		
				//Render the Slider
				
				//First, let's draw a box for the slider fill.   There is eGFX function to make a box
				
				eGFX_Box SliderFillBox;

				//Since the monkey sprite has some size to it,  we render the slider a little longer so the head always appears on the slider.
				SliderFillBox.P1.X = SLIDER_REGION_START_X - Sprite_16BPP_565_monkey.SizeX/2;   
				SliderFillBox.P1.Y = (SLIDER_REGION_STOP_Y+SLIDER_REGION_START_Y)/2 - 5;
				SliderFillBox.P2.X= MonkeySliderPosition;
				SliderFillBox.P2.Y= (SLIDER_REGION_STOP_Y+SLIDER_REGION_START_Y)/2 + 5;
				
				eGFX_DrawFilledBox(&eGFX_BackBuffer,&SliderFillBox,eGFX_RGB888_TO_RGB565(128,128,128));
				
				//Now let's draw the monkey head. The "blit" function just copies one ImagePlane to another.
				//In this case we will copy the monkey head to our back buffer
				
				eGFX_DMA_16BPP_Blit(&eGFX_BackBuffer,
								MonkeySliderPosition - Sprite_16BPP_565_monkey.SizeX/2 + 1,  //x coordinate of where to put the monkey head
								SLIDER_REGION_START_Y,																			 //y coordinate of where to put the monkey head
								&Sprite_16BPP_565_monkey);																								
			
			
			  //Let's draw some lines on the left and right to define our boundaries
				eGFX_DrawVline(&eGFX_BackBuffer,SLIDER_REGION_START_Y,SLIDER_REGION_STOP_Y,SLIDER_REGION_START_X-Sprite_16BPP_565_monkey.SizeX/2,eGFX_RGB888_TO_RGB565(255,255,255));
				eGFX_DrawVline(&eGFX_BackBuffer,SLIDER_REGION_START_Y,SLIDER_REGION_STOP_Y,SLIDER_REGION_START_X-Sprite_16BPP_565_monkey.SizeX/2-1,eGFX_RGB888_TO_RGB565(255,255,255));
				
				eGFX_DrawVline(&eGFX_BackBuffer,SLIDER_REGION_START_Y,SLIDER_REGION_STOP_Y,SLIDER_REGION_STOP_X+Sprite_16BPP_565_monkey.SizeX/2,eGFX_RGB888_TO_RGB565(255,255,255));
				eGFX_DrawVline(&eGFX_BackBuffer,SLIDER_REGION_START_Y,SLIDER_REGION_STOP_Y,SLIDER_REGION_STOP_X+Sprite_16BPP_565_monkey.SizeX/2+1,eGFX_RGB888_TO_RGB565(255,255,255));

			  //The last step is to dump the backbuffer to the screen.
				eGFX_Dump(&eGFX_BackBuffer);
		}
}



