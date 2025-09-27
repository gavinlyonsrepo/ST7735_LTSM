/*!
	@file    DEMOS_TWO.ino
	@author  Gavin Lyons
	@brief   Example file for st7735 library.  ST7735_LTSM library. demo
	@details See USER OPTIONS 1-3 in SETUP function
	@note  
	@test
    -# Test 409 demo volt meter with gradient colour bar
*/

// libraries
#include "ST7735_LTSM.hpp"
#include "fonts_LTSM/FontDefault_LTSM.hpp"
#include "fonts_LTSM/FontOrla_LTSM.hpp"
/// @cond

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
  if (bhardwareSPI == true) {          // hw spi
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
  Serial.println("Start demo 2");
}


// MAIN loop
void loop(void) {
  Demo(50);
  EndTests();
}
// *** End OF MAIN **

//  Function Space

void EndTests(void) {
  myTFT.TFTPowerDown();
  Serial.println("Demo Over");
  while (1) {};
}

void Demo(uint16_t countLimit) {
  myTFT.setRotation(myTFT.Degrees_90);  // or 3 depending on your orientation
  myTFT.fillScreen(myTFT.C_BLACK);
  // --- Static elements ---
  myTFT.setTextColor(myTFT.C_YELLOW, myTFT.C_BLACK);
  myTFT.setFont(FontDefault);
  char titleArray[] = "Voltmeter";
  myTFT.writeCharString(20, 5, titleArray);
  // Bargraph outline
  int barX = 20;
  int barY = 70;
  int barWidth = 100;
  int barHeight = 15;
  myTFT.drawRectWH(barX, barY, barWidth, barHeight, myTFT.C_WHITE);
  float voltage = 9.0f;  // starting voltage (mid-range)
  const float minVoltage = 0.0f;
  const float maxVoltage = 15.0f;
  // count info
  uint16_t count = 0;
  Serial.print("countlimit = ");
  Serial.println(countLimit);
  while (count++ < countLimit) {
    int step = random(-10, 11);
    voltage += step / 10.0f;
    // Constrain to valid voltage range
    if (voltage < minVoltage) voltage = minVoltage;
    if (voltage > maxVoltage) voltage = maxVoltage;
    // Format string
    char buffer[10];
    snprintf(buffer, sizeof(buffer), "%04.1f", voltage);
    // Clear only the numeric area (enough for 4 chars × 16 wide, 24 tall)
    myTFT.fillRect(20, 30, 64, 24, myTFT.C_BLACK);
    // Large text display
    myTFT.setFont(FontOrla);
    myTFT.setTextColor(myTFT.C_CYAN, myTFT.C_BLACK);
    myTFT.writeCharString(20, 30, buffer);
    // Update Bargraph
    myTFT.fillRect(barX + 1, barY + 1, barWidth - 2, barHeight - 2, myTFT.C_BLACK);
    int filled = (int)((voltage / 15.0) * (barWidth - 2));
    for (int i = 0; i < filled; i++) {
      // Map column index to gradient range (0–127)
      uint8_t val = (i * 127) / (barWidth - 2);
      uint16_t color = generateColor(val);
      // Draw a 1-pixel wide vertical strip
      myTFT.fillRect(barX + 1 + i, barY + 1, 1, barHeight - 2, color);
    }

    delay(1000);
  }
}

/*!
	@brief This function is designed to return a color in the 16-bit 
		RGB format based on the input value, creating a smooth transition 
		between colors in different stages. 
	@details The gradient is calculated by 
		cycling through different color transitions as val increases, 
		allowing for smooth visual effects such as changing hues 
		or creating rainbow-like effects on an LCD. 1-127 will
		This will create a gradient color between blue and red based on the value.
*/
uint16_t generateColor(uint8_t value) {
  // Clamp to 0–127
  value = (value > 127) ? 127 : value;
  uint8_t red = 0;
  uint8_t green = 0;
  uint8_t blue = 0;
  uint8_t segment = value / 32;
  switch (segment) {
    case 0:
      red = 0;
      green = 2 * (value % 32);
      blue = 31;
      break;
    case 1:
      red = 0;
      green = 63;
      blue = 31 - (value % 32);
      break;
    case 2:
      red = value % 32;
      green = 63;
      blue = 0;
      break;
    case 3:
      red = 31;
      green = 63 - 2 * (value % 32);
      blue = 0;
      break;
  }
  return (red << 11) + (green << 5) + blue;
}

/// @endcond
