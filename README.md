# ST7735_LTSM Readme

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/paypalme/whitelight976)

![ ig ](https://github.com/gavinlyonsrepo/pic_16F18346_projects/blob/master/images/st7735/pcb.jpg)

## Table of contents

* [Overview](#overview)
* [Installation](#installation)
* [Dependency](#dependency)
* [Documentation](#documentation)
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
8. [Project url link](https://github.com/gavinlyonsrepo/ST7735_LTSM)

## Installation

The library is included in the official Arduino library manger and the optimum way to install it is using the library manager which can be opened by the *manage libraries* option in Arduino IDE.

## Dependency

This library requires the Arduino library 'display16_LTSM' as a dependency. display16_LTSM library contains
the graphics, bitmaps, and font methods as well as font data and bitmap test data. Its also
where the user sets options(debug, advanced graphics and frame buffer mode).
When you install 'ST7735_LTSM' with Arduino IDE. It should install 'display16_LTSM' as well after
a prompt, if it does not you have to install it same way as 'ST7735_LTSM'.
The 'display16_LTSM' project and readme is at [URL github link.](https://github.com/gavinlyonsrepo/display16_LTSM)
'display16_LTSM' is also written by author of this library.

## Documentation

Code is commented for the 'doxygen' API generation tool.
Documents on fonts, bitmaps and graphics can be found at
the dependency 'display16_LTSM' repository, [URL github link](https://github.com/gavinlyonsrepo/display16_LTSM)

## Software

### Examples

There are example files included.

| Filename .ino | Function | Note |
| --- | --- | --- |
| HELLO WORLD | Hello world basic use case on 128x128 pixel screen | --- |
| HELLO WORLD 80x160 | Hello world basic use case on 80x160 pixel screen | --- |
| HELLO WORLD 128x160 | Hello world basic use case on 128x160 pixel screen | --- |
| TEST | Text + fonts | --- |
| GRAPHICS | Graphics | dislib16 ADVANCED GRAPHICS ENABLE must be enabled for all tests to work |
| FUNCTIONS FPS | Functions(like rotate, scroll) + FPS tests | --- |
| BITMAP | 1,8 & 16 bit bitmaps tests + bitmap FPS tests | Bitmap test data is stored in arrays |
| DEMOS | A demo showing a round coloured gauge | dislib16 ADVANCED GRAPHICS ENABLE must be enabled |
| DEMOS_2 | Simulated voltmeter with gradient color gauge | |
| DEMOS_3 | Vertical Gauges based on sin(x), cos(x), & sin(x)*cos(x). Updates over time to create a dynamic effect. | --- |
| FRAME BUFFER | Testing frame Buffer mode | dislib16 ADVANCED SCREEN BUFFER ENABLE must be enabled user option 2 |

### SPI

In the example ino files. There are sections in "setup()" function
where user can make adjustments to select for SPI type used, PCB type used and screen size.

1. USER OPTION 1 GPIO, SPI_SPEED + TYPE
2. USER OPTION 2 SCREEN SECTION
3. USER OPTION 3 PCB_TYPE

#### USER OPTION 1 GPIO SPI SPEED

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

#### USER OPTION 2 Screen size  + Offsets

User can adjust screen pixel height, screen pixel width and x & y screen offsets.
These offsets can be used in the event of screen damage or manufacturing errors around edge
such as cropped data or defective pixels. The function TFTInitScreenSize sets them.

#### USER OPTION 3 PCB Version

Select your PCB controller type by passing an enum type to function  TFTInitPCBType.
Default is "TFT_ST7735R_Red".  If you select the wrong one if may still work but with inverted colors.

| Number | Description | Enum label | Tested on |
| ---- | ---- | --- | --- |
| 1 | ST7735B controller | TFT_ST7735B | n/a |
| 2 | ST7735R "Green Tab" | TFT_ST7735R_Green | n/a |
| 3 | ST7735R "Red Tab" | TFT_ST7735R_Red | RED PCB v1.1, 1.44 , 128 x 128 pixels |
| 4 | ST7735S "Black Tab" | TFT_ST7735S_Black | RED PCB v1.2, 1.8 , 128 x 160 pixels |
| 5 | ST7735S 80x160 | TFT_ST7735S_80160 | Blue PCB .96 inch 80x160 pixels |

The ST7735S 80x160 requires an offset of (26,1) or (24,0) depending on the screen. This is set in the example ino files for this screen at USER OPTION 2.

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

Tested with both software and hardware SPI on:

1. ESP32
2. Arduino UNO R4 Minima, Frame buffer example is not supported on this board.

Compiled only (not fully hardware-tested) on:

1. Arduino UNO
2. ESP8266
3. STM32 “Blue Pill”

> Some examples on low-RAM MCUs will fail( insufficient memory ), numerous fonts and bitmap data are included.
> Frame buffer mode requires sufficient dynamic memory for the buffer — see the README in display16_LTSM for details.

## Output

[![ Demo pic ](https://github.com/gavinlyonsrepo/ST7735_TFT_RPI/blob/main/extra/images/4.jpg)](https://github.com/gavinlyonsrepo/ST7735_TFT_RPI/blob/main/extra/images/4.jpg)
[![ demo pic 2](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/st7735output.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/st7735output.jpg)

## Notes and Issues
