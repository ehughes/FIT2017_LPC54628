################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/TouchScreen/fsl_ft5406.c 

OBJS += \
./source/TouchScreen/fsl_ft5406.o 

C_DEPS += \
./source/TouchScreen/fsl_ft5406.d 


# Each subdirectory must supply rules for building sources it contributes
source/TouchScreen/%.o: ../source/TouchScreen/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DSDK_DEBUGCONSOLE=0 -DSDK_DEBUGCONSOLE_UART -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DCPU_LPC54608J512BD208 -DCPU_LPC54608J512BD208_cm4 -D__REDLIB__ -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 4 - Starfield\source\Audio" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 4 - Starfield\source\CMSIS" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 4 - Starfield\source\drivers" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 4 - Starfield\source\eGFX" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 4 - Starfield\source\eGFX\Drivers" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 4 - Starfield\source\eGFX\Fonts\Arial__23px__Regular__SystemDefault_1BPP" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 4 - Starfield\source\eGFX\Fonts" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 4 - Starfield\source\eGFX\Fonts\Magneto__26px__Regular__AntiAliasGridFit_16BPP_565" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 4 - Starfield\source\eGFX\Fonts\OCR_A_Extended__20px__Bold__SingleBitPerPixelGridFit_1BPP" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 4 - Starfield\source\eGFX\Fonts\Consolas__26px__Regular__AntiAliasGridFit_1BPP" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 4 - Starfield\source\eGFX\Sprites" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 4 - Starfield\source\lpcxpresso54608" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 4 - Starfield\source\startup" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 4 - Starfield\source\TouchScreen" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 4 - Starfield\source\utilities" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 4 - Starfield" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


