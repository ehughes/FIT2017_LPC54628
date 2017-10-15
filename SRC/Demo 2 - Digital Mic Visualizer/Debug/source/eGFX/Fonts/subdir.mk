################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/eGFX/Fonts/FONT_3_5_1BPP.c \
../source/eGFX/Fonts/FONT_3_5_4BPP.c \
../source/eGFX/Fonts/FONT_3_5_8BPP.c \
../source/eGFX/Fonts/FONT_5_7_1BPP.c \
../source/eGFX/Fonts/FONT_5_7_4BPP.c \
../source/eGFX/Fonts/FONT_5_7_8BPP.c 

OBJS += \
./source/eGFX/Fonts/FONT_3_5_1BPP.o \
./source/eGFX/Fonts/FONT_3_5_4BPP.o \
./source/eGFX/Fonts/FONT_3_5_8BPP.o \
./source/eGFX/Fonts/FONT_5_7_1BPP.o \
./source/eGFX/Fonts/FONT_5_7_4BPP.o \
./source/eGFX/Fonts/FONT_5_7_8BPP.o 

C_DEPS += \
./source/eGFX/Fonts/FONT_3_5_1BPP.d \
./source/eGFX/Fonts/FONT_3_5_4BPP.d \
./source/eGFX/Fonts/FONT_3_5_8BPP.d \
./source/eGFX/Fonts/FONT_5_7_1BPP.d \
./source/eGFX/Fonts/FONT_5_7_4BPP.d \
./source/eGFX/Fonts/FONT_5_7_8BPP.d 


# Each subdirectory must supply rules for building sources it contributes
source/eGFX/Fonts/%.o: ../source/eGFX/Fonts/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DSDK_DEBUGCONSOLE=0 -DSDK_DEBUGCONSOLE_UART -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DCPU_LPC54608J512BD208 -DCPU_LPC54608J512BD208_cm4 -D__REDLIB__ -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 2 - Digital Mic Visualizer\source\Audio" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 2 - Digital Mic Visualizer\source\CMSIS" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 2 - Digital Mic Visualizer\source\drivers" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 2 - Digital Mic Visualizer\source\eGFX" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 2 - Digital Mic Visualizer\source\eGFX\Drivers" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 2 - Digital Mic Visualizer\source\eGFX\Fonts\Arial__23px__Regular__SystemDefault_1BPP" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 2 - Digital Mic Visualizer\source\eGFX\Fonts" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 2 - Digital Mic Visualizer\source\eGFX\Fonts\Magneto__26px__Regular__AntiAliasGridFit_16BPP_565" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 2 - Digital Mic Visualizer\source\eGFX\Fonts\OCR_A_Extended__20px__Bold__SingleBitPerPixelGridFit_1BPP" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 2 - Digital Mic Visualizer\source\eGFX\Fonts\Consolas__26px__Regular__AntiAliasGridFit_1BPP" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 2 - Digital Mic Visualizer\source\eGFX\Sprites" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 2 - Digital Mic Visualizer\source\lpcxpresso54608" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 2 - Digital Mic Visualizer\source\startup" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 2 - Digital Mic Visualizer\source\TouchScreen" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 2 - Digital Mic Visualizer\source\utilities" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 2 - Digital Mic Visualizer" -O3 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


