/*!
	@file TEXT.ino
	@brief Library test file, st7735, tests text & fonts.
	@author Gavin Lyons.
	@note See USER OPTIONS 1-3 in SETUP function, 
	@test
	-# Test 701 print out  fonts
	-# Test 702 defined 16-bit Colors, text
	-# Test 703 print entire ASCII font 32 to 127, default font
	-# Test 704 font invert + test character draw using draw functions
	-# Test 705 print method all fonts
	-# Test 706 Misc print class tests (println invert, wrap, base nums etc)
	-# Test 707 Misc draw functions (Invert, wrap)
*/

// libraries 
#include "ST7735_LTSM.hpp"
// Included Fonts 
#include <fonts_LTSM/FontArialBold_LTSM.hpp>
#include <fonts_LTSM/FontArialRound_LTSM.hpp>
#include <fonts_LTSM/FontDefault_LTSM.hpp>
#include <fonts_LTSM/FontGll_LTSM.hpp>
#include <fonts_LTSM/FontGroTeskBig_LTSM.hpp>
#include <fonts_LTSM/FontGroTesk_LTSM.hpp>
#include <fonts_LTSM/FontHallfetica_LTSM.hpp>
#include <fonts_LTSM/FontInconsola_LTSM.hpp>
#include <fonts_LTSM/FontMega_LTSM.hpp>
#include <fonts_LTSM/FontMint_LTSM.hpp>
#include <fonts_LTSM/FontOrla_LTSM.hpp>
#include <fonts_LTSM/FontPico_LTSM.hpp>
#include <fonts_LTSM/FontRetro_LTSM.hpp>
#include <fonts_LTSM/FontSevenSeg_LTSM.hpp>
#include <fonts_LTSM/FontSinclairS_LTSM.hpp>
#include <fonts_LTSM/FontSixteenSeg_LTSM.hpp>

/// @cond
#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is defined. This example is not for that mode")
#endif
//  Test timing related defines
#define TEST_DELAY1 1000	  // mS
#define TEST_DELAY2 2000	  // mS
#define TEST_DELAY5 5000	  // mS

ST7735_LTSM myTFT;
bool bhardwareSPI = true; // true for hardware spi, false for software

void setup(void)
{
	Serial.begin(38400); 
	delay(1000);
	Serial.println("TFT Start");
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
	uint16_t TFT_WIDTH = 128;// Screen width in pixels
	uint16_t TFT_HEIGHT = 128; // Screen height in pixels
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW , TFT_WIDTH , TFT_HEIGHT);
// ******************************************
// ******** USER OPTION 3 PCB_TYPE  **************************
	myTFT.TFTInitPCBType(myTFT.TFT_ST7735R_Red); // pass enum,multi choices,see README
//**********************************************************
}
// MAIN loop
void loop(void)
{
	Test701();
	Test702();
	Test703();
	Test704();
	Test705();
	Test706();
	Test707();
	EndTests();
}
// *** End OF MAIN **

//  Function Space

void Test701(void) {
	// Test 701: Print out some fonts with writeCharString
	myTFT.fillScreen(myTFT.C_BLACK);
	
	struct FontText {
		const char *text;
		const uint8_t * font;
		int x, y;
	};
	
	// First set of fonts
	FontText fontTests1[] = {
		{"Default", FontDefault, 5, 5}, {"GLL", FontGll, 5, 15}, {"Pico", FontPico, 5, 30},
		{"Sinclair", FontSinclairS, 5, 45}, {"Orla", FontOrla, 5, 65}, {"Retro", FontRetro, 5, 95},
		{"Mega", FontMega, 5, 2}, {"Arial b", FontArialBold, 5, 30}, {"Hall", FontHallfetica, 5, 60},
		{"Arial R", FontArialRound, 5, 90}, {"GroTesk", FontGroTesk, 5, 5}
	};
	// Loop for first set
	size_t testCount1 = sizeof(fontTests1) / sizeof(FontText);
	for (size_t i = 0; i < testCount1; ++i) {
		myTFT.setFont(fontTests1[i].font);
		myTFT.writeCharString(fontTests1[i].x, fontTests1[i].y, const_cast<char*>(fontTests1[i].text));
		if (i == 5 || i == 9) {
			DisplayReset();
		}
	}
	DisplayReset();
	// Second set of fonts
	FontText fontTests2[] = {
		{"16", FontSixteenSeg, 5, 5}, {"7", FontSevenSeg, 5, 5}, {"inco", FontInconsola, 5, 5},
		{"GB", FontGroTeskBig, 5, 5}, {"Mint", FontMint, 5, 5}
	};
	// Loop for second set
	size_t testCount2 = sizeof(fontTests2) / sizeof(FontText);
	for (size_t i = 0; i < testCount2; ++i) {
		myTFT.setFont(fontTests2[i].font);
		myTFT.writeCharString(fontTests2[i].x, fontTests1[i].y, const_cast<char*>(fontTests2[i].text));
		DisplayReset();
	}
}

