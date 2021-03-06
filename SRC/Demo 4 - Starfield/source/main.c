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

#define NUM_STARS   256

typedef struct
{
	float x;
	float y;
	float dx;
	float dy;

	uint16_t Color;

} StarRecord;

StarRecord MyStars[NUM_STARS];

void ProcessStars(StarRecord *SR, uint32_t N)
{
	for(int i=0;i<N;i++)
	{
		SR[i].x += SR[i].dx;
		SR[i].y += SR[i].dy;

		if(SR[i].x > eGFX_PHYSICAL_SCREEN_SIZE_X)
		{
			SR[i].x -= eGFX_PHYSICAL_SCREEN_SIZE_X;
		}
		if(SR[i].x < 0)
		{
			 SR[i].x += eGFX_PHYSICAL_SCREEN_SIZE_X;
		}

		if(SR[i].y > eGFX_PHYSICAL_SCREEN_SIZE_Y)
		{
			SR[i].y -= eGFX_PHYSICAL_SCREEN_SIZE_Y;
		}
		if(SR[i].y < 0)
		{
			 SR[i].y += eGFX_PHYSICAL_SCREEN_SIZE_Y;
		}
	}
}

void RenderStars(StarRecord *SR, uint32_t N)
{
	for(int i=0;i<N;i++)
	{
		eGFX_PutPixel(&eGFX_BackBuffer, SR[i].x, SR[i].y, SR[i].Color);
		eGFX_PutPixel(&eGFX_BackBuffer, SR[i].x+1, SR[i].y, SR[i].Color);
	}
}

void GenerateStars()
{
	for(int i=0;i<NUM_STARS;i++)
		{
			MyStars[i].x= rand()%eGFX_PHYSICAL_SCREEN_SIZE_X;
			MyStars[i].dx= -10.0f * (float)(rand()%255)/255.0f;
			MyStars[i].Color = eGFX_RGB888_TO_RGB565(255,255,255);
			MyStars[i].y= rand()%eGFX_PHYSICAL_SCREEN_SIZE_Y;
		}
}

int main(void)
{
	

	
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


	   FT5406_Init(&touch_handle,I2C2);

	   /*
	    * We get a COM port to the PC to do printf's
	    *
	    */
	   PRINTF("Demo 4 : Star Field!\r\n");

	   GenerateStars();

		while(1)
		{

			 ProcessStars(&MyStars[0], NUM_STARS);


			  eGFX_ImagePlane_16BPP_ClearToColor(&eGFX_BackBuffer,eGFX_RGB888_TO_RGB565(0,0,0));
			
			  RenderStars(&MyStars[0], NUM_STARS);

			  eGFX_DMA_16BPP_Blit(&eGFX_BackBuffer, 10, 100, &Sprite_16BPP_565_enterprise);
			  eGFX_Dump(&eGFX_BackBuffer);
		}
}



