################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/main.c 

OBJS += \
./source/main.o 

C_DEPS += \
./source/main.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DSDK_DEBUGCONSOLE=0 -DARM_MATH_CM4 -DSDK_DEBUGCONSOLE_UART -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DCPU_LPC54608J512BD208 -DCPU_LPC54608J512BD208_cm4 -D__REDLIB__ -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 3 -  Audio Spectrum Analyzer\source\Audio" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 3 -  Audio Spectrum Analyzer\source\CMSIS" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 3 -  Audio Spectrum Analyzer\source\drivers" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 3 -  Audio Spectrum Analyzer\source\eGFX" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 3 -  Audio Spectrum Analyzer\source\eGFX\Drivers" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 3 -  Audio Spectrum Analyzer\source\eGFX\Fonts\Arial__23px__Regular__SystemDefault_1BPP" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 3 -  Audio Spectrum Analyzer\source\eGFX\Fonts" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 3 -  Audio Spectrum Analyzer\source\eGFX\Fonts\Magneto__26px__Regular__AntiAliasGridFit_16BPP_565" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 3 -  Audio Spectrum Analyzer\source\eGFX\Fonts\OCR_A_Extended__20px__Bold__SingleBitPerPixelGridFit_1BPP" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 3 -  Audio Spectrum Analyzer\source\eGFX\Fonts\Consolas__26px__Regular__AntiAliasGridFit_1BPP" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 3 -  Audio Spectrum Analyzer\source\eGFX\Sprites" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 3 -  Audio Spectrum Analyzer\source\lpcxpresso54608" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 3 -  Audio Spectrum Analyzer\source\startup" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 3 -  Audio Spectrum Analyzer\source\TouchScreen" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 3 -  Audio Spectrum Analyzer\source\utilities" -I"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 3 -  Audio Spectrum Analyzer" -O3 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


