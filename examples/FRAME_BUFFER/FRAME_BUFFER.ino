/*!
	@file   FRAME_BUFFER.ino
	@author Gavin Lyons
	@brief  Example file for st7735 library. Arduino library ST7735_LTSM. Tests frame buffer mode
	@note   See USER OPTIONS 1-3 in SETUP function
					dislib16_ADVANCED_SCREEN_BUFFER_ENABLE must be enabled for this example
					or it will not compile , uses vector , MCU cores which do not support vector 
					will not compile( UNO R3)
	@test
		-# Test 1200 Frame buffer mode.
*/

// libraries
#include "ST7735_LTSM.hpp"
// Fonts used.
#include "fonts_LTSM/FontDefault_LTSM.hpp"
#include "fonts_LTSM/FontPico_LTSM.hpp"
// bitmap test data ~53K
#include "bitmap_test_data_LTSM/Bitmap_TEST_Data_16color1.hpp"
#include "bitmap_test_data_LTSM/Bitmap_TEST_Data_16color2.hpp"
#include "bitmap_test_data_LTSM/Bitmap_TEST_Data_8color.hpp"
#include "bitmap_test_data_LTSM/Bitmap_TEST_Data_bicolor.hpp"
/// @cond

#ifndef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is not defined. it is required for this example")
#endif

ST7735_LTSM myTFT;
bool bhardwareSPI = true;  // true for hardware spi, false for software

void setup(void) {
	Serial.begin(38400);
	delay(1000);
	Serial.println("Start");
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
	uint16_t TFT_WIDTH = 128;   // Screen width in pixels
	uint16_t TFT_HEIGHT = 128;  // Screen height in pixels
	uint8_t OFFSET_COL = 0;     // 2, These offsets can be adjusted for any issues->
	uint8_t OFFSET_ROW = 0;     // 3, with screen manufacture tolerance/defects
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW, TFT_WIDTH, TFT_HEIGHT);
	// ******************************************
	// ******** USER OPTION 3 PCB_TYPE  **************************
	myTFT.TFTInitPCBType(myTFT.TFT_ST7735R_Red);  // pass enum,4 choices,see README
	//**********************************************************
	if (myTFT.setBuffer() != DisLib16::Success) {
		Serial.println("Set buffer failed");
		while (1) {};
	}
	//  === important in Frame buffer mode  ===
	myTFT.setTextCharPixelOrBuffer(true);  // set to use pixel mode for text
	// ====
}


// MAIN loop
void loop(void) {
	Test1();
	TestRotate();
	TestBitmap();
	TestBitmapFPS();
	TestTextFPS();
	EndTests();
}
// *** End OF MAIN **

void EndTests(void) {
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.clearBuffer(myTFT.C_BLACK);
	myTFT.destroyBuffer();
	myTFT.TFTPowerDown();
	Serial.println("Tests Over");
	while (1) {};
}

void ScreenReset(void) {
	myTFT.writeBuffer();
	delay(2500);
	myTFT.clearBuffer(myTFT.C_BLACK);
}

void Test1(void) {
	Serial.println("Test1");
	myTFT.clearBuffer(myTFT.C_RED);
	myTFT.writeBuffer();
	delay(1000);
	myTFT.clearBuffer(myTFT.C_GREEN);
	myTFT.writeBuffer();
	delay(1000);
	myTFT.clearBuffer(myTFT.C_BLUE);
	myTFT.writeBuffer();
	delay(1000);

	myTFT.clearBuffer(myTFT.C_BLACK);
	myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
	myTFT.setCursor(5, 5);
	myTFT.setFont(FontDefault);
	myTFT.print("Buffer Mode     Test");
	myTFT.writeBuffer();
	delay(5000);
}

void TestRotate(void) {
	Serial.println("Rotate");
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.clearBuffer(myTFT.C_BLACK);
	const char *rotationLabels[] = { "Rotate 0", "Rotate 90", "Rotate 180", "Rotate 270" };
	const decltype(myTFT.Degrees_0) rotations[] = {
		myTFT.Degrees_0,
		myTFT.Degrees_90,
		myTFT.Degrees_180,
		myTFT.Degrees_270
	};
	for (int i = 0; i < 4; ++i) {
		myTFT.setRotation(rotations[i]);
		myTFT.setCursor(20, 20);
		myTFT.print(rotationLabels[i]);
		myTFT.setCursor(20, 100);
		myTFT.print("Bottom");
		ScreenReset();
	}
	myTFT.setRotation(myTFT.Degrees_0);
	myTFT.TFTchangeMode(myTFT.TFT_Normal_mode);
	ScreenReset();
}

