/*!
	@file    DEMOS.ino
	@author  Gavin Lyons
	@brief   Example file for st7735 library.  ST7735_LTSM library. demo
	@details See USER OPTIONS 1-3 in SETUP function
				If dislib16_ADVANCED_GRAPHICS_ENABLE is not enabled it will not compile,
				it is in file common_dis16_data_LTSM.hpp from display16_graphics_LTSM library.
	@note  Tested only on ESP32
	@test
		-# Test 408 gauge demo 
*/

// libraries
#include "ST7735_LTSM.hpp"
#include "fonts_LTSM/FontDefault_LTSM.hpp"  // Depends

/// @cond

#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is defined. This example is not for that mode")
#endif
#ifndef dislib16_ADVANCED_GRAPHICS_ENABLE
#pragma message("gll: dislib16_ADVANCED_GRAPHICS_ENABLE is not defined. it is required for this demo")
#endif

ST7735_LTSM myTFT;
bool bhardwareSPI = true;  // true for hardware spi, false for software

void setup(void) {
#ifdef dislib16_DEBUG_MODE_ENABLE
  Serial.begin(38400);
#endif
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
  myTFT.TFTInitPCBType(myTFT.TFT_ST7735R_Red);  // pass enum,multi choices,see README
  //**********************************************************
}


// MAIN loop
void loop(void) {
  arcGauge(200);
  EndTests();
}
// *** End OF MAIN **

//  Function Space

void EndTests(void) {
  myTFT.TFTPowerDown();
  Serial.println("Demo Over");
  while (1) {};
}

void arcGauge(uint16_t countLimit) {
  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.setTextColor(myTFT.C_YELLOW, myTFT.C_BLACK);
  Serial.print("Arc Demo ends at : ");
  Serial.println(countLimit);
  int16_t currentValue = 150;
  int16_t oldValue = 150;
  myTFT.setFont(FontDefault);
  char buffer[10];
  // Draw Gauge
  uint16_t count = 1;
  uint8_t x = 64;
  uint8_t y = 64;
  uint8_t radius = 48;
  const int16_t minValue = 1;
  const int16_t maxValue = 255;
  //myTFT.drawArc(centerX, centerY, radius, 10, 180.0f, 360.0f, myTFT.C_BLUE);
  myTFT.drawArc(x, y, radius, 11, 150.0f, 180.0f, myTFT.C_RED);
  myTFT.drawArc(x, y, radius, 11, 181.0f, 250.0f, myTFT.C_ORANGE);
  myTFT.drawArc(x, y, radius, 11, 251.0f, 320.0f, myTFT.C_YELLOW);
  myTFT.drawArc(x, y, radius, 11, 321.0f, 30.0f, myTFT.C_GREEN);
  drawGaugeMarkers(x, y, radius, 150, 390, 1.3);
  drawGaugeMarkers(x, y, radius, 165, 375, 1.1);
  // Initialize random generator , optional
  // randomSeed(analogRead(A0));
  while (count++ < countLimit) {
    int step = random(-10, 11);  // Arduino built-in RNG
    currentValue += step;
    currentValue = constrain(currentValue, minValue, maxValue);
    myTFT.setCursor(10, 100);
    if (oldValue != currentValue) {
      drawPointer(currentValue, oldValue, x, y, radius, myTFT.C_GREEN, myTFT.C_BLACK);
      oldValue = currentValue;
    }
    myTFT.print(" Count :: ");
    myTFT.print(count);
    myTFT.setCursor(10, 110);
    myTFT.print(" Value :: ");
    sprintf(buffer, "%03d", currentValue);
    myTFT.print(buffer);
  }

  myTFT.fillScreen(myTFT.C_BLACK);
}

void drawGaugeMarkers(uint8_t centerX, uint8_t centerY, uint8_t radius, int startAngle, int endAngle, float scaleFactor) {

  int angle;
  // Loop through the specified angle range, drawing ticks every 30 degrees
  for (angle = startAngle; angle <= endAngle; angle += 30) {
    float angleRad, innerX, innerY, outerX, outerY;
    // Convert degrees to radians
    angleRad = angle * (PI / 180);
    // inner marker position
    innerX = cos(angleRad) * (radius / scaleFactor);
    innerY = sin(angleRad) * (radius / scaleFactor);
    // outer marker position
    outerX = cos(angleRad) * radius;
    outerY = sin(angleRad) * radius;
    // Draw marker line from inner to outer radius
    myTFT.drawLine(1 + centerX + innerX, 1 + centerY + innerY,
                   1 + centerX + outerX, 1 + centerY + outerY,
                   myTFT.C_WHITE);
  }
}

void drawPointer(const int16_t &currentValue, const int16_t &oldValue, uint8_t x, uint8_t y, uint8_t r, uint16_t colour, uint16_t bcolour) {
  uint16_t i;
  if (currentValue > oldValue) {
    // Incrementally move the pointer from oldValue to currentValue
    for (i = oldValue; i <= currentValue; i++) {
      // Erase the previous pointer position
      drawPointerHelper(i - 1, x, y, r, bcolour);
      // Draw the pointer at the new position
      drawPointerHelper(i, x, y, r, colour);
      // Introduce a small delay for smooth animation
      if ((currentValue - oldValue) < 128) {
        delay(20);
      } else {
        delay(30);  // Slower update for small changes
      }
    }
  } else {
    // Decrementally move the pointer from oldValue to currentValue
    for (i = oldValue; i >= currentValue; i--) {
      // Erase the previous pointer position
      drawPointerHelper(i + 1, x, y, r, bcolour);
      // Draw the pointer at the new position
      drawPointerHelper(i, x, y, r, colour);
      // Adjust delay based on the difference to control animation speed
      if ((oldValue - currentValue) >= 128) {
        delay(20);  // Faster update for large changes
      } else {
        delay(30);  // Slower update for small changes
      }
    }
  }
}

void drawPointerHelper(int16_t value, uint8_t centerX, uint8_t centerY, uint8_t radius, uint16_t pointerColor) {
  float angleRad, pointerX, pointerY;
  int16_t minValue = 0;
  int16_t maxValue = 255;
  int startAngle = 150;
  int endAngle = 240;
  if (value > maxValue) value = maxValue;
  if (value < minValue) value = minValue;
  // Map value to an angle in radians based on the gauge range
  angleRad = (((float)(uint16_t)(value - minValue) / (float)(uint16_t)(maxValue - minValue) * endAngle)
              + startAngle)
             * (PI / 180);
  // Calculate pointer tip coordinates
  pointerX = cos(angleRad) * (radius / 1.35);
  pointerY = sin(angleRad) * (radius / 1.35);
  // Draw the pointer line from the center to the calculated tip position
  myTFT.drawLine(centerX, centerY, 1 + centerX + pointerX, 1 + centerY + pointerY, pointerColor);
  myTFT.fillCircle(centerX, centerY, 2, pointerColor);
}
/// @endcond