void Test702(void) {
	// Test 702: Some of the Defined 16-bit Colors for text
	myTFT.setFont(FontDefault);
	struct ColorText {
		const char *text;
		uint16_t color;
	};
	
	ColorText colorTexts[] = {
		{"WHITE", myTFT.C_WHITE}, {"BLUE", myTFT.C_BLUE}, {"RED", myTFT.C_RED},
		{"GREEN", myTFT.C_GREEN}, {"CYAN", myTFT.C_CYAN}, {"MAGENTA", myTFT.C_MAGENTA},
		{"YELLOW", myTFT.C_YELLOW}, {"GREY", myTFT.C_GREY}, {"TAN", myTFT.C_TAN},
		{"BROWN", myTFT.C_BROWN}, {"ORANGE", myTFT.C_ORANGE}, {"DGREEN", myTFT.C_DGREEN}
	};
	
	for (size_t i = 0; i < 12; ++i) {
		int x = (i < 6) ? 5 : 60;  // First 6 on the left, next 6 on the right
		int y = 5 + (i % 6) * 10;  // Vertical spacing
		myTFT.setTextColor(colorTexts[i].color, myTFT.C_BLACK);
		myTFT.writeCharString(x, y, const_cast<char*>(colorTexts[i].text));
	}
	
	DisplayReset();
	
}

void Test703(void) {
	// Test 703: Print entire ASCII font 32 to 127, default font
	uint8_t row = 5;
	uint8_t col = 5;
	myTFT.setFont(FontDefault);
	myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
	for (char i = 32; i < 127; i++) {

		myTFT.writeChar(col, row, i);
		col += 10;
		if (col > 115) {
			row += 10;
			col = 5;
		}
	}
	DisplayReset();
}



void Test704(void) {
	// Test 704: font invert + test character draw using draw functions
	myTFT.setTextColor(myTFT.C_RED, myTFT.C_YELLOW);
	myTFT.setFont(FontDefault);
	char teststr1[] = "TEST";
	myTFT.writeCharString(15, 5, teststr1);
	myTFT.setInvertFont(true);
	myTFT.writeCharString(15, 15, teststr1); //inverted yellow on red
	myTFT.setInvertFont(false);
	myTFT.writeCharString(15, 25, teststr1);
	myTFT.writeChar(0, 85, 'G');
	myTFT.writeChar(25, 85, 'L');
	DisplayReset();
}


void Test705(void)
{
	// Test 705: Print class methods

	// Test Fonts default +  + pico+ sinclair + retro
	myTFT.setTextColor(myTFT.C_WHITE, myTFT.C_BLACK);

	myTFT.setCursor(5,5);
	myTFT.setFont(FontDefault);
	myTFT.print("Default ");
	myTFT.print(-43);

	myTFT.setCursor(5,15);
	myTFT.setFont(FontGll);
	myTFT.print("GLL ");
	myTFT.print(123.284,1); // print 123.3

	myTFT.setCursor(5,25);
	myTFT.setFont(FontPico);
	myTFT.print("pico ");
	myTFT.print(747);

	myTFT.setCursor(5,65);
	myTFT.setFont(FontSinclairS);
	myTFT.print("sinclair ");
	myTFT.print(456);

	myTFT.setCursor(5,85);
	myTFT.setFont(FontRetro);
	myTFT.print("retro  ");
	myTFT.print(-3.14);
	DisplayReset();

	// Test font  mega Arial bold and Hallf.
	myTFT.setCursor(5,5);
	myTFT.setFont(FontMega);
	myTFT.println("mega ");
	myTFT.print(61);

	myTFT.setCursor(5,40);
	myTFT.setFont(FontArialBold);
	myTFT.println("A bold");
	myTFT.print(12.08);

	myTFT.setCursor(5,80);
	myTFT.setFont(FontHallfetica);
	myTFT.println("hall f");
	myTFT.print(1.48);
	DisplayReset();

	// Test Font orla + arial round
	myTFT.setFont(FontOrla);
	myTFT.setCursor(5,5);
	myTFT.println(-7.16);
	myTFT.print("Orla");

	myTFT.setCursor(5,70);
	myTFT.setFont(FontArialRound);
	myTFT.println(-8.16);
	myTFT.print("a rnd");
	DisplayReset();

	// Test font grotesk + sixteen segment
	myTFT.setCursor(5,5);
	myTFT.setFont(FontGroTesk);
	myTFT.println("GROTESK");
	myTFT.print(1.78);
	DisplayReset();

	myTFT.setCursor(5,5);
	myTFT.setFont(FontSixteenSeg);
	myTFT.print(124);
	DisplayReset();
	
	// Test font seven segment 
	myTFT.setCursor(5,5);
	myTFT.setFont(FontSevenSeg);
	myTFT.println(12);
	DisplayReset();

	// Test font mint
	myTFT.setFont(FontMint);
	myTFT.println("MINT");;
	myTFT.print("9`C");
	DisplayReset();
}

