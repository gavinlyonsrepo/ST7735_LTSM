/*!
	@file    GRAPHICS.ino
	@author  Gavin Lyons, Liontron systems.
	@brief   Example ino file for st7735 driver, ST7735_LTSM library. Graphics methods testing
	@details See USER OPTIONS 1-3 in SETUP function, dislib16_ADVANCED_GRAPHICS_ENABLE in file 
		   common_dis16_data_LTSM.hpp from display16_graphics_LTSM library.
		   needs to be enabled for all tests 905-910
	@test
		-# Test 901  pixels and lines
		-# Test 902  rectangles
		-# Test 903  Circle
		-# Test 904  Triangles
		-# Test 905 Polygons
		-# Test 906 Dot Grid
		-# Test 907 Quadrilateral
		-# Test 908 Ellipse
		-# Test 909 Draw Arc
		-# Test 910 Draw Line at angle function
*/

// libraries 
#include "ST7735_LTSM.hpp"
#include "display16_graphics_LTSM.hpp" // Depends
#include "fonts_LTSM/FontDefault_LTSM.hpp" // Depends

/// @cond

//  Test timing related defines
#define TEST_DELAY1 1000	  // mS
#define TEST_DELAY2 2000	  // mS
#define TEST_DELAY5 5000	  // mS
#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is defined. This example is not for that mode")
#endif
#ifndef dislib16_ADVANCED_GRAPHICS_ENABLE
#pragma message("gll: dislib16_ADVANCED_GRAPHICS_ENABLE is not defined. it is required for this demo: test 905-910")
#endif

ST7735_LTSM myTFT;
bool bhardwareSPI = true; // true for hardware spi, false for software
uint16_t TFT_WIDTH = 128;// Screen width in pixels
uint16_t TFT_HEIGHT = 128; // Screen height in pixels

void setup(void)
{
	#ifdef dislib16_DEBUG_MODE_ENABLE
	Serial.begin(38400); 
  #endif
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
	uint8_t OFFSET_COL = 0;  // 2, These offsets can be adjusted for any issues->
	uint8_t OFFSET_ROW = 0; // 3, with screen manufacture tolerance/defects
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW , TFT_WIDTH , TFT_HEIGHT);
// ******************************************
// ******** USER OPTION 3 PCB_TYPE  **************************
	myTFT.TFTInitPCBType(myTFT.TFT_ST7735R_Red); // pass enum,4 choices,see README
//**********************************************************
}

//  Section ::  MAIN loop
void loop(void)
{
	Test901();
	Test902();
	Test903();
#ifdef dislib16_ADVANCED_GRAPHICS_ENABLE
	Test905();
	Test906();
	Test907();
	Test908();
	Test909();
	Test910(3);
#endif
	EndTests();
}
// *** End OF MAIN **

