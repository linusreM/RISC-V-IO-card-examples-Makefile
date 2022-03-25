# RISC-V MCU (GD32VF103) Examples for IO-card

The examples in this repository are intended as starting points for some of the different peripherals available on the GD32VF103 chip and some of the additional hardware on the IO-card used in some courses at KTH. 

To make getting started as easy as possible the examples in general only include the most basic settings to make a peripheral work. To go further you will need to either look at the firmware library examples which are much more comprehensive, or reference the manual directly. 

## Using the examples

The main program is located in **src/app.c**. Any external library used will be in **lib/[library name]**.

### Opening

Transfer the source files into a new project from the template (provided in your courseware) and you should be set.

You need a working toolchain which includes the libraries found in [GD32VF103 Firmware Library Examples](https://github.com/riscv-mcu/GD32VF103_Firmware_Library).

If you want to mix two projects make sure to bring along any neccessary libraries.

### Programming

Use **make dfu** after putting the board into bootloader mode by pressing reset+boot, then releasing reset, and last releasing boot.

## Acknowledgements

The examples in this repository are based on a number of different sources. 

* [GD32VF103 Firmware Library Examples](https://github.com/riscv-mcu/GD32VF103_Firmware_Library) Main reference
* [GD32V Virtual Com Port](https://github.com/linusreM/GD32V-Virtual-COM-Port) in turn based on
* [CDC_ACM Example](https://github.com/riscv-mcu/GD32VF103_Demo_Suites/tree/master/GD32VF103C_START_Demo_Suites/Projects/04_USBFS/Device/CDC_ACM) For USB base code 
* [Longan_GD32VF_examples](https://github.com/sipeed/Longan_GD32VF_examples) For LCD and RGB-led 
* [FatFs by ChaN](http://elm-chan.org/fsw/ff/00index_e.html) For the sd-card read/write

