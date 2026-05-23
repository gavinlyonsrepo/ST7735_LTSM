/*!
	@file   FUNCTIONS_FPS.ino
	@author Gavin Lyons
	@brief  Example file for st7735 driver, Arduino library ST7735_LTSM.
					Function testing, rotate , modes, scroll etc.
					Also FPS frame rate per second test.
	@note  See USER OPTIONS 1-3 in SETUP function
	@test
		-# Test501 Color
		-# Test502 scroll
		-# Test503 Rotate
		-# Test504 change modes test -> Invert, display on/off and Sleep.
		-# Test602 FPS frame rate per second test , text
		-# Test603 FPS frame rate per second test , graphics
*/

// libraries 
#include "ST7735_LTSM.hpp"
// Fonts used. 
#include "fonts_LTSM/FontDefault_LTSM.hpp" 
#include "fonts_LTSM/FontMega_LTSM.hpp"
#include "fonts_LTSM/FontPico_LTSM.hpp"
/// @cond

// Section :: Defines
//  Test timing related defines
#define TEST_DELAY1 1000  // mS
#define TEST_DELAY2 2000  // mS
#define TEST_DELAY5 5000  // mS
#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is defined. This example is not for that mode")
#endif

ST7735_LTSM myTFT;
bool bhardwareSPI = true;  // true for hardware spi, false for software

void setup(void) {
	Serial.begin(38400);
	delay(1000);
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
	myTFT.TFTInitPCBType(myTFT.TFT_ST7735R_Red);  // pass enum,multi choices,see README
	//**********************************************************
}

// MAIN loop
void loop(void) {
	Test501();
	Test502();
	Test503();
	Test504();
	Test602();
	Test603();
	EndTests();
}
// *** End OF MAIN **
// Function Space

