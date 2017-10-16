#include "eGFX_DataTypes.h"

#ifndef GFX_DRIVER_
#define GFX_DRIVER_

#define eGFX_PHYSICAL_SCREEN_SIZE_X	((uint16_t) 480)	//This is the actual X and Y size of the physical screen in *pixels*
#define eGFX_PHYSICAL_SCREEN_SIZE_Y	((uint16_t) 272)

//These are the prototypes for the GFX HAL
extern void	eGFX_InitDriver(void);
extern void	eGFX_Dump(eGFX_ImagePlane *Image);
extern void eGFX_WaitForVSync(void);
//A Driver *Must* have a backbuffer exposed
extern eGFX_ImagePlane eGFX_BackBuffer;

extern void eGFX_ImagePlane_16BPP_ClearToColor(eGFX_ImagePlane   *Image,uint16_t Color);
extern void eGFX_DMA_16BPP_Blit(eGFX_ImagePlane *Destination,
								int16_t x,
								int16_t y,
								const eGFX_ImagePlane *Sprite);
#endif
