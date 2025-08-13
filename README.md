# ST7735_LTSM Readme

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/paypalme/whitelight976)

![ ig ](https://github.com/gavinlyonsrepo/pic_16F18346_projects/blob/master/images/st7735/pcb.jpg)

## Table of contents

* [Overview](#overview)
* [Installation](#installation)
* [Dependency](#dependency)
* [Software](#software)
	* [Examples](#examples)
	* [SPI](#spi)
* [Hardware](#hardware)
* [Tested](#tested)
* [Output](#output)
* [Notes and Issues](#notes-and-issues)


## Overview

* Name: ST7735_LTSM
* Description:

C++ Library for a ST7735 TFT SPI LCD for the Arduino Eco-system.

1. Arduino eco-system library.
2. Invert, Scroll, rotate and sleep control.
3. 16 ASCII fonts included, fonts can easily be removed or added.
4. Advanced graphics class included.
5. Advanced frame buffer mode included.
6. Bitmaps supported: 1, 8 and 16 bit.
7. Hardware & software SPI options
8. Code commented for 'doxygen' API generation tool.

## Installation

The library is included in the official Arduino library manger and the optimum way to install it is using the library manager which can be opened by the *manage libraries* option in Arduino IDE. 

## Dependency

This library requires the Arduino library 'display16_LTSM' as a dependency. display16_LTSM library contains
the graphics, bitmaps, and font methods as well as font data and bitmap test data. Its also 
where the user sets options(debug, advanced graphics and frame buffer mode).
When you install 'ST7735_LTSM' with Arduino IDE. It should install 'display16_LTSM' as well after 
a prompt, if it does not you have to install it same way as 'ST7735_LTSM'.
The 'display16_LTSM' project and readme is at [URL github link](https://github.com/gavinlyonsrepo/display16_LTSM)

## Software

### Examples

There are 7 example files included.

| Filename .ino | Function  | Note |
| --- | --- | --- |
| HELLO WORLD | Hello world basic use case | --- |
| TEST | Text + fonts | --- |
| GRAPHICS| Graphics | dislib16 ADVANCED GRAPHICS ENABLE must be enabled for all tests to work|
| FUNCTIONS FPS| Functions(like rotate, scroll) + FPS tests| --- |
| BITMAP| 1,8 & 16 bit bitmaps tests + bitmap FPS tests| Bitmap test data is stored in arrays |
| DEMO| A demo showing a gauge | dislib16 ADVANCED GRAPHICS ENABLE must be enabled |
| FRAME BUFFER | Testing frame Buffer mode | dislib16 ADVANCED SCREEN BUFFER ENABLE must be enabled user option 2 |

### SPI

In the example ino files. There are sections in "setup()" function
where user can make adjustments to select for SPI type used, PCB type used and screen size.

1. USER OPTION 1 GPIO, SPI_SPEED + TYPE
2. USER OPTION 2 SCREEN SECTION
3. USER OPTION 3 PCB_TYPE


*USER OPTION 1 GPIO SPI SPEED*

Two different constructors which one is called depends on 'bhardwareSPI', 
true for hardware spi, false for software SPI.

Hardware SPI:

Here the user can pass the SPI Bus freq in Hertz, Currently set to 8 Mhz,
and the Reset, chip select and data or command line. Any GPIO can be used for these.
The MOSI and CLk are tied to default MCU SPI interface GPIO.

Software SPI:

The optional GPIO software uS delay,which by default is zero.
Setting this higher can be used to slow down Software SPI
which may be beneficial on Fast MCU's.
The 5 GPIO pins used. Any GPIO can be used for these.

*USER OPTION 2 Screen size  + Offsets*

User can adjust screen pixel height, screen pixel width and x & y screen offsets.
These offsets can be used in the event of screen damage or manufacturing errors around edge
such as cropped data or defective pixels. The function TFTInitScreenSize sets them.

*USER OPTION 3 PCB Version*

Select your PCB controller type by passing an enum type to function  TFTInitPCBType.
Default is "TFT_ST7735R_Red".  If you select the wrong one if may still work but with inverted colors.

| Number | Description | Enum label| Tested on |
| ---- | ---- | --- | --- |
| 1 | ST7735B controller| TFT_ST7735B | n/a |
| 2 | ST7735R "Green Tab" | TFT_ST7735R_Green | n/a |
| 3 | ST7735R "Red Tab"   | TFT_ST7735R_Red | RED PCB v1.1, 1.44 , 128 x 128 pixels |
| 4 | ST7735S "Black Tab" | TFT_ST7735S_Black | RED PCB v1.2, 1.8 , 128 x 160 pixels |


## Hardware

Connections as setup in HELLO_WORLD.ino  test file.

| TFT PinNum | Pindesc | Hardware SPI | Software SPI |
| --- | --- | --- | --- |
| 1 | LED | VCC | VCC |
| 2 | SCLK | MCU SPI CLK | GPIO12 |
| 3 | SDA | MCU MOSI CLK | GPIO13 |
| 4 | A0/DC | GPIO5 | GPIO5 |
| 5 | RESET | GPIO4 | GPIO4 |
| 6 | SS/CS | GPIO15 | GPIO15 |
| 7 | GND | GND | GND |
| 8 | VCC | VCC | VCC |

1. NOTE: Connect LED backlight pin 1 thru a 150R/220R ohm resistor to 3.3/5V VCC.
2. This is a 3.3V logic device do NOT connect the I/O logic lines to 5V logic device.
3. You can connect VCC to 5V if there is a 3.3 volt regulator on back of TFT module.
4. Pick any GPIO you like but on HW SPI mode SCLK and SDA will be tied to SPIO interface of MCU.
5. Backlight on/off control is left to user.


## Tested 

Tested (1-2) on following MCUs both software and hardware SPI,

1. ESP32 
2. UNO Minima R4 (Exception : Frame buffer example will not work here)

Complied only on these 3-5. Note : many low RAM MCU will
not work if a lot of fonts and bitmaps are used. In addition 'Frame Buffer mode requires 
Dynamic memory to hold a frame buffer. See dependency readme for more details.

3. Arduino UNO & NANO v3
4. ESP8266 
5. STM32 "blue pill"


## Output

[![ Demo pic ](https://github.com/gavinlyonsrepo/ST7735_TFT_RPI/blob/main/extra/images/4.jpg)](https://github.com/gavinlyonsrepo/ST7735_TFT_RPI/blob/main/extra/images/4.jpg)
[![ demo pic 2](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/st7735output.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/st7735output.jpg)

## Notes and Issues