void Test501(void) {
	myTFT.fillScreen(myTFT.C_BLACK);
	Serial.println("Test 501: Color Test:: Red,green,blue,yellow,white, black background");
	myTFT.fillRoundRect(8, 10, 24, 60, 8, myTFT.C_RED);
	myTFT.fillRoundRect(32, 10, 24, 60, 8, myTFT.C_GREEN);
	myTFT.fillRoundRect(56, 10, 24, 60, 8, myTFT.C_BLUE);
	myTFT.fillRoundRect(80, 10, 24, 60, 8, myTFT.C_YELLOW);
	myTFT.fillRoundRect(104, 10, 24, 60, 8, myTFT.C_WHITE);
	Serial.println("If colors are wrong user may have selected wrong PCB type in setup option 3");
	delay(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  Vertical Scroll test
*/
void Test502(void) {
	const uint8_t LINES = 10, LINE_SIZE = 10, LINE_OFFSET = 3, TOP_FIXED = 0, BOTTOM_FIXED = 0;
	char teststr1[] = "Scroll test";

	for (uint8_t i = 0; i < LINES; i++) {
		myTFT.writeCharString(0, LINE_OFFSET + i * LINE_SIZE, teststr1);
	}
	myTFT.setScrollDefinition(TOP_FIXED, BOTTOM_FIXED, 1);  // bottom-to-top
	uint8_t pos = LINE_OFFSET;
	for (uint8_t i = 0; i < LINES; i++) {
		for (uint8_t j = 0; j < LINE_SIZE; j++) {
			myTFT.TFTVerticalScroll(pos + TOP_FIXED);
			pos++;
			// check pos if necessary: must be < tftTFT_HEIGHT - TOP_FIXED - BOTTOM_FIXED
			delay(5);
		}
		delay(TEST_DELAY1);
	}
	myTFT.TFTchangeMode(myTFT.TFT_Normal_mode);
	myTFT.fillScreen(myTFT.C_BLACK);
}


/*!
	@brief  Rotate
*/
void Test503() {
	char teststr0[] = "Rotate 0";    // normal
	char teststr1[] = "Rotate 90";   // 90
	char teststr2[] = "Rotate 180";  // 180
	char teststr3[] = "Rotate 270";  // 270

	myTFT.fillScreen(myTFT.C_BLACK);

	myTFT.setRotation(myTFT.Degrees_0);
	myTFT.writeCharString(20, 20, teststr0);
	delay(TEST_DELAY2);

	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setRotation(myTFT.Degrees_90);
	myTFT.writeCharString(20, 20, teststr1);
	delay(TEST_DELAY2);

	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setRotation(myTFT.Degrees_180);
	myTFT.writeCharString(20, 20, teststr2);
	delay(TEST_DELAY2);

	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setRotation(myTFT.Degrees_270);
	myTFT.writeCharString(20, 20, teststr3);
	delay(TEST_DELAY2);

	myTFT.setRotation(myTFT.Degrees_0);
	myTFT.TFTchangeMode(myTFT.TFT_Normal_mode);
	myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  change modes test -> Invert, display on/off and Sleep.
*/
void Test504() {
	Serial.println("Test 504: Mode Tests");
	char teststr1[] = "Modes Test";
	myTFT.setTextColor(myTFT.C_RED, myTFT.C_YELLOW);
	myTFT.fillRoundRect(8, 10, 24, 60, 8, myTFT.C_RED);
	myTFT.fillRoundRect(32, 10, 24, 60, 8, myTFT.C_GREEN);
	myTFT.fillRoundRect(56, 10, 24, 60, 8, myTFT.C_BLUE);
	myTFT.fillRoundRect(80, 10, 24, 60, 8, myTFT.C_YELLOW);
	myTFT.fillRoundRect(104, 10, 24, 60, 8, myTFT.C_WHITE);
	myTFT.writeCharString(10, 80, teststr1);
	delay(TEST_DELAY5);
	myTFT.TFTchangeMode(myTFT.TFT_Normal_mode);
	delay(TEST_DELAY5);
	myTFT.TFTchangeMode(myTFT.TFT_Invert_mode);
	Serial.println("Test 504-2: Mode Invert");
	delay(TEST_DELAY5);
	myTFT.TFTchangeMode(myTFT.TFT_Normal_mode);
	Serial.println("Test 504-3: Mode Normal");
	delay(TEST_DELAY5);
	myTFT.TFTchangeMode(myTFT.TFT_Display_off_mode);
	Serial.println("Test 504-4: Mode Display off");
	delay(TEST_DELAY5);
	myTFT.TFTchangeMode(myTFT.TFT_Display_on_mode);
	Serial.println("Test 504-5: Mode Display on");
	delay(TEST_DELAY5);
	myTFT.TFTchangeMode(myTFT.TFT_Sleep_mode);
	Serial.println("Test 504-6: Mode Sleep on");
	delay(TEST_DELAY5);
	myTFT.TFTchangeMode(myTFT.TFT_Normal_mode);
	Serial.println("Test 504-7: Mode Normal");
	delay(TEST_DELAY2);
	myTFT.fillScreen(myTFT.C_BLACK);
}


/*!
	@brief   Frame rate per second test, FPS test
*/
void Test602(void) {
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
			if (count >= 1000) {
				Serial.print("FPS : ");
				Serial.println(fps);
				Serial.print("Seconds : ");
				Serial.println(seconds);
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
		//  ****
	}
}

/*!
	@brief   Frame rate per second test, FPS test. graphics
*/
void Test603(void) {
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setFont(FontMega);
	// Values to count frame rate per second
	long previousMillis = 0;
	long lastFramerate = 0;
	long currentFramerate = 0;
	uint16_t count = 0;
	uint16_t seconds = 0;
	uint16_t fps = 0;
	uint16_t shapeColor = 0x2222;
	while (1) {
		unsigned long currentMillis = millis();

		if (currentMillis - previousMillis >= 1000)  // every second
		{
			fps = currentFramerate - lastFramerate;
			lastFramerate = currentFramerate;
			previousMillis = currentMillis;
			seconds++;
			shapeColor = rand() % 60000;
			if (count >= 1000) {
				Serial.print("FPS : ");
				Serial.println(fps);
				Serial.print("Seconds : ");
				Serial.println(seconds);
				return;  // end if count gets to this
			}
		}
		currentFramerate++;
		count++;
		//  ** Code to test **
		// print fps
		myTFT.setCursor(5, 5);
		myTFT.print(fps);
		// Print some graphics adjusted for 128x128 screen
		myTFT.drawCircle(90, 50, 10, shapeColor);
		myTFT.drawRectWH(40, 40, 20, 20, shapeColor << 1);
		myTFT.drawRoundRect(5, 40, 20, 20, 2, shapeColor >> 1);
		myTFT.fillCircle(90, 90, 10, shapeColor);
		myTFT.fillRect(40, 90, 20, 20, shapeColor << 1);
		myTFT.fillRoundRect(5, 90, 20, 20, 2, shapeColor >> 1);
		myTFT.fillTriangle(30, 100, 64, 70, 98, 100, shapeColor - 100);
		myTFT.fillTriangle(30, 100, 64, 120, 98, 120, shapeColor);
		//  ****
	}
}

/*!
	@brief  Stop testing and shutdown the TFT
*/
void EndTests(void) {
	myTFT.setFont(FontDefault);
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
	myTFT.setCursor(15, 15);
	myTFT.print("tests over");
	delay(TEST_DELAY5);
	myTFT.TFTPowerDown();
	Serial.println("Tests Over");
	while (1) {};
}

// *************** EOF ****************

/// @endcond