void TestBitmap(void) {
	Serial.println("Sprite");
	// Sprite
	for (int col = 0; col < 2; ++col)  // 0: no flip, 1: flip
	{
		bool flip = (col == 1);
		int x = col * 32;
		for (int row = 0; row < 3; ++row) {
			int y = row * 32;
			myTFT.drawSpriteData(x, y, SpriteTest16, 32, 32, myTFT.C_LBLUE, flip);
		}
	}
	ScreenReset();
	// Bitmap 1-bit
	Serial.println("Bitmap 1-bit");
	myTFT.drawBitmap(0, 0, 128, 128, myTFT.C_WHITE, myTFT.C_GREEN, ArrowImage);
	ScreenReset();
	// Bitmap 8-bit
	Serial.println("Bitmap 8-bit");
	myTFT.drawBitmap8Data(20, 20, ColorTestImage, 96, 96);
	ScreenReset();
	// Bitmap 16-bit
	Serial.println("Bitmap 16-bit");
	myTFT.drawBitmap16Data(0, 0, MotorImage, 128, 128);
	ScreenReset();
}

void TestBitmapFPS(void) {
	Serial.println("Bitmap FPS");
	// Values to count frame rate per second
	long previousMillis = 0;
	long lastFramerate = 0;
	long currentFramerate = 0;

	uint16_t count = 0;
	uint16_t seconds = 0;
	uint16_t fps = 0;

	char teststr1[] = "Test bitmap FPS, Output Results to USB Serial port";
	myTFT.writeCharString(5, 5, teststr1);
	delay(5000);
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
		myTFT.setCursor(0, 0);
		myTFT.print(fps);
		myTFT.writeBuffer();
		//   **
	}  // end of while
	// Report results to usb
	Serial.print("Seconds : ");
	Serial.println(seconds);
	Serial.print("Count : ");
	Serial.println(count);
	Serial.print("FPS : ");
	Serial.println(fps);

	// Print to screen
	myTFT.clearBuffer(myTFT.C_BLACK);
	myTFT.setCursor(0, 0);
	myTFT.print("Seconds : ");
	myTFT.println(seconds);
	myTFT.print("Count : ");
	myTFT.println(count);
	myTFT.print("FPS : ");
	myTFT.print(fps);
	myTFT.writeBuffer();
	delay(5000);
	myTFT.clearBuffer(myTFT.C_BLACK);
	myTFT.fillScreen(myTFT.C_BLACK);
}

void TestTextFPS(void) {
	Serial.println("Text FPS");
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setFont(FontPico);
	// Values to count frame rate per second
	long previousMillis = 0;
	long lastFramerate = 0;
	long currentFramerate = 0;
	uint16_t count = 0;
	uint16_t seconds = 0;
	uint16_t fps = 0;
	while (1) {
		unsigned long currentMillis = millis();

		if (currentMillis - previousMillis >= 1000)  // every second
		{
			fps = currentFramerate - lastFramerate;
			lastFramerate = currentFramerate;
			previousMillis = currentMillis;
			seconds++;
			//myTFT.clearBuffer(myTFT.C_BLACK);
			if (count >= 1000) {
				Serial.print("Seconds : ");
				Serial.println(seconds);
				Serial.print("Count : ");
				Serial.println(count);
				Serial.print("FPS : ");
				Serial.println(fps);
				return;  // end if count gets to this
			}
		}
		currentFramerate++;
		count++;
		//  ** Code to test **
		// print seconds
		myTFT.setCursor(5, 5);
		myTFT.println("Secs:");
		myTFT.println(seconds);
		// print fps
		myTFT.println("FPS:");
		myTFT.println(fps);
		// print count
		myTFT.println("Count:");
		myTFT.println(count);
		myTFT.println("Testing!");
		myTFT.println("testing");
		myTFT.writeBuffer();
		//  ****
	}
}
/// @endcond
