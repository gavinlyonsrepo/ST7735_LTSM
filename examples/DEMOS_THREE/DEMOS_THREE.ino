/*!
	@file    DEMOS_THREE.ino
	@author  Gavin Lyons
	@brief   Example file for st7735 library. ST7735_LTSM library. demo
	@details See USER OPTIONS 1-3 in SETUP function
	@note  
	@test
		-# Test 410 Vertical Gauge Based on sin(x), cos(x), & sin(x)*cos(x). Updates over time to create a dynamic effect.
*/

// libraries
#include "ST7735_LTSM.hpp"
#include "fonts_LTSM/FontDefault_LTSM.hpp"

/// @cond

#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is defined. This example is not for that mode")
#endif

ST7735_LTSM myTFT;
bool bhardwareSPI = true;  // true for hardware spi, false for software

// Gauge properties
const int GAUGE_WIDTH = 20;
const int GAUGE_HEIGHT = 70;
const int GAUGE_X_START = 5;
const int GAUGE_Y_START = 10;
const int GAUGE_SPACING = 40;
// Store previous values to prevent redundant redraws
float prevVal1 = -1, prevVal2 = -1, prevVal3 = -1;

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
  Serial.println("Start demo 3");
}


// MAIN loop
void loop(void) {

  drawGaugeDemoThree(100);
  EndTests();
}
// *** End OF MAIN **

//  Function Space

void EndTests(void) {
  myTFT.TFTPowerDown();
  Serial.println("Demo Over");
  while (1) {};
}

// Draw border once 
void drawGaugeBorder(int x, int y) {
  myTFT.drawRectWH(x - 2, y - 2, GAUGE_WIDTH + 4, GAUGE_HEIGHT + 4, myTFT.C_WHITE);
}

// Gauge update 
void drawGauge(int x, int y, float value, float prevVal) {
  int fillHeight = static_cast<int>(GAUGE_HEIGHT * value);
  int prevFillHeight = (prevVal < 0) ? 0 : static_cast<int>(GAUGE_HEIGHT * prevVal);

  if (fillHeight == prevFillHeight) return;  // nothing to update

  // If shrinking, clear the difference 
  if (fillHeight < prevFillHeight) {
    int clearHeight = prevFillHeight - fillHeight;
    myTFT.fillRectBuffer(
      x,
      y + (GAUGE_HEIGHT - prevFillHeight),
      GAUGE_WIDTH,
      clearHeight,
      myTFT.C_BLACK);
  }
  // If growing, draw gradient for the new part
  if (fillHeight > prevFillHeight) {
    int growHeight = fillHeight - prevFillHeight;
    for (int i = 0; i < growHeight; i++) {
      uint8_t val = map(prevFillHeight + i, 0, GAUGE_HEIGHT - 1, 1, 127);
      uint16_t color = generateColor(val);
      myTFT.fillRectBuffer(
        x,
        y + (GAUGE_HEIGHT - fillHeight) + (growHeight - 1 - i),
        GAUGE_WIDTH,
        1,
        color);
    }
  }
}

// Update all gauges + numeric readouts
void updateGauges(float phase) {
  // Calculate values (0 → 1)
  float val1 = (std::sin(phase) + 1) / 2;
  float val2 = (std::cos(phase) + 1) / 2;
  float val3 = ((std::sin(phase) * std::cos(phase)) + 1) / 2;

  myTFT.setTextColor(myTFT.C_YELLOW, myTFT.C_BLACK);
  char buffer[6];  // formatted text
  // --- Gauge 1 ---
  if (val1 != prevVal1) {
    drawGauge(GAUGE_X_START, GAUGE_Y_START, val1, prevVal1);
    sprintf(buffer, "%.2f", val1);
    myTFT.fillRectBuffer(GAUGE_X_START, GAUGE_Y_START + GAUGE_HEIGHT + 10, 36, 8, myTFT.C_BLACK);
    myTFT.setCursor(GAUGE_X_START, GAUGE_Y_START + GAUGE_HEIGHT + 10);
    myTFT.print(buffer);
    prevVal1 = val1;
  }
  // --- Gauge 2 ---
  if (val2 != prevVal2) {
    drawGauge(GAUGE_X_START + GAUGE_SPACING, GAUGE_Y_START, val2, prevVal2);
    sprintf(buffer, "%.2f", val2);
    myTFT.fillRectBuffer(GAUGE_X_START + GAUGE_SPACING, GAUGE_Y_START + GAUGE_HEIGHT + 10, 36, 8, myTFT.C_BLACK);
    myTFT.setCursor(GAUGE_X_START + GAUGE_SPACING, GAUGE_Y_START + GAUGE_HEIGHT + 10);
    myTFT.print(buffer);
    prevVal2 = val2;
  }
  // --- Gauge 3 ---
  if (val3 != prevVal3) {
    drawGauge(GAUGE_X_START + 2 * GAUGE_SPACING, GAUGE_Y_START, val3, prevVal3);
    sprintf(buffer, "%.2f", val3);
    myTFT.fillRectBuffer(GAUGE_X_START + 2 * GAUGE_SPACING, GAUGE_Y_START + GAUGE_HEIGHT + 10, 36, 8, myTFT.C_BLACK);
    myTFT.setCursor(GAUGE_X_START + 2 * GAUGE_SPACING, GAUGE_Y_START + GAUGE_HEIGHT + 10);
    myTFT.print(buffer);
    prevVal3 = val3;
  }
}

// --- Main demo ---
void drawGaugeDemoThree(uint16_t countLimit) {
  Serial.print("Gauge Demo 3 ends at: ");
  Serial.println(countLimit);

  // Draw borders once
  myTFT.fillScreen(myTFT.C_BLACK);
  drawGaugeBorder(GAUGE_X_START, GAUGE_Y_START);
  drawGaugeBorder(GAUGE_X_START + GAUGE_SPACING, GAUGE_Y_START);
  drawGaugeBorder(GAUGE_X_START + 2 * GAUGE_SPACING, GAUGE_Y_START);

  float phase = 0;
  while (countLimit-- > 1) {
    updateGauges(phase);
    phase += 0.1;
    Serial.println(countLimit);
    delay(250);
  }
  // Clear at end
  myTFT.fillScreen(myTFT.C_BLACK);
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