void Test706(void)
{
	// Test 706: Misc print class(string object, println invert, wrap, base nums etc)
	//Inverted print fonts
	myTFT.setTextColor(myTFT.C_RED, myTFT.C_YELLOW);
	myTFT.setFont(FontDefault);

	myTFT.setFont(FontSinclairS);
	myTFT.setCursor(5,15);
	myTFT.print(-49);

	myTFT.setFont(FontDefault);
	myTFT.setCursor(5,35);
	myTFT.print(112.09);
	myTFT.setCursor(5,55);
	myTFT.print("ABCD");

	myTFT.setFont(FontGll);
	myTFT.setCursor(5,85);
	myTFT.print("ER");

	DisplayReset();

	// Inverted print fonts
	myTFT.setTextColor(myTFT.C_YELLOW, myTFT.C_RED);

	myTFT.setFont(FontArialBold);
	myTFT.setCursor(5,5);
	myTFT.print("INVERT");
	myTFT.setCursor(5,25);
	myTFT.print(-94.982, 2);

	myTFT.setFont(FontHallfetica);
	myTFT.setCursor(5,65);
	myTFT.print("INVERT");
	myTFT.setCursor(5,85);
	myTFT.print(123456);
	DisplayReset();

	// Test print with DEC BIN OCT HEX
	myTFT.setTextColor(myTFT.C_WHITE, myTFT.C_BLACK);
	uint8_t numPos = 47;
	myTFT.setFont(FontPico);
	myTFT.setCursor(5,5);
	myTFT.print(numPos , DEC); // 47
	myTFT.setCursor(5,25);
	myTFT.print(numPos , BIN); // 10111
	myTFT.setCursor(5,45);
	myTFT.print(numPos , OCT); // 57
	myTFT.setCursor(5,65);
	myTFT.print(numPos , HEX); // 2F

	DisplayReset();

	//text wrap with print
	myTFT.setCursor(5,70);
	myTFT.print("12345678901234567890ABCDEFGHIJ");
	DisplayReset();
}


void Test707(void)
{
	// Test 707: Misc draw functions tests wrap,  etc

	//wrap
	char teststr0[] = "123456789";

	myTFT.setFont(FontDefault);
	myTFT.writeCharString(80, 5, teststr0);
	myTFT.setFont(FontMega);
	myTFT.writeCharString(80, 60, teststr0);
	DisplayReset();

	//invert + Numeric fonts  sprintf
	int myInt=931;
	char myStr[5];
	myTFT.setFont(FontSinclairS);
	sprintf(myStr, "%d", myInt);
	myTFT.writeCharString(5, 5, myStr);

	myTFT.setFont(FontRetro);
	float  myPI = 3.171592;
	char myStr2[8];
	sprintf(myStr2, "%0.3f", myPI); // 3.172
	myTFT.writeCharString(5, 65, myStr2);

	DisplayReset();

	//invert + Numeric
	char teststr1[] = "12:81";
	char teststr2[] = "72:83";
	char teststr3[] = "9 rnd";
	char teststr4[] = "10 bold";

	myTFT.setFont(FontMega);
	myTFT.setTextColor(myTFT.C_RED, myTFT.C_BLACK);
	myTFT.writeChar(2, 2, '7');
	myTFT.setTextColor(myTFT.C_YELLOW, myTFT.C_BLACK);
	myTFT.writeCharString(2, 40, teststr1);

	myTFT.setTextColor(myTFT.C_WHITE, myTFT.C_BLACK);
	myTFT.writeChar(2, 75, '8');
	myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_RED);
	myTFT.writeCharString(2, 100, teststr2);

	DisplayReset();

	myTFT.setFont(FontArialRound);
	myTFT.setTextColor(myTFT.C_RED, myTFT.C_BLACK);
	myTFT.writeChar(2, 2, '9');
	myTFT.setTextColor(myTFT.C_YELLOW, myTFT.C_BLACK);
	myTFT.writeCharString(2, 26, teststr3);

	myTFT.setFont(FontArialBold);
	myTFT.setTextColor(myTFT.C_WHITE, myTFT.C_BLACK);
	myTFT.writeChar(2, 56, 'A');
	myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_RED);
	myTFT.writeCharString(2, 90, teststr4);

	DisplayReset();
}


void DisplayReset(void)
{
	delay(TEST_DELAY5);
	myTFT.fillScreen(myTFT.C_BLACK);
}

/*!
	@brief  Stop testing and shutdown the TFT
*/
void EndTests(void)
{
	char teststr1[] = "Tests over";
	myTFT.setFont(FontDefault);
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
	myTFT.writeCharString(5, 50, teststr1);
	delay(TEST_DELAY5);
	myTFT.TFTPowerDown();
	Serial.println("TFT :: Tests Over");
  while(1){};
}

/// @endcond
