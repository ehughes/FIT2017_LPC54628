#include <fsl_i2c.h>
#include <fsl_lcdc.h>
#include <fsl_sctimer.h>
#include "fsl_dma.h"
#include "board.h"
#include "eGFX_DataTypes.h"
#include "eGFX.h"
#include "eGFX_Driver.h"
#include "cr_section_macros.h"
#include "pin_mux.h"



#define EXAMPLE_I2C_MASTER_BASE (I2C2_BASE)
#define I2C_MASTER_CLOCK_FREQUENCY (12000000)



#define APP_LCD LCD
#define APP_LCD_IRQHandler LCD_IRQHandler
#define APP_LCD_IRQn LCD_IRQn

#define LCD_PANEL_CLK 9000000
#define LCD_PPL 480
#define LCD_HSW 2
#define LCD_HFP 8
#define LCD_HBP 43
#define LCD_LPP 272
#define LCD_VSW 10
#define LCD_VFP 4
#define LCD_VBP 12
#define LCD_POL_FLAGS kLCDC_InvertVsyncPolarity | kLCDC_InvertHsyncPolarity
#define LCD_INPUT_CLK_FREQ CLOCK_GetFreq(kCLOCK_LCD)
#define LCD_WIDTH 480
#define LCD_HEIGHT 272
#define LCD_BITS_PER_PIXEL 16


#define EXAMPLE_I2C_MASTER ((I2C_Type *)EXAMPLE_I2C_MASTER_BASE)
#define I2C_MASTER_SLAVE_ADDR_7BIT 0x7EU
#define I2C_BAUDRATE 100000U


/* Frame end flag. */
static volatile bool s_frameEndFlag;

eGFX_ImagePlane eGFX_BackBuffer;

__NOINIT(BOARD_SDRAM) uint8_t eGFX_FrameBuffer1[eGFX_CALCULATE_16BPP_IMAGE_STORAGE_SPACE_SIZE(480,272)];
__NOINIT(BOARD_SDRAM) uint8_t eGFX_FrameBuffer2[eGFX_CALCULATE_16BPP_IMAGE_STORAGE_SPACE_SIZE(480,272)];


#define FRAMEBUFFER1_ADDR   (SDRAM_BASE_ADDR)
#define FRAMEBUFFER2_ADDR   (SDRAM_BASE_ADDR + eGFX_CALCULATE_16BPP_IMAGE_STORAGE_SPACE_SIZE(480,272)*2)


#define I2C0_FLEXCOMM	2

static void BOARD_InitPWM(void)
{
    sctimer_config_t config;
    sctimer_pwm_signal_param_t pwmParam;
    uint32_t event;

    CLOCK_AttachClk(kMCLK_to_SCT_CLK);

    CLOCK_SetClkDiv(kCLOCK_DivSctClk, 2, true);

    SCTIMER_GetDefaultConfig(&config);

    SCTIMER_Init(SCT0, &config);

    pwmParam.output = kSCTIMER_Out_5;
    pwmParam.level = kSCTIMER_HighTrue;
    pwmParam.dutyCyclePercent = 5;

    SCTIMER_SetupPwm(SCT0, &pwmParam, kSCTIMER_CenterAlignedPwm, 1000U, CLOCK_GetFreq(kCLOCK_Sct), &event);
}

void APP_LCD_IRQHandler(void)
{
    uint32_t intStatus = LCDC_GetEnabledInterruptsPendingStatus(APP_LCD);

    LCDC_ClearInterruptsStatus(APP_LCD, intStatus);

  if (intStatus & kLCDC_BaseAddrUpdateInterrupt)
    {
     s_frameEndFlag = true;
		}
    __DSB();
}

