################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include source/utilities/subdir.mk
-include source/startup/subdir.mk
-include source/lpcxpresso54608/subdir.mk
-include source/eGFX/Sprites/subdir.mk
-include source/eGFX/Fonts/OCR_A_Extended__20px__Bold__SingleBitPerPixelGridFit_1BPP/subdir.mk
-include source/eGFX/Fonts/Magneto__26px__Regular__AntiAliasGridFit_16BPP_565/subdir.mk
-include source/eGFX/Fonts/Consolas__26px__Regular__AntiAliasGridFit_1BPP/subdir.mk
-include source/eGFX/Fonts/Arial__23px__Regular__SystemDefault_1BPP/subdir.mk
-include source/eGFX/Fonts/subdir.mk
-include source/eGFX/Drivers/subdir.mk
-include source/eGFX/subdir.mk
-include source/drivers/subdir.mk
-include source/TouchScreen/subdir.mk
-include source/CMSIS/subdir.mk
-include source/Audio/subdir.mk
-include source/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Demo\ 1\ -\ Drag\ The\ Monkey.axf

# Tool invocations
Demo\ 1\ -\ Drag\ The\ Monkey.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\Eli Hughes\Desktop\FIT2017_LPC54628\SRC\Demo 1 - Drag The Monkey\libs" -Xlinker -Map="Demo 1 - Drag The Monkey.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -T "Demo_1_-_Drag_The_Monkey_Debug.ld" -o "Demo 1 - Drag The Monkey.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) "Demo 1 - Drag The Monkey.axf"
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "Demo 1 - Drag The Monkey.axf"; # arm-none-eabi-objcopy -v -O binary "Demo 1 - Drag The Monkey.axf" "Demo 1 - Drag The Monkey.bin" ; # checksum -p LPC54608J512 -d "Demo 1 - Drag The Monkey.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