void Test901(void)
{
	MILLISEC_DELAY(TEST_DELAY1);
	myTFT.fillScreen(myTFT.C_BLACK);

	myTFT.drawPixel(85, 5, myTFT.C_WHITE);
	myTFT.drawPixel(87, 7, myTFT.C_WHITE);
	myTFT.drawPixel(89, 9, myTFT.C_WHITE);
	myTFT.drawLine(10, 10, 30, 30, myTFT.C_RED);
	myTFT.drawFastVLine(40, 40, 40, myTFT.C_GREEN);
	myTFT.drawFastHLine(60, 60, 40, myTFT.C_YELLOW);

	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

void Test902(void)
{
	myTFT.drawRectWH(5, 5, 20, 20, myTFT.C_RED);
	if(myTFT.fillRectBuffer(45, 5, 20, 20, myTFT.C_YELLOW) != 0) //uses spiwrite
	{
		Serial.println("Test902 Error in the fillRectangle function");
	}
	myTFT.fillRect(85, 5, 20, 20, myTFT.C_GREEN);
	myTFT.drawRoundRect(15, 60, 50, 50, 5, myTFT.C_CYAN);
	myTFT.fillRoundRect(70, 60, 50, 50, 10, myTFT.C_WHITE);

	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

void Test903(void)
{
	myTFT.drawCircle(40, 20, 15, myTFT.C_GREEN);
	myTFT.fillCircle(80, 20, 15, myTFT.C_YELLOW);
	myTFT.drawTriangle(5, 80, 50, 40, 95, 80, myTFT.C_CYAN);
	myTFT.fillTriangle(55, 120, 100, 90, 127, 120, myTFT.C_RED);

	MILLISEC_DELAY(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}


void EndTests(void)
{
	myTFT.TFTPowerDown(); 
	Serial.println("TFT: Tests Over");
  while(1){}
}

#ifdef dislib16_ADVANCED_GRAPHICS_ENABLE
void Test905(void)
{
	//Test 905 : polygons
	myTFT.fillScreen(myTFT.C_BLACK);
	//drawPolygon( x,  y, sides,  diameter,  rotation, color);
	// hexagon  draw
	myTFT.drawPolygon(50, 50, 6, 30, 0, false, myTFT.C_RED);
	delay(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
	// hexagon rotated fill 
	myTFT.drawPolygon(50, 50, 6, 30, 45, true, myTFT.C_WHITE);
	delay(TEST_DELAY5);
  // hexagon fill 
	myTFT.drawPolygon(50, 50, 10, 40, 0, true, myTFT.C_GREEN);
	delay(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

void Test906(void) {
	// Test 906 : dot grid
	myTFT.fillScreen(myTFT.C_BLACK);
	//drawDotGrid(x, y,  w,  h,  DotGridGap, color);
	myTFT.drawDotGrid(0, 0, TFT_WIDTH, TFT_HEIGHT, 5, myTFT.C_GREEN);
	delay(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

void Test907() {
	// Test 907 : quadrilateral
	// Define the  quadrilateral vertices for different shapes
	// Parallelogram 
	int16_t x4 = 45, y4 = 70;   // First vertex
	int16_t x5 = 75, y5 = 75;
	int16_t x6 = 70, y6 = 100;
	int16_t x7 = 40, y7 = 95;
	// Trapezoid 1 
	int16_t x8 = 30, y8 = 30;   // First vertex
	int16_t x9 = 35, y9 = 50;
	int16_t x10 = 60, y10 = 50;
	int16_t x11 = 75, y11 = 30;

	myTFT.drawQuadrilateral(x4, y4, x5, y5, x6, y6, x7, y7, myTFT.C_GREEN);
	delay(TEST_DELAY2);
	myTFT.fillQuadrilateral(x4, y4, x5, y5, x6, y6, x7, y7, myTFT.C_GREEN);
	delay(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);

	myTFT.drawQuadrilateral(x8, y8, x9, y9, x10, y10, x11, y11, myTFT.C_YELLOW);
	delay(TEST_DELAY2);
	myTFT.fillQuadrilateral(x8, y8, x9, y9, x10, y10, x11, y11, myTFT.C_YELLOW);
	delay(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

void Test908(void) {
	// Test 908 : Ellipse
	myTFT.fillScreen(myTFT.C_BLACK);
	// Define ellipses at different positions
	// Format: drawEllipse(centerX, centerY, semiMajorAxis, semiMinorAxis, fill, color)
	myTFT.drawEllipse(TFT_WIDTH / 2,TFT_HEIGHT / 2, 20, 40, false, myTFT.C_WHITE);  // Top-left
	delay(TEST_DELAY5);
	myTFT.drawEllipse(TFT_WIDTH / 2,TFT_HEIGHT / 2, 20, 40, true, myTFT.C_WHITE);
	delay(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

void Test909(void) {
	//Test 909 : Drawing Arc: drawArc
	myTFT.fillScreen(myTFT.C_BLACK);
	int16_t centerX = 50;  // X-coordinate of the circle center
	int16_t centerY = 50;  // Y-coordinate of the circle center
	int16_t radius = 45;    // Radius of the circle
	Serial.println(myTFT.getArcAngleOffset());
	//drawArc(cx,  cy,  radius, thickness, startAngle, endAngle, color);
	// _arcAngleOffset is zero default means 0 is positive Axis , Arc drawn counterclockwise
	// XXX 270 XXX
	// 180 XXX 000
	// XXX 090 XXX
	// Draw the full lower half (0° to 180°)
	myTFT.drawArc(centerX, centerY, radius, 10, 0.0f, 180.0f, myTFT.C_GREEN);
	// Draw the full upper half (180° to 360°)
	myTFT.drawArc(centerX, centerY, radius, 10, 180.0f, 360.0f, myTFT.C_BLUE);
	delay(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
	// Draw the first quarter (0° to 90°)
	myTFT.drawArc(centerX, centerY, radius, 5, 0.0f, 90.0f, myTFT.C_RED);
	// Draw the second quarter (90° to 180°)
	myTFT.drawArc(centerX, centerY, radius, 5, 90.0f, 180.0f, myTFT.C_YELLOW);
	// Draw the third quarter (180° to 270°)
	myTFT.drawArc(centerX, centerY, radius, 5, 180.0f, 270.0f, myTFT.C_CYAN);
	// Draw the fourth quarter (270° to 360°)
	myTFT.drawArc(centerX, centerY, radius, 5, 270.0f, 360.0f, myTFT.C_MAGENTA);
	delay(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

void Test910(uint8_t revolutions)
{
	//Test 910 : Draw line at angle function
	myTFT.drawCircle(48, 48, 35, myTFT.C_GREEN);
	myTFT.drawCircle(48, 48, 34, myTFT.C_GREEN);
	for (uint8_t j =0 ; j< revolutions ; j++)
	{
		for (int i = 0; i <= 361; i++)
		{
			//drawLineAngle(x, y, angle, start,length, offset,  color);
			myTFT.drawLineAngle(48, 48, i-1, 0 , 33, -90, myTFT.C_BLACK);//erase last needle
			myTFT.drawLineAngle(48, 48, i, 0, 33, -90, myTFT.C_GREEN);//draw needle
			delay(15);
		}
	}
	myTFT.fillScreen(myTFT.C_BLACK);
}

#endif

/// @endcond