status_t APP_LCDC_Init(void)
{
    /* Initialize the display. */
    lcdc_config_t lcdConfig;

    LCDC_GetDefaultConfig(&lcdConfig);

    lcdConfig.panelClock_Hz = LCD_PANEL_CLK;
    lcdConfig.ppl = LCD_PPL;
    lcdConfig.hsw = LCD_HSW;
    lcdConfig.hfp = LCD_HFP;
    lcdConfig.hbp = LCD_HBP;
    lcdConfig.lpp = LCD_LPP;
    lcdConfig.vsw = LCD_VSW;
    lcdConfig.vfp = LCD_VFP;
    lcdConfig.vbp = LCD_VBP;
    lcdConfig.polarityFlags = LCD_POL_FLAGS;
    lcdConfig.upperPanelAddr = (uint32_t)&eGFX_FrameBuffer1[0];
    
	lcdConfig.bpp = kLCDC_16BPP565;
    lcdConfig.display = kLCDC_DisplayTFT;
    lcdConfig.swapRedBlue = true;

    LCDC_Init(APP_LCD, &lcdConfig, LCD_INPUT_CLK_FREQ);

    ///* Trigger interrupt at start of every vertical back porch. */
    
    LCDC_EnableInterrupts(APP_LCD, kLCDC_BaseAddrUpdateInterrupt);
    NVIC_EnableIRQ(APP_LCD_IRQn);

    LCDC_Start(APP_LCD);
    LCDC_PowerUp(APP_LCD);

    return kStatus_Success;
}
status_t APP_I2C_Init(void)
{
    i2c_master_config_t masterConfig;

    I2C_MasterGetDefaultConfig(&masterConfig);

    /* Change the default baudrate configuration */
    masterConfig.baudRate_Bps = I2C_BAUDRATE;

    /* Initialize the I2C master peripheral */
    I2C_MasterInit(EXAMPLE_I2C_MASTER, &masterConfig, I2C_MASTER_CLOCK_FREQUENCY);

    return kStatus_Success;
}


void eGFX_InitDriver()
{
	/* Route Main clock to LCD. */
    CLOCK_AttachClk(kMCLK_to_LCD_CLK);
	  
	/*Divide the LCD clock by 2*/
	CLOCK_SetClkDiv(kCLOCK_DivLcdClk, 2, true);
	
	BOARD_InitPWM();
	
	APP_LCDC_Init();
	
	eGFX_ImagePlaneInit(&eGFX_BackBuffer,&eGFX_FrameBuffer2[0],eGFX_PHYSICAL_SCREEN_SIZE_X, eGFX_PHYSICAL_SCREEN_SIZE_Y,eGFX_IMAGE_PLANE_16BPP_565);

    DMA_Init(DMA0);

}

void eGFX_WaitForVSync()
{

	while(!s_frameEndFlag)
	{
	}

}

void eGFX_Dump(eGFX_ImagePlane *Image)
{
 //	eGFX_WaitForVSync();

 	if(Image->Data == &eGFX_FrameBuffer2[0])
		{
			Image->Data  = &eGFX_FrameBuffer1[0];
			LCD->UPBASE  = (uint32_t)(&eGFX_FrameBuffer2[0]);
		}
		else
		{
			Image->Data  = &eGFX_FrameBuffer2[0];
			LCD->UPBASE = (uint32_t)(&eGFX_FrameBuffer1[0]);
		}
		 	s_frameEndFlag = false;
	
	eGFX_WaitForVSync();
   
}


uint8_t DMA_GetFreeChannel(){

	for(uint8_t i=0;i<FSL_FEATURE_DMA_NUMBER_OF_CHANNELS;i++){
		if(!(DMA_ChannelIsActive(DMA0, i)))
			return i;
	}
	return 255;
}

volatile uint8_t blitComplete;



/*
void eGFX_Blit_Callback(uint8_t channel){
	blitComplete = 1;
}*/

