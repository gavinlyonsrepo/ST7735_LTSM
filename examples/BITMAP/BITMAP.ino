/*!
	@file   BITMAP.ino
	@author Gavin Lyons
	@brief  Example ino file for st7735 bitmap tests + FPS bitmap test.
	@details Arduino library ST7735_LTSM. See USER OPTIONS 1-3 in SETUP function  
	@test
		-# Test 300 Sprite 
		-# Test 301 icons
		-# Test 302 bi-color small image
		-# Test 303 bi-color full screen image 128x128
		-# Test 304 16 bit color image from a data array
		-# Test 305 16 bit color image data from a data array
		-# Test 306 8 bit color image data from a data array
		-# Test 601 FPS bitmap results to serial port
*/

// libraries
#include "ST7735_LTSM.hpp"
// Depends
#include "fonts_LTSM/FontDefault_LTSM.hpp"
// bitmap test data ~53K
#include "bitmap_test_data_LTSM/Bitmap_TEST_Data_16color1.hpp"
#include "bitmap_test_data_LTSM/Bitmap_TEST_Data_16color2.hpp"
#include "bitmap_test_data_LTSM/Bitmap_TEST_Data_8color.hpp"
#include "bitmap_test_data_LTSM/Bitmap_TEST_Data_bicolor.hpp"

/// @cond

// Section :: Defines
//  Test timing related defines
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000
#define CLOCK_DISPLAY_TIME 20  // seconds
#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is defined. This example is not for that mode")
#endif

ST7735_LTSM myTFT;
bool bhardwareSPI = true;  // true for hardware spi, false for software

void setup(void) {
	Serial.begin(38400);
	delay(1000);
	Serial.println("TFT Start");
	//*************** USER OPTION 1 SPI_SPEED + TYPE ***********
	int8_t DC_TFT = 5;
	int8_t RST_TFT = 4;
	int8_t CS_TFT = 15;
	if (bhardwareSPI == true) {       // hw spi
		uint32_t TFT_SCLK_FREQ = 8000000;  // Spi freq in Hertz
		myTFT.setupGPIO_SPI(TFT_SCLK_FREQ, RST_TFT, DC_TFT, CS_TFT);
	} else {                        // sw spi
		uint16_t SWSPICommDelay = 0;  // optional SW SPI GPIO delay in uS
		int8_t SDIN_TFT = 13;
		int8_t SCLK_TFT = 12;
		myTFT.setupGPIO_SPI(SWSPICommDelay, RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
	}
	//********************************************************
	// ****** USER OPTION 2 Screen Setup ******
	uint8_t OFFSET_COL = 0;     // 2, These offsets can be adjusted for any issues->
	uint8_t OFFSET_ROW = 0;     // 3, with screen manufacture tolerance/defects
	uint16_t TFT_WIDTH = 128;   // Screen width in pixels
	uint16_t TFT_HEIGHT = 128;  // Screen height in pixels
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW, TFT_WIDTH, TFT_HEIGHT);
	// ******************************************
	// ******** USER OPTION 3 PCB_TYPE  **************************
	myTFT.TFTInitPCBType(myTFT.TFT_ST7735R_Red);  // pass enum,4 choices,see README
	//**********************************************************
}

// MAIN
void loop(void) {
	Test300();
	Test301();
	Test302();
	Test303();
	Test304();
	Test305();
	Test306();
	Test601();
	EndTests();
}
// End OF MAIN 

