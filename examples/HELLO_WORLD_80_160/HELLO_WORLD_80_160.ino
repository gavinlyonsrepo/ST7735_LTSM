/*!
	@file    HELLO_WORLD_80_160.ino
	@author  Gavin Lyons
	@brief   Example  file for arduino ST7735_LTSM library. Tests Hello World.
	@details change 'bhardwareSPI'  to switch between hardware and software SPI.
			 		 See USER OPTIONS 1-3 in SETUP function,
	@test
		-# Test 101 write out Hello world , 80x160 pixel .96 inch display
*/

// libraries 
#include "ST7735_LTSM.hpp"
// Fonts needed
#include "fonts_LTSM/FontDefault_LTSM.hpp" 

/// @cond

#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is defined. This example is not for that mode")
#endif

ST7735_LTSM myTFT;
bool bhardwareSPI = true; // true for hardware spi, false for software

void setup(void)
{
	Serial.begin(38400); 
	delay(1000);
//*************** USER OPTION 1 SPI_SPEED + TYPE ***********
	int8_t DC_TFT  = 5;
	int8_t RST_TFT = 4;
	int8_t CS_TFT  = 15;  
	if (bhardwareSPI == true) { // hw spi
		uint32_t TFT_SCLK_FREQ = 8000000;  // Spi freq in Hertz
		myTFT.setupGPIO_SPI(TFT_SCLK_FREQ, RST_TFT, DC_TFT, CS_TFT); 
	} else { // sw spi
		uint16_t SWSPICommDelay = 0; // optional SW SPI GPIO delay in uS
		int8_t SDIN_TFT = 13; 
		int8_t SCLK_TFT = 12; 
		myTFT.setupGPIO_SPI(SWSPICommDelay,RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
	}
//********************************************************
// ****** USER OPTION 2 Screen Setup ****** 
	uint8_t OFFSET_COL = 26;   // 'buydisplay' init code suggests set to 24
	uint8_t OFFSET_ROW = 1;	   // 'buydisplay' init code suggests set to  0
	uint16_t TFT_WIDTH  = 80;   // Screen width in pixels
	uint16_t TFT_HEIGHT = 160; // Screen height in pixels
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW, TFT_WIDTH, TFT_HEIGHT);
// ******************************************
// ******** USER OPTION 3 PCB_TYPE  **************************
	myTFT.TFTInitPCBType(myTFT.TFT_ST7735S_80160); // pass enum,multi choices,see README
//**********************************************************
  myTFT.setRotation(myTFT.Degrees_0);
}

//   MAIN loop
void loop(void) 
{
	Test101();
	EndTests();
}
// *** End OF MAIN **

// Function Space 
void Test101(void) {
	myTFT.setRotation(myTFT.Degrees_0);
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setTextColor(myTFT.C_RED, myTFT.C_BLACK);
	myTFT.setCursor(5,5);
	myTFT.setFont(FontDefault);
	myTFT.print("Hello");
	delay(5000);
	myTFT.fillScreen(myTFT.C_BLACK);
	delay(1000);
}

void EndTests(void)
{
	myTFT.TFTPowerDown(); 
	while(1){};
}


/// @endcond
