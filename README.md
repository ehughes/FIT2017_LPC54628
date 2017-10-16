# FIT2017_LPC54628

This repository contains LPC54628 examples used at FIT2017.  These examples can be compiled with MCUXpresso 10.0.2.    The MCU SDK is baked into the project.  These examples do not use the power library modifications to run at 220MHz.   These projects run at 180MHz and should also run on the LPCXpresso54608.

Examples:

SRC/Demo 1 - Drag The Monkey

This example shows how to read the touch screen and do some basic graphics.    The audio CODEC is configured to generate a 2KHz tone from the headphone jack.   Samples are read from the ADC for the user to experiment with.

SRC/Demo 2 - DMIC Visualizer

Samples From the digital microphone are stored in a buffer and displayed real-time to the screen

SRC/Demo 3 - Audio Spectrum Analyzer.

A frequency spectrum of the digital microphone is displayed real-time on the screen

SRC/Demo 4 - Starfield

The starship Enterprise flying through space


Some Useful resources:

[LPC54628 Xpresso User Manual] (https://www.nxp.com/docs/en/user-guide/UM11035.pdf)

[Getting Started with the LPCXpresso 54608](https://www.nxp.com/products/microcontrollers-and-processors/arm-based-processors-and-mcus/lpc-cortex-m-mcus/developer-resources-/lpcxpresso-boards/lpcxpresso54628-development-board:OM13098?tab=In-Depth_Tab)


[The MCUXpresso IDE](https://www.nxp.com/support/developer-resources/run-time-software/mcuxpresso-software-and-tools/mcuxpresso-integrated-development-environment-ide-v10.0.2:MCUXpresso-IDE)


[The MCUXpresso SDK Generator](https://mcuxpresso.nxp.com/en/welcome)