// Function Space
/*!
	@brief  "sprite demo" 32 x 32 with LBLUE background
*/
void Test300(void) {
	// Test 300-A test 16-bit color Sprite
	// Draw as sprite, without background , 32 X 32 .background color = ST7375_LBLUE
	// Green bakcground screen
	myTFT.fillScreen(myTFT.C_GREEN);
	delay(TEST_DELAY2);
	myTFT.drawSpriteData(5, 5, SpriteTest16, 32, 32, myTFT.C_LBLUE, false);
	myTFT.drawSpriteData(40, 40, SpriteTest16, 32, 32, myTFT.C_LBLUE, true);
	delay(TEST_DELAY5);

	// Test 300-B test 16-bit color Sprite
	// Draw as sprite, without background , 32 X 32 .background color = ST7375_LBLUE
	// Bitmap background screen
	myTFT.drawBitmap16Data(20, 20, PosterImage, 80, 48);
	delay(TEST_DELAY5);

	myTFT.drawSpriteData(60, 10, SpriteTest16, 32, 32, myTFT.C_LBLUE, false);
	myTFT.drawSpriteData(60, 60, SpriteTest16, 32, 32, myTFT.C_LBLUE, false);
	delay(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  small 1-bit bitmap's
*/
void Test301(void) {
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.drawBitmap(2, 2, 16, 8, myTFT.C_BLACK, myTFT.C_WHITE, SignalIcon);
	myTFT.drawBitmap(20, 2, 16, 8, myTFT.C_BLACK, myTFT.C_WHITE, MsgIcon);
	myTFT.drawBitmap(40, 2, 8, 8, myTFT.C_BLACK, myTFT.C_WHITE, AlarmIcon);
	myTFT.drawBitmap(80, 2, 16, 8, myTFT.C_BLACK, myTFT.C_WHITE, BatIcon);
	myTFT.drawBitmap(5, 20, 16, 8, myTFT.C_GREEN, myTFT.C_BLACK, PowerIcon);
	myTFT.drawBitmap(30, 20, 16, 8, myTFT.C_RED, myTFT.C_YELLOW, SpeedIcon);
	myTFT.setCursor(5, 40);
	myTFT.print("Test 301");

	delay(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}  // end of Test 301

/*!
	@brief  Test 302 bi-color small image 20x24
*/
void Test302(void) {

	myTFT.fillScreen(myTFT.C_BLACK);
	char teststr1[] = "Test 302";
	myTFT.writeCharString(5, 5, teststr1);
	myTFT.drawBitmap(80, 20, 40, 16, myTFT.C_CYAN, myTFT.C_BLACK, SunTextImage);
	myTFT.drawBitmap(20, 40, 40, 16, myTFT.C_RED, myTFT.C_BLACK, SunTextImage);
	myTFT.drawBitmap(30, 70, 40, 16, myTFT.C_YELLOW, myTFT.C_RED, SunTextImage);
	delay(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  Test303 bi-color full screen image 128x128
*/
void Test303(void) {
	char teststr1[] = "Test 303";
	myTFT.writeCharString(5, 5, teststr1);
	delay(TEST_DELAY2);

	myTFT.drawBitmap(0, 0, 128, 128, myTFT.C_WHITE, myTFT.C_GREEN, ArrowImage);
	delay(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  Test304 16 bit color image from a data array
*/
void Test304(void) {
	char teststr1[] = "Test 304";
	myTFT.writeCharString(5, 5, teststr1);
	delay(TEST_DELAY5);

	myTFT.drawBitmap16Data(0, 0, MotorImage, 128, 128);
	delay(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  Test305 16 bit color image data from a data array
*/
void Test305(void) {
	char teststr1[] = "Test 305";
	myTFT.fillScreen(myTFT.C_WHITE);
	myTFT.writeCharString(5, 5, teststr1);
	delay(TEST_DELAY5);

	myTFT.drawBitmap16Data(20, 20, PosterImage, 80, 48);
	delay(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  Test306 8 bit color image data from a data array
*/
void Test306(void) {
	char teststr1[] = "Test 306";
	myTFT.writeCharString(5, 5, teststr1);
	delay(TEST_DELAY2);
	myTFT.drawBitmap8Data(20, 20, ColorTestImage, 96, 96);
	delay(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  Test601 frame rate per second FPS ,results to serial port
*/
void Test601(void) {
	// Values to count frame rate per second
	long previousMillis = 0;
	long lastFramerate = 0;
	long currentFramerate = 0;
	uint16_t count = 0;
	uint16_t seconds = 0;
	uint16_t fps = 0;
	char teststr1[] = "Test 601 FPS, Output Results to USB Serial port";
	myTFT.writeCharString(5, 5, teststr1);
	delay(TEST_DELAY5);
	myTFT.setTextColor(myTFT.C_YELLOW, myTFT.C_RED);

	while (1) {
		unsigned long currentMillis = millis();
		if (currentMillis - previousMillis >= 1000)  // every second
		{
			fps = currentFramerate - lastFramerate;
			lastFramerate = currentFramerate;
			previousMillis = currentMillis;
			seconds++;
			if (seconds == 20)
				break;
		}
		currentFramerate++;
		count++;

		//  ** Code to test **
		myTFT.drawBitmap16Data(0, 0, MotorImage, 128, 128);
		myTFT.setCursor(5, 5);
		myTFT.print(fps);
		//   **
	}  // end of while

	// Report results to usb
	Serial.print("Seconds :: ");
	Serial.println(seconds);
	Serial.print("Count :: ");
	Serial.println(count);
	Serial.print("FPS :: ");
	Serial.println(fps);
	// Print to screen
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setCursor(0, 0);
	myTFT.print("Seconds : ");
	myTFT.println(seconds);
	myTFT.print("Count : ");
	myTFT.println(count);
	myTFT.print("FPS : ");
	myTFT.print(fps);
	delay(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}


/*!
	@brief  Stop testing and shutdown the TFT
*/
void EndTests(void) {
	char teststr1[] = "Tests over";
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.writeCharString(15, 15, teststr1);
	delay(TEST_DELAY5);
	myTFT.TFTPowerDown();
	Serial.println("TFT Tests Over");
	while (1) {};
}
// *************** EOF ****************

/// @endcond