/*
void eGFX_Blit(eGFX_ImagePlane *Destination,
	int16_t x,
	int16_t y,
	const eGFX_ImagePlane *Sprite)
{
    uint16_t i,j;

    for(j=0; j<Sprite->SizeY; j++)
    {
        for(i=0; i<Sprite->SizeX; i++)
        {
			eGFX_PutPixel(Destination, x + i, y + j, eGFX_GetPixel(Sprite, i, j));
        }

    }
}
*/
dma_descriptor_t line[272] __attribute__ ((aligned (16)));
//x,y is a the upper left corner of the blit, not the center
/*
void eGFX_Blit(eGFX_ImagePlane *Destination,
	int16_t x,
	int16_t y,
	const eGFX_ImagePlane *Sprite)
{
    dma_transfer_config_t transferConfig;

    dma_xfercfg_t options;

    for(int i=1;i<Sprite->SizeY;i++){
    	dma_descriptor_t *nextDesc;
    	options.byteWidth = 2;
    	options.clrtrig = 0;
    	options.dstInc = 1;
    	options.srcInc = 1;
    	if(i != Sprite->SizeY - 1){
    		options.intA = 0;
    		options.intB = 0;
    		options.reload = 1;
    		nextDesc = &line[i+1];
    	}
    	else{
    		options.intA = 1;
    		options.reload = 0;
    		nextDesc = NULL;
    	}

    	options.intB =0;
    	options.swtrig = 0;
    	options.transferCount = Sprite->SizeX;
    	options.valid = true;

    	DMA_CreateDescriptor(&line[i],
    						 &options,
							 &Sprite->Data[i * Sprite->SizeX * 2],
							 &Destination->Data[x*2 + ((y + i) * Destination->SizeX *2)],
							 nextDesc);

    }
    while(DMA_GetFreeChannel() == 255){}
    volatile uint8_t channel = DMA_GetFreeChannel();

	DMA_EnableChannel(DMA0, channel);
	DMA_CreateHandle(&DMA_Handle_Blit, DMA0, channel);

	DMA_PrepareTransfer(&transferConfig,
			 	 	 	&Sprite->Data[0],
						&Destination->Data[x*2 + ((y) * Destination->SizeX *2)],
						2 * sizeof(Sprite->Data[0]),
						(Sprite->SizeX)*2,
						kDMA_MemoryToMemory,
						&line[1]);

	while(DMA_SubmitTransfer(&DMA_Handle_Blit, &transferConfig) == kStatus_DMA_Busy){}
	DMA_StartTransfer(&DMA_Handle_Blit);
	while(DMA0->COMMON->ACTIVE & (1<<channel)){}
}
*/

dma_handle_t DMA_Handle_Clear;
dma_handle_t DMA_Handle_Blit;
volatile uint8_t ClearDone = 0;
void eGFX_ImagePlane_Clear_Callback(struct _dma_handle *handle, void *userData, bool transferDone, uint32_t intmode)
{
	ClearDone++;
}

static C = 0;

void eGFX_ImagePlane_16BPP_ClearToColor(eGFX_ImagePlane   *Image,uint16_t Color)
{
	dma_transfer_config_t transferConfig;
    dma_xfercfg_t options;
    memset(&transferConfig, 0x00, sizeof(transferConfig));
    memset(line, 0x00, sizeof(line));
    uint32_t TotalDesc = eGFX_PHYSICAL_SCREEN_SIZE_Y;
    C = Color;

    		for(int i =1; i< TotalDesc; i++){
    	dma_descriptor_t *nextDesc;
    	options.byteWidth = 2;
    	options.clrtrig = 0;
    	options.dstInc = 1;
    	options.srcInc = 0;
    	if(i != TotalDesc - 1){
    		options.intA = 0;
    		options.intB = 0;
    		options.reload = 1;
    		nextDesc = &line[i+1];
    	}
    	else{
    		options.intA = 1;
    		options.reload = 0;
    		nextDesc = NULL;
    	}

    	options.intB =0;
    	options.swtrig = 0;
    	options.transferCount = eGFX_PHYSICAL_SCREEN_SIZE_X;
    	options.valid = true;

    	DMA_CreateDescriptor(&line[i],
    						 &options,
							 &C,
							 &Image->Data[i * eGFX_PHYSICAL_SCREEN_SIZE_X *2],
							 nextDesc);
    }
    volatile uint8_t channel;
    channel = DMA_GetFreeChannel();

	ClearDone = 0;
	DMA_EnableChannel(DMA0, channel);
	DMA_CreateHandle(&DMA_Handle_Clear, DMA0, channel);
	DMA_SetCallback(&DMA_Handle_Clear, eGFX_ImagePlane_Clear_Callback, NULL);

	DMA_PrepareTransfer(&transferConfig,
						&C,
						&Image->Data[0],
						2,
						eGFX_PHYSICAL_SCREEN_SIZE_X*2,
						kDMA_MemoryToMemory,
						&line[1]);
	transferConfig.xfercfg.srcInc = 0;
	DMA_SubmitTransfer(&DMA_Handle_Clear, &transferConfig);
	DMA_StartTransfer(&DMA_Handle_Clear);
	while(!ClearDone){}
	while(DMA0->COMMON->ACTIVE & (1<<channel)){}
}